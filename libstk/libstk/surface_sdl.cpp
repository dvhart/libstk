/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: SDL surface implementation.
 *     AUTHORS: Darren Hart, Vernon Mauery, Marc Strämke 
 *  START DATE: 2002/Dec/10
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

/* SDL has multi-threading problems in X.  To account for this, all library calls should be
 * wrapped with SDL_MUTEX_LOCK and SDL_MUTEX_UNLOCK.  The current definition of these macros
 * uses a boost recursive mutex and a scoped lock (meaning SDL_MUTEX_UNLOCK does nothing), but 
 * please include both in case the implementation of the locking mechanism changes in the future.
 * Note that the put_pixel and get_pixel routines are not wrapped for performance reasons, any call
 * to one of these methods must be made inside the lock.
 *
 * SDL requires the surface to be locked (SDL_SurfaceLock()) prior to any direct access to 
 * surface->pixels, for this reason the drawing routines which call any of the put_pixel or 
 * get_pixel private members should be wrapped in lock() and unlock().  Note, draw_line is wrapped,
 * draw_poly is not however because it calls draw_line.  Please be take care in using lock() and
 * unlock() appropriately.
 */

#include <cmath>
#include <vector>
#include <list>
#include <iostream>
#include <SDL/SDL.h>
#include <boost/thread/recursive_mutex.hpp>
#include "libstk/surface.h"
#include "libstk/surface_sdl.h"
#include "libstk/stk.h"
#include "libstk/sdl_data.h"
#include "libstk/overlay.h"
#include "libstk/overlay_sdl.h"

namespace stk
{

    surface_sdl::ptr surface_sdl::create(const rectangle& rect, bool fullscreen)
    {
        surface_sdl::ptr new_surface_sdl(new surface_sdl(rect, fullscreen));
        return new_surface_sdl;
    }

    surface_sdl::surface_sdl(const rectangle &rect, bool fullscreen) : surface_impl<surface_sdl>(rect)
    {
        // ensure that SDL has been initialized
        INFO("getting sdl_data pointer");
        sdl_data_ = sdl_data::get(); // reference counting
        sdl_data_->init();

        SDL_MUTEX_LOCK; // this must come after assigning sdl_data_ or SDL will init and quit

        // if this is the first surface, init video and set the video mode
        if (SDL_WasInit(SDL_INIT_VIDEO) == 0)
        {
            SDL_InitSubSystem(SDL_INIT_VIDEO);
            // FIXME: SDL_DOUBLEBUF causes a blackscreen in framebuffer mode
            sdl_surface_ = SDL_SetVideoMode(rect.width(), rect.height(), 32, 
                    SDL_HWSURFACE /*| SDL_DOUBLEBUF*/ | (fullscreen ? SDL_FULLSCREEN : 0) );
            if (!sdl_surface_)
                ERROR("SDL_SetVideoMode failed: " << SDL_GetError());
            primary_ = true;
            
        }
        // this is not the first surface, so make one size rect of the same format as the first
        else
        {
            // build a surface from the original and return that
            SDL_Surface* temp_surface = SDL_CreateRGBSurface(0, rect.width(), rect.height(), 32, 
                    RMASK, GMASK, BMASK, AMASK);
            if (!temp_surface)
                ERROR("SDL_CreateRGBSurface failed: " << SDL_GetError());
            sdl_surface_ = SDL_DisplayFormatAlpha(temp_surface); // convert to display format for faster blitting
            if (!sdl_surface_)
                ERROR("SDL_DisplayFormatAlpha failed: " << SDL_GetError());
            SDL_FreeSurface(temp_surface);
            SDL_SetAlpha(sdl_surface_, SDL_SRCALPHA | SDL_RLEACCEL, SDL_ALPHA_OPAQUE);
            primary_ = false;
        }
        SDL_MUTEX_UNLOCK;
    }

    surface_sdl::~surface_sdl()
    {
        SDL_MUTEX_LOCK;
        // don't free the primary surface (returned by SDL_SetVideoMode), SDL_Quit does that!
        if (sdl_surface_ && !primary_)
            SDL_FreeSurface(sdl_surface_);
        SDL_MUTEX_UNLOCK;
    }

