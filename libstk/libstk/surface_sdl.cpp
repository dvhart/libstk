/**************************************************************************************************
 *    FILENAME: surface_sdl.cpp
 * DESCRIPTION: SDL surface implementation.
 *     AUTHORS: Darren Hart, Vernon Mauery, Marc Straemke 
 *  START DATE: 10/Dec/2002  LAST UPDATE: 20/Jul/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 **************************************************************************************************/

// ***********************************************************
// FIXME: we simply cast Uint32 sdl colors to stk colors,
// we need to determine how exactly this mapping works
// ***********************************************************

#include <math.h>
#include <vector>
#include <list>
#include <iostream>
#include <SDL/SDL.h>
#include "libstk/surface.h"
#include "libstk/surface_sdl.h"
#include "libstk/stk.h"
#include "libstk/sdl_data.h"
#include "libstk/overlay.h"
#include "libstk/overlay_sdl.h"

using std::cout;
using std::endl;
using std::cerr;

namespace stk
{

    surface_sdl::ptr surface_sdl::create(const rectangle& rect)
    {
        surface_sdl::ptr new_surface_sdl(new surface_sdl(rect));
        return new_surface_sdl;
    }

    surface_sdl::surface_sdl(const rectangle &rect) : surface_impl<surface_sdl>(rect)
    {
        cout << "surface_sdl::surface_sdl()" << endl;

        // ensure that SDL has been initialized
        sdl_data_ = sdl_data::get
                        (); // reference counting
        sdl_data_->init();

        int bpp = 32;

        // if this is the first surface, init video and set the video mode
        if (SDL_WasInit(SDL_INIT_VIDEO) == 0)
        {
            SDL_InitSubSystem(SDL_INIT_VIDEO);
            sdl_surface_ = SDL_SetVideoMode(rect.width(), rect.height(), bpp,
                                            SDL_HWSURFACE | SDL_DOUBLEBUF /*|SDL_FULLSCREEN*/); // FIXME: use the flags
        }
        // this is not the first surface, so make one size rect of the same format as the first
        else
        {
            // build a surface from the original and return that
            SDL_Surface* temp_surface = SDL_CreateRGBSurface(SDL_SRCALPHA,
                                        rect.width(), rect.height(), bpp, RMASK, GMASK, BMASK, AMASK);
            sdl_surface_ = SDL_DisplayFormatAlpha(temp_surface); // convert to display format for faster blitting
            SDL_FreeSurface(temp_surface);
            SDL_SetAlpha(sdl_surface_, SDL_SRCALPHA | SDL_RLEACCEL, SDL_ALPHA_OPAQUE);
        }
    }

    surface_sdl::~surface_sdl()
    {
        cout << "surface_sdl::~surface_sdl()" << endl;
        if (sdl_surface_)
            SDL_FreeSurface(sdl_surface_);
    }

    color surface_sdl::gen_color(const std::string &str_color) const
    {
        Uint8 r, g, b, a;
        Uint32 int_color = strtoll(str_color.c_str(), NULL, 16);
        r = (int_color & RMASK) >> RSHIFT;
        g = (int_color & GMASK) >> GSHIFT;
        b = (int_color & BMASK) >> BSHIFT;
        a = (int_color & AMASK) >> ASHIFT;
        Uint32 sdl_color = gen_color(r, g, b, a);
        return (color)sdl_color;
    }

    color surface_sdl::gen_color(Uint8 r, Uint8 g, Uint8 b, Uint8 a) const
    {
        Uint32 sdl_color = SDL_MapRGBA(sdl_surface_->format, r, g, b, a);
        return (color)sdl_color;
    }

    void surface_sdl::lock(rectangle &rect, int flags, color** buf, int &stride)
    {}

    void surface_sdl::unlock()
    {}

    void surface_sdl::update(const rectangle& u_rect)
    {
        if (u_rect.empty())
        {
            //cout << "surface_sdl::update() - updating entire screen" << endl;
            SDL_Flip(sdl_surface_); // FIXME: this is hw accelerated if supported,
            // same as SDL_UpdateRect(surface, 0,0,0,0) otherwise
            // would it be faster to call SDL_Flip regardless of u_rect
            // if we have accelerated hw ?
        }
        else
        {
            //cout << "surface_sdl::update() - updating the rect: " << u_rect << endl;
            SDL_UpdateRect(sdl_surface_, u_rect.x1(), u_rect.y1(), u_rect.width(), u_rect.height());
        }
    }

    overlay::ptr surface_sdl::create_overlay(int width, int height, int format)
    {
        return overlay_sdl::create(width, height, format, sdl_surface_);
    }
    

