/***************************************************************************
	surface.cpp  -  surface class implementation
	-------------------
begin                : Tue September 10 2002
copyright            : (C) 2002 by Darren Hart
email                : dvhart@byu.edu
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef STK_SURFACE_SDL_H
#define STK_SURFACE_SDL_H

#include "surface.h"
#include "point.h"
#include "edge.h"
#include <string>
#include <vector>
#include <SDL/SDL.h>

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

	class surface_sdl : public stk::surface
	{
		private:
			SDL_Surface *sdl_surface_;

			// optimized pixel routines (private and not virtual)
			inline void put_pixel(int x, int y, color clr);
			inline void put_pixel_aa(int x, int y, double distance, color clr);
			inline color pixel_at(int x, int y);
			SDL_Rect rect_to_sdl_rect(const rectangle &rect)
			{
				SDL_Rect sdl_rect = { rect.x1(), rect.y1(), rect.w(), rect.h() };
				return sdl_rect;
			}
		public:
			surface_sdl(SDL_Surface &new_surface);
			surface_sdl(rectangle &rect);
			~surface_sdl();

			SDL_Surface *sdl_surface() const { return sdl_surface_; };

			// methods which MUST be implemented in derived classes
			virtual void draw_pixel(int x, int y, color clr);
			virtual void draw_pixel_aa(int x, int y, double distance, color clr);
			virtual color get_pixel(int x, int y);
			// format: "0xRRGGBBAA", 0-255, alpha 255 being opaque
			virtual color gen_color(const std::string &str_color);
			virtual color gen_color(byte r, byte g, byte b, byte a);
			virtual void lock(rectangle &rect, int flags, color** buf, int &stride);
			virtual void unlock();
			virtual void blit(surface &dst_surface);
			virtual void flip();
			
			// optimized drawing routines
			// WRITEME...

			// optimied aa drawing routines
			// WRITEME...

			// optimized fill routines
			// WRITEME...
			virtual void fill_rect(int x1, int y1, int x2, int y2);
			virtual void fill_rect(const rectangle &rect);

			// optimized aa fill routines
			// WRITEME...
	};
} //end namespace stk

#endif