    void surface_sdl::put_pixel(int x, int y, color clr)
    {
        x += offset_.x();
        y += offset_.y();

        if(!clip_rect_.contains(x,y)) return;

        Uint32 sdl_color = (Uint32)clr;

        // DELETEME check for out of bounds
        if (x < 0 || y < 0 || x >= sdl_surface_->w || y >= sdl_surface_->h)
        {
            //throw error_message_exception("surface_sdl::put_pixel() - pixel coords out of bounds");
            INFO("surface_sdl::put_pixel() - pixel coords out of bounds");
            return;
        }

        switch (sdl_surface_->format->BytesPerPixel)
        {
        case 1:                                       // Assuming 8-bpp
        {
            // untested
            Uint8 *bufp;
            bufp = (Uint8 *) sdl_surface_->pixels + y * sdl_surface_->pitch + x;
            *bufp = sdl_color;
        }
        break;
        case 2:                                       // Probably 15-bpp or 16-bpp
        {
            // untested
            Uint16 *bufp;
            bufp = (Uint16 *) sdl_surface_->pixels + y * sdl_surface_->pitch / 2 + x;
            *bufp = sdl_color;
        }
        break;
        case 3:                                       // Slow 24-bpp mode, usually not used
        {
            // untested
            Uint8 *bufp;
            bufp = (Uint8 *) sdl_surface_->pixels + y * sdl_surface_->pitch + x * 3;
            if (SDL_BYTEORDER == SDL_LIL_ENDIAN)
            {
                bufp[0] = sdl_color;
                bufp[1] = sdl_color >> 8;
                bufp[2] = sdl_color >> 16;
            }
            else
            {
                bufp[2] = sdl_color;
                bufp[1] = sdl_color >> 8;
                bufp[0] = sdl_color >> 16;
            }
        }
        break;
        case 4:                                       // Probably 32-bpp
        {
            Uint32 *bufp;
            bufp = (Uint32 *)sdl_surface_->pixels + y*sdl_surface_->pitch/4 + x;
            *bufp = sdl_color;
        }
        break;
        }
    }

    void surface_sdl::put_pixel_aa(int x, int y, double distance, color clr)
    {
        Uint32 color_a = (Uint32)clr;

        Uint8 red_a, green_a, blue_a, alpha_a;
        SDL_GetRGB(color_a, sdl_surface_->format, &red_a, &green_a, &blue_a);
        alpha_a = filter_aa((fabs(distance)));

        Uint8 red_b, green_b, blue_b, alpha_b;
        Uint32 color_b = get_pixel(x, y);
        SDL_GetRGBA(color_b, sdl_surface_->format, &red_b, &green_b, &blue_b, &alpha_b);

        Uint8 red_o, green_o, blue_o, alpha_o;
        alpha_o = composite_alpha(alpha_a, alpha_b);
        float alpha_a_f = alpha_a / 255.0;
        float alpha_b_f = alpha_b / 255.0;
        float alpha_o_f = alpha_o / 255.0;

        red_o   = composite_a_over_b(red_a, red_b, alpha_a_f, alpha_b_f, alpha_o_f);
        green_o = composite_a_over_b(green_a, green_b, alpha_a_f, alpha_b_f, alpha_o_f);
        blue_o  = composite_a_over_b(blue_a, blue_b, alpha_a_f, alpha_b_f, alpha_o_f);

        put_pixel(x, y, gen_color(red_o, green_o, blue_o, alpha_o));
    }

    void surface_sdl::put_pixel_aa(int x, int y, unsigned char alpha_a, color clr)
    {
        Uint32 color_a = (Uint32)clr;
        Uint8 red_a, green_a, blue_a;
        SDL_GetRGB(color_a, sdl_surface_->format, &red_a, &green_a, &blue_a);

        Uint8 red_b, green_b, blue_b, alpha_b;
        Uint32 color_b = get_pixel(x, y);
        SDL_GetRGBA(color_b, sdl_surface_->format, &red_b, &green_b, &blue_b, &alpha_b);

        Uint8 red_o, green_o, blue_o, alpha_o;
        alpha_o = composite_alpha(alpha_a, alpha_b);
        float alpha_a_f = alpha_a / 255.0;
        float alpha_b_f = alpha_b / 255.0;
        float alpha_o_f = alpha_o / 255.0;

        red_o   = composite_a_over_b(red_a, red_b, alpha_a_f, alpha_b_f, alpha_o_f);
        green_o = composite_a_over_b(green_a, green_b, alpha_a_f, alpha_b_f, alpha_o_f);
        blue_o  = composite_a_over_b(blue_a, blue_b, alpha_a_f, alpha_b_f, alpha_o_f);

        put_pixel(x, y, gen_color(red_o, green_o, blue_o, alpha_o));
    }