    // optimized pixel routines
    void surface_sdl::blit(surface &dst_surface)
    {
        // blit the local surface to the destination surface
        surface_sdl *dst_surface_ptr = dynamic_cast<surface_sdl *>(&dst_surface);
        if (dst_surface_ptr != NULL)
        {
            SDL_Rect dst_sdl_rect = rect_to_sdl_rect(rect_);

            if (SDL_BlitSurface(sdl_surface_, NULL,
                                dst_surface_ptr->sdl_surface(), &dst_sdl_rect) < 0)
            {
                throw error_message_exception(
                    "widget: Failed to blit sdl_surface_ to screen");
            }
        }
        else
        {
            throw error_message_exception("surface_sdl::blit - "
                                          "unable to cast dst_surface to surface_sdl\n"
                                          "Can only blit an sdl_surface to another sdl_surface");
        }
    }
    void surface_sdl::blit(surface &dst_surface, rectangle src_rect, rectangle dst_rect)
    {
        // blit the local surface to the destination surface
        surface_sdl *dst_surface_ptr = dynamic_cast<surface_sdl *>(&dst_surface);
        if (dst_surface_ptr != NULL)
        {
	    SDL_Rect src_sdl_rect = rect_to_sdl_rect(src_rect);
	    SDL_Rect dst_sdl_rect = rect_to_sdl_rect(dst_rect);
	    
	    if (SDL_BlitSurface(sdl_surface_, &src_sdl_rect ,
                                dst_surface_ptr->sdl_surface(), &dst_sdl_rect) < 0)
            {
                throw error_message_exception(
                    "widget: Failed to blit sdl_surface_ to screen");
            }
        }
        else
        {
            throw error_message_exception("surface_sdl::blit - "
                                          "unable to cast dst_surface to surface_sdl\n"
                                          "Can only blit an sdl_surface to another sdl_surface");
        }
    }

    void surface_sdl::put_pixel(int x_, int y_, color clr)
    {
        int x = x_ + offset_.x();
        int y = y_ + offset_.y();

        if(!clip_rect_.contains(x,y))
            return;

        Uint32 sdl_color = (Uint32)clr;

        // DELETEME check for out of bounds
        if (x < 0 || y < 0 || x >= sdl_surface_->w || y >= sdl_surface_->h)
        {
            //throw error_message_exception("surface_sdl::put_pixel() - pixel coords out of bounds");
            cout << "surface_sdl::put_pixel() - pixel coords out of bounds" << endl;
            return;
        }

        switch (sdl_surface_->format->BytesPerPixel)
        {
        case 1:                                       // Assuming 8-bpp
            {
                Uint8 *bufp;
                bufp = (Uint8 *) sdl_surface_->pixels + y * sdl_surface_->pitch + x;
                *bufp = sdl_color;
            }
            break;
        case 2:                                       // Probably 15-bpp or 16-bpp
            {
                Uint16 *bufp;
                bufp = (Uint16 *) sdl_surface_->pixels + y * sdl_surface_->pitch / 2 + x;
                *bufp = sdl_color;
            }
            break;
        case 3:                                       // Slow 24-bpp mode, usually not used
            {
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
                bufp = (Uint32 *) sdl_surface_->pixels + y * sdl_surface_->pitch / 4 + x;
                *bufp = sdl_color;
            }
            break;
        }
    }

    color surface_sdl::get_pixel(int x_, int y_) const
    {
        int x=x_+offset_.x();
        int y=y_+offset_.y();
        // DELETEME check for out of bounds
        if (x < 0 || y < 0 || x >= sdl_surface_->w || y >= sdl_surface_->h)
        {
            //throw error_message_exception("surface_sdl::get_pixel() - pixel coords out of bounds");
            cout << "surface_sdl::get_pixel() - pixel coords out of bounds" << endl;
            return 0;
        }

        switch (sdl_surface_->format->BytesPerPixel)
        {
        case 1:                                       // Assuming 8-bpp
            {
                Uint8 *bufp;
                bufp = (Uint8 *) sdl_surface_->pixels + y * sdl_surface_->pitch + x;
                return (Uint32)(*bufp);
            }
            break;
        case 2:                                       // Probably 15-bpp or 16-bpp
            {
                Uint16 *bufp;
                bufp = (Uint16 *) sdl_surface_->pixels + y * sdl_surface_->pitch / 2 + x;
                return (Uint32)(*bufp);
            }
            break;
        case 3:                                       // Slow 24-bpp mode, usually not used
            {
                // FIXME
                Uint8 *bufp;
                bufp = (Uint8 *) sdl_surface_->pixels + y * sdl_surface_->pitch + x * 3;
                Uint32 sdl_color;
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
                return (color)sdl_color;
            }
            break;
        case 4:                                       // Probably 32-bpp
            {
                Uint32 *bufp;
                bufp = (Uint32 *) sdl_surface_->pixels + y * sdl_surface_->pitch / 4 + x;
                return (color)*bufp;
            }
            break;
        }
        return 0; // should never happen
    }

    void surface_sdl::put_pixel_aa(int x, int y, double distance, color clr)
    {
        // FIXME: confirm the format of clr
        // will it always be in SDL color format?
        // or will it always be in some 32 bit stk RGBA format ??
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
        // FIXME: confirm the format of clr
        // will it always be in SDL color format?
        // or will it always be in some 32 bit stk RGBA format ??
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

    // overridden drawing routines
    void surface_sdl::fill_rect(int x1, int y1, int x2, int y2)
    {
        x1 += offset().x();
        y1 += offset().y();
        Uint32 sdl_color = (Uint32)gc_->fill_color();
        rectangle rect(x1, y1, x2 - x1, y2 - y1);
        SDL_Rect sdl_rect = rect_to_sdl_rect(rect);
        SDL_FillRect(sdl_surface_, &sdl_rect, sdl_color);
    }

    void surface_sdl::fill_rect(const rectangle& rect)
    {
        rectangle t_rect = rect;
        t_rect.position(t_rect.p1() + offset());
        Uint32 sdl_color = (Uint32)gc_->fill_color();
        SDL_Rect sdl_rect = rect_to_sdl_rect(t_rect);
        SDL_FillRect(sdl_surface_, &sdl_rect, sdl_color);
    }


} // end namespace stk
