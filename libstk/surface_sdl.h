/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: SDL surface backend.
 *     AUTHORS: Darren Hart, Vernon Mauery, Marc Str�mke
 *  START DATE: 2002/Dec/10
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Str�mke, Dirk H�rner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_SURFACE_SDL_H
#define STK_SURFACE_SDL_H

#include <string>
#include <vector>
#include <SDL/SDL.h>
#include <libstk/surface.h>
#include <libstk/surface_impl.h>
#include <libstk/point.h>
#include <libstk/stk_types.h>
#include <libstk/sdl_data.h>

// this is opposite from the docs, but it works
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
#define RMASK  0xff000000
#define GMASK  0x00ff0000
#define BMASK  0x0000ff00
#define AMASK  0x000000ff
#define RSHIFT 24
#define GSHIFT 16
#define BSHIFT 8
#define ASHIFT 0
#else
#define RMASK 0x000000ff
#define GMASK 0x0000ff00
#define BMASK 0x00ff0000
#define AMASK 0xff000000
#define RSHIFT 0
#define GSHIFT 8
#define BSHIFT 16
#define ASHIFT 24
#endif

namespace stk
{
    class overlay;
    
    class surface_sdl : public surface_impl<surface_sdl>
    {
    public:
        typedef boost::shared_ptr<surface_sdl> ptr;
        typedef boost::weak_ptr<surface_sdl> weak_ptr;

    private:
        SDL_Surface* sdl_surface_;
        sdl_data::ptr sdl_data_;
        bool primary_;

        SDL_Rect rect_to_sdl_rect(const rectangle& rect)
        {
            SDL_Rect sdl_rect = { rect.x1(), rect.y1(), rect.width(), rect.height() };
            return sdl_rect;
        }

    protected:
        surface_sdl(const rectangle& rect, bool fullscreen);

    public:
        /// \param fullscreen since SDL is meant as a development surface, you can specify fullscreen on creation of the initial surface
        static surface_sdl::ptr create(const rectangle& rect, bool fullscreen=true);
        ~surface_sdl();

        SDL_Surface* sdl_surface() const
        {
            return sdl_surface_;
        }
            
        // methods which MUST be implemented in derived classes
        // FIXME: make these 4 protected!
        inline void put_pixel(int x, int y, color clr);
        inline void put_pixel_aa(int x, int y, double distance, color clr);
        inline void put_pixel_aa(int x, int y, unsigned char alpha_a, color clr);
        inline color get_pixel(int x, int y) const;

        virtual color gen_color(byte r, byte g, byte b, byte a) const;
        virtual color rgba_color(color clr) const;
        virtual void lock();
        virtual void lock(rectangle &rect, int flags, color** buf, int &stride);
        virtual void unlock();
        virtual void update(const rectangle& u_rect=rectangle());
        virtual boost::shared_ptr<overlay> create_overlay(int width, int height, int format);
        virtual surface::ptr create_surface(const rectangle& rect);

        // overridden drawing routines
        virtual color read_pixel(int x, int y);
        virtual void clip_rect(const rectangle& clip_rectangle);
        // FIXME: make the blit accept "const rectangle&"s
        virtual void blit(surface &dst_surface);
	virtual void blit(surface &dst_surface, rectangle src_rect, rectangle dst_rect);
        // non antialiased draw routines
        virtual void draw_pixel(int x, int y, color clr);
        virtual void draw_line(int x1, int y1, int x2, int y2);
        virtual void draw_arc(const rectangle& rect, int quadrant);
        virtual void draw_arc(int x1, int y1, int x2, int y2, int quadrant);
        //virtual void draw_rect(const rectangle &rect);
        virtual void draw_rect(int x1, int y1, int x2, int y2);
        virtual void draw_circle(int x, int y, int radius);
        virtual void draw_circle(const rectangle &rect);
        virtual void draw_ellipse(int x, int y, int a, int b);
        virtual void draw_ellipse(const rectangle &rect);
        virtual void draw_poly(std::vector<point> points);
        virtual void draw_text(const rectangle& rect, const std::wstring &text, int kerning_mode);
        virtual void draw_image(int x, int y, image::ptr img);
        // antialiased draw routines
        virtual void draw_pixel_aa(int x, int y, double distance, color clr);
        virtual void draw_pixel_aa(int x, int y, unsigned char alpha_a, color clr);
        virtual void draw_line_aa(int x1, int y1, int x2, int y2);
        virtual void draw_arc_aa(const rectangle &rect, int quadrant);
        virtual void draw_arc_aa(int x1, int y1, int x2, int y2, int quadrant);
        virtual void draw_rect_aa(const rectangle &rect);
        virtual void draw_rect_aa(int x1, int y1, int x2, int y2);
        virtual void draw_circle_aa(int x, int y, int radius);
        virtual void draw_circle_aa(const rectangle &rect);
        virtual void draw_ellipse_aa(int x, int y, int a, int b);
        virtual void draw_ellipse_aa(const rectangle &rect);
        virtual void draw_poly_aa(std::vector<point> points);
        // non antialiased fill routines
        virtual void fill_rect(int x1, int y1, int x2, int y2);
        virtual void fill_rect(const rectangle& rect);
        virtual void fill_circle(int x, int y, int radius);
        virtual void fill_circle(const rectangle &rect);
        virtual void fill_ellipse(int x, int y, int a, int b);
        virtual void fill_ellipse(const rectangle &rect);
        virtual void fill_poly(std::vector<point> points);
        // antialiased fill routines
        virtual void fill_rect_aa(int x1, int y1, int x2, int y2);
        virtual void fill_rect_aa(const rectangle &rect);
        virtual void fill_circle_aa(int x, int y, int radius);
        virtual void fill_circle_aa(const rectangle &rect);
        virtual void fill_ellipse_aa(int x, int y, int a, int b);
        virtual void fill_ellipse_aa(const rectangle &rect);
        virtual void fill_poly_aa(std::vector<point> points);
    };
} //end namespace stk

#endif