    color surface_sdl::get_pixel(int x, int y) const
    {
        x += offset_.x();
        y += offset_.y();
        // DELETEME check for out of bounds
        if (x < 0 || y < 0 || x >= sdl_surface_->w || y >= sdl_surface_->h)
        {
            //throw error_message_exception("surface_sdl::get_pixel() - pixel coords out of bounds");
            //INFO("surface_sdl::get_pixel() - pixel coords out of bounds");
            return 0;
        }

        Uint32 sdl_color = 0;
        switch (sdl_surface_->format->BytesPerPixel)
        {
        case 1:                                       // Assuming 8-bpp
        {
            // untested
            Uint8 *bufp;
            bufp = (Uint8 *) sdl_surface_->pixels + y * sdl_surface_->pitch + x;
            sdl_color = (Uint32)(*bufp);
        }
        break;
        case 2:                                       // Probably 15-bpp or 16-bpp
        {
            // untested
            Uint16 *bufp;
            bufp = (Uint16 *) sdl_surface_->pixels + y * sdl_surface_->pitch / 2 + x;
            sdl_color = (Uint32)(*bufp);
        }
        break;
        case 3:                                       // Slow 24-bpp mode, usually not used
        {
            // untested
            Uint8 *bufp;
            bufp = (Uint8 *) sdl_surface_->pixels + y * sdl_surface_->pitch + x * 3;
            if (SDL_BYTEORDER == SDL_LIL_ENDIAN)
            {
                sdl_color =  bufp[0];
                sdl_color += (bufp[1] >> 8);
                sdl_color += (bufp[2] >> 16);
            }
            else
            {
                sdl_color =  bufp[2];
                sdl_color += (bufp[1] >> 8);
                sdl_color += (bufp[0] >> 16);
            }
        }
        break;
        case 4:                                       // Probably 32-bpp
        {
            Uint32 *bufp;
            bufp = (Uint32 *) sdl_surface_->pixels + y * sdl_surface_->pitch / 4 + x;
            sdl_color = *bufp;
        }
        break;
        }

        return (color)sdl_color; // should never happen
    }

    color surface_sdl::gen_color(Uint8 r, Uint8 g, Uint8 b, Uint8 a) const
    {
        SDL_MUTEX_LOCK;
        Uint32 sdl_color = SDL_MapRGBA(sdl_surface_->format, r, g, b, a);
        SDL_MUTEX_UNLOCK;
        return (color)sdl_color;
    }

    void surface_sdl::lock()
    {
        SDL_MUTEX_LOCK;
        if (SDL_LockSurface(sdl_surface_))
            throw error_message_exception("surface_sdl::lock() failed - " + 
                    std::string(SDL_GetError()));
        SDL_MUTEX_UNLOCK;
    }
    
    void surface_sdl::lock(rectangle &rect, int flags, color** buf, int& stride)
    {
        SDL_MUTEX_LOCK;
        // FIXME: what shall we do with flags (i.e. read and write)
        SDL_LockSurface(sdl_surface_);
        switch (sdl_surface_->format->BytesPerPixel)
        {
        case 1:                                       
        case 2:                                      
        case 3:                                     
        {
            // FIXME: provide a means to use non 32 bit surfaces
            //        perhaps set a format variable
            // create a virtual framebuffer for the region requested
            WARN("surface_sdl::lock - non 32 bit surface, unable to set buf");
            break;
        }
        case 4:                                       // Probably 32-bpp
            *buf = (color*)sdl_surface_->pixels + rect.y1()*sdl_surface_->pitch/4 + rect.x1();
            break;
        }
        stride = sdl_surface_->pitch;
        SDL_MUTEX_UNLOCK;
    }

