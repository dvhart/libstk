/******************************************************************************
 *    FILENAME: surface_sdl.h
 * DESCRIPTION: SDL surface backend.
 *     AUTHORS: Darren Hart, Vernon Mauery, Marc Straemke 
 *  START DATE: 10/Dec/2002  LAST UPDATE: 28/May/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available
 *              with the source as license.txt or at 
 *              http://www.libstk.org/index.php?page=docs/license
 *****************************************************************************/

#ifndef STK_SURFACE_SDL_H
#define STK_SURFACE_SDL_H

#include <string>
#include <vector>
#include <SDL/SDL.h>
#include <libstk/surface.h>
#include <libstk/surface_impl.h>
#include <libstk/point.h>
#include <libstk/edge.h>
#include <libstk/stk.h>
#include <libstk/sdl_data.h>

// this is opposite from the docs, but it works
#if SDL_BYTEORDER != SDL_BIG_ENDIAN
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

#define MIN(A,B) (((A)<(B))?(A):(B))

namespace stk
{

	class surface_sdl : public stk::surface_impl<surface_sdl>
	{
		public:
			typedef boost::shared_ptr<surface_sdl> ptr;
			typedef boost::weak_ptr<surface_sdl> weak_ptr;
			
		private:
			SDL_Surface *sdl_surface_;
			sdl_data::ptr sdl_data_;

			SDL_Rect rect_to_sdl_rect(const rectangle &rect)
			{
				SDL_Rect sdl_rect = { rect.x1(), rect.y1(), rect.width(), rect.height() };
				return sdl_rect;
			}

		protected:
			surface_sdl(const rectangle& rect);
			
		public:
			static surface_sdl::ptr create(const rectangle& rect);
			~surface_sdl();

			SDL_Surface *sdl_surface() const { return sdl_surface_; };

			// methods which MUST be implemented in derived classes
			inline void put_pixel(int x, int y, color clr);
			inline void put_pixel_aa(int x, int y, double distance, color clr);
			inline void put_pixel_aa(int x, int y, unsigned char alpha_a, color clr);
			inline color get_pixel(int x, int y) const;
			virtual color gen_color(const std::string &str_color) const;
			virtual color gen_color(byte r, byte g, byte b, byte a) const;
			virtual void lock(rectangle &rect, int flags, color** buf, int &stride);
			virtual void unlock();
			virtual void update(const rectangle& u_rect=rectangle());
			
			// overridden drawing routines
			virtual void blit(surface &dst_surface);
			virtual void fill_rect(int x1, int y1, int x2, int y2);
			virtual void fill_rect(const rectangle& rect);
	};
} //end namespace stk

#endif