    void surface_sdl::unlock()
    {
        SDL_MUTEX_LOCK;
        SDL_UnlockSurface(sdl_surface_);
        SDL_MUTEX_UNLOCK;
    }

    void surface_sdl::update(const rectangle& u_rect)
    {
        SDL_MUTEX_LOCK;
        //INFO("updating SDL surface with u_rect=" << u_rect);
        if (u_rect.empty())
        {
            //INFO("surface_sdl::update() - updating entire screen");
            SDL_Flip(sdl_surface_); // note: this is hw accelerated if supported,
            // same as SDL_UpdateRect(surface, 0,0,0,0) otherwise
            // would it be faster to call SDL_Flip regardless of u_rect
            // if we have accelerated hw ?
        }
        else
        {
            //INFO("surface_sdl::update() - updating the rect: " << u_rect);
            // The SDL_UpdateRect call fails if the rect is outside the screen! 
            rectangle u_rect_clipped = u_rect.intersection(rect_);
            SDL_UpdateRect(sdl_surface_, u_rect_clipped.x1(), u_rect_clipped.y1(),
                           u_rect_clipped.width(), u_rect_clipped.height());
        }
        SDL_MUTEX_UNLOCK;
    }

    overlay::ptr surface_sdl::create_overlay(int width, int height, int format)
    {
        return overlay_sdl::create(width, height, format, sdl_surface_);
    }
    
    surface::ptr surface_sdl::create_surface(const rectangle& rect)
    {
        return surface_sdl::create(rect);
    }

    // overridden drawing routines
    color surface_sdl::read_pixel(int x, int y)
    {
        SDL_MUTEX_LOCK;
        color clr = get_pixel(x, y);
        SDL_MUTEX_UNLOCK;
        return clr;
    }
    void surface_sdl::clip_rect(const rectangle& clip_rectangle)
    {
        rectangle clip_rect = clip_rectangle;
        clip_rect.position(clip_rectangle.position()+offset_);

        SDL_MUTEX_LOCK;
        surface::clip_rect(clip_rect);
        SDL_Rect sdl_rect = rect_to_sdl_rect(clip_rect);
        SDL_SetClipRect(sdl_surface_, &sdl_rect);
        SDL_MUTEX_UNLOCK;
    }
    
    void surface_sdl::blit(surface& dst_surface)
    {
        SDL_MUTEX_LOCK;
        
        // blit the local surface to the destination surface
        surface_sdl *dst_surface_ptr = dynamic_cast<surface_sdl *>(&dst_surface);
        if (dst_surface_ptr != NULL)
        {
            SDL_Rect dst_sdl_rect = rect_to_sdl_rect(rect_);

            if (SDL_BlitSurface(sdl_surface_, NULL,
                                dst_surface_ptr->sdl_surface(), &dst_sdl_rect) < 0)
            {
                throw error_message_exception("widget: Failed to blit sdl_surface_ to dst_surface: "
                        + std::string(SDL_GetError()));
            }
        }
        else
        {
            throw error_message_exception("surface_sdl::blit - "
                                          "unable to cast dst_surface to surface_sdl\n"
                                          "Can only blit an sdl_surface to another sdl_surface");
        }
        SDL_MUTEX_UNLOCK;
    }

    void surface_sdl::blit(surface& dst_surface, rectangle src_rect, rectangle dst_rect)
    {
        SDL_MUTEX_LOCK;

        dst_rect.position(dst_rect.position()+dst_surface.offset());
        
        INFO("blit: src(" << src_rect << ")  dst_rect(" << dst_rect << ")");
        // blit the local surface to the destination surface
        surface_sdl *dst_surface_ptr = dynamic_cast<surface_sdl *>(&dst_surface);
        if (dst_surface_ptr != NULL)
        {
	    SDL_Rect src_sdl_rect = rect_to_sdl_rect(src_rect);
	    SDL_Rect dst_sdl_rect = rect_to_sdl_rect(dst_rect);
	    
	    if (SDL_BlitSurface(sdl_surface_, &src_sdl_rect,
                        dst_surface_ptr->sdl_surface(), &dst_sdl_rect) < 0)
            {
                throw error_message_exception("widget: Failed to blit sdl_surface_ to dst_surface_: " 
                        + std::string(SDL_GetError()));
            }
        }
        else
        {
            throw error_message_exception("surface_sdl::blit - "
                    "unable to cast dst_surface to surface_sdl\n"
                    "Can only blit an sdl_surface to another sdl_surface");
        }
        SDL_MUTEX_UNLOCK;
    }

    // non antialiased draw routines
    void surface_sdl::draw_pixel(int x, int y, color clr)
    {
        SDL_MUTEX_LOCK;
        lock();
        put_pixel(x, y, clr);
        unlock();
        SDL_MUTEX_UNLOCK;
    }
    void surface_sdl::draw_line(int x1, int y1, int x2, int y2)
    {
        SDL_MUTEX_LOCK;
        lock();
        surface_impl<surface_sdl>::draw_line(x1, y1, x2, y2);
        unlock();
        SDL_MUTEX_UNLOCK;
    }
    void surface_sdl::draw_arc(const rectangle& rect, int quadrant)
    {
        INFO(__FUNCTION__ << " not implemented");
    }
    void surface_sdl::draw_arc(int x1, int y1, int x2, int y2, int quadrant)
    {
        INFO(__FUNCTION__ << " not implemented");
    }
    //void surface_sdl::draw_rect(const rectangle &rect) { }
    void surface_sdl::draw_rect(int x1, int y1, int x2, int y2)
    {
        SDL_MUTEX_LOCK;
        surface_impl<surface_sdl>::draw_rect(x1, y1, x2, y2);
        SDL_MUTEX_UNLOCK;
    }
    void surface_sdl::draw_circle(int x, int y, int radius)
    {
        INFO(__FUNCTION__ << " not implemented");
    }
    void surface_sdl::draw_circle(const rectangle &rect)
    {
        INFO(__FUNCTION__ << " not implemented");
    }
    void surface_sdl::draw_ellipse(int x, int y, int a, int b)
    {
        INFO(__FUNCTION__ << " not implemented");
    }
    void surface_sdl::draw_ellipse(const rectangle &rect)
    {
        INFO(__FUNCTION__ << " not implemented");
    }
    void surface_sdl::draw_poly(std::vector<point> points)
    {
        SDL_MUTEX_LOCK;
        surface_impl<surface_sdl>::draw_poly(points);
        SDL_MUTEX_UNLOCK;
    }
    void surface_sdl::draw_text(const rectangle& rect, const std::wstring &text, int kerning_mode)
    {
        SDL_MUTEX_LOCK;
        lock();
        surface_impl<surface_sdl>::draw_text(rect, text, kerning_mode);
        unlock();
        SDL_MUTEX_UNLOCK;
    }
    void surface_sdl::draw_image(int x, int y, image::ptr img)
    {
        SDL_MUTEX_LOCK;
        surface_impl<surface_sdl>::draw_image(x, y, img);
        SDL_MUTEX_UNLOCK;
    }
    // antialiased draw routines
    void surface_sdl::draw_pixel_aa(int x, int y, double distance, color clr)
    {
        lock();
        put_pixel_aa(x, y, distance, clr);
        unlock();
    }
    void surface_sdl::draw_pixel_aa(int x, int y, unsigned char alpha_a, color clr)
    {
        lock();
        draw_pixel_aa(x, y, alpha_a, clr);
        unlock();
    }
    void surface_sdl::draw_line_aa(int x1, int y1, int x2, int y2)
    {
        SDL_MUTEX_LOCK;
        lock();
        surface_impl<surface_sdl>::draw_line_aa(x1, y1, x2, y2);
        unlock();
        SDL_MUTEX_UNLOCK;
    }
    void surface_sdl::draw_arc_aa(const rectangle &rect, int quadrant)
    {
        INFO(__FUNCTION__ << " not implemented");
    }
    void surface_sdl::draw_arc_aa(int x1, int y1, int x2, int y2, int quadrant)
    {
        INFO(__FUNCTION__ << " not implemented");
    }
    void surface_sdl::draw_rect_aa(const rectangle &rect)
    {
        INFO(__FUNCTION__ << " not implemented");
    }
    void surface_sdl::draw_rect_aa(int x1, int y1, int x2, int y2)
    {
        INFO(__FUNCTION__ << " not implemented");
    }
    void surface_sdl::draw_circle_aa(int x, int y, int radius)
    {
        INFO(__FUNCTION__ << " not implemented");
    }
    void surface_sdl::draw_circle_aa(const rectangle &rect)
    {
        INFO(__FUNCTION__ << " not implemented");
    }
    void surface_sdl::draw_ellipse_aa(int x, int y, int a, int b)
    {
        INFO(__FUNCTION__ << " not implemented");
    } 
    void surface_sdl::draw_ellipse_aa(const rectangle &rect)
    {
        INFO(__FUNCTION__ << " not implemented");
    }
    void surface_sdl::draw_poly_aa(std::vector<point> points)
    {
        SDL_MUTEX_LOCK;
        surface_impl<surface_sdl>::draw_poly_aa(points);
        SDL_MUTEX_UNLOCK;
    }
    
    // non antialiased fill routines
    void surface_sdl::fill_rect(int x1, int y1, int x2, int y2)
    {
        SDL_MUTEX_LOCK;
        x1 += offset_.x();
        y1 += offset_.y();
        x2 += offset_.x();
        y2 += offset_.y();
        Uint32 sdl_color = (Uint32)gc_->fill_color();
        SDL_Rect rect = {x1, y1, x2 - x1, y2 - y1};
        SDL_FillRect(sdl_surface_, &rect, sdl_color);
        SDL_MUTEX_UNLOCK;
    }

    void surface_sdl::fill_rect(const rectangle& rect)
    {
        SDL_MUTEX_LOCK;
        rectangle t_rect = rect;
        t_rect.position(t_rect.p1() + offset_);
        Uint32 sdl_color = (Uint32)gc_->fill_color();
        SDL_Rect sdl_rect = rect_to_sdl_rect(t_rect);
        SDL_FillRect(sdl_surface_, &sdl_rect, sdl_color);
        SDL_MUTEX_UNLOCK;
    }

    void surface_sdl::fill_circle(int x, int y, int radius)
    {
        INFO(__FUNCTION__ << " not implemented");
    }
    void surface_sdl::fill_circle(const rectangle &rect)
    {
        INFO(__FUNCTION__ << " not implemented");
    }
    void surface_sdl::fill_ellipse(int x, int y, int a, int b)
    {
        INFO(__FUNCTION__ << " not implemented");
    }
    void surface_sdl::fill_ellipse(const rectangle &rect)
    {
        INFO(__FUNCTION__ << " not implemented");
    }
    void surface_sdl::fill_poly(std::vector<point> points)
    {
        SDL_MUTEX_LOCK;
        surface_impl<surface_sdl>::fill_poly(points);
        SDL_MUTEX_UNLOCK;
    }
    // antialiased fill routines
    void surface_sdl::fill_rect_aa(int x1, int y1, int x2, int y2)
    {
        INFO(__FUNCTION__ << " not implemented");
    }
    void surface_sdl::fill_rect_aa(const rectangle &rect)
    {
        INFO(__FUNCTION__ << " not implemented");
    }
    void surface_sdl::fill_circle_aa(int x, int y, int radius)
    {
        INFO(__FUNCTION__ << " not implemented");
    }
    void surface_sdl::fill_circle_aa(const rectangle &rect)
    {
        INFO(__FUNCTION__ << " not implemented");
    }
    void surface_sdl::fill_ellipse_aa(int x, int y, int a, int b)
    {
        INFO(__FUNCTION__ << " not implemented");
    }
    void surface_sdl::fill_ellipse_aa(const rectangle &rect)
    {
        INFO(__FUNCTION__ << " not implemented");
    }
    void surface_sdl::fill_poly_aa(std::vector<point> points)
    {
        SDL_MUTEX_LOCK;
        surface_impl<surface_sdl>::fill_poly_aa(points);
        SDL_MUTEX_UNLOCK;
    }


} // end namespace stk
