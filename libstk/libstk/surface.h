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


#ifndef STK_SURFACE_H
#define STK_SURFACE_H

#include <SDL/SDL.h>
#include <string>

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

namespace stk {
	class surface;
	
	class surface {
	private:
		SDL_Surface *_surface;
		
	public:
		surface(SDL_Surface &new_surface);
		surface(SDL_Rect &rect); 
		~surface();
		
		// non antialiased draw routines
		void draw_pixel(int x, int y, Uint32 color);
		void draw_line(int x1, int y1, int x2, int y2, Uint32 color, /*style,width*/);
		void draw_rect();
		void draw_arc();
		void draw_ellipse();
		void draw_poly();
		
		// antialiased draw routines
		void draw_pixel_aa();
		void draw_line_aa();
		void draw_rect_aa();
		void draw_arc_aa();
		void draw_ellipse_aa();
		void draw_poly_aa();
		
		// non antialiased fill routines
		void fill_rect();
		void fill_ellipse();
		
		// antialiased fill routines
		void fill_rect_aa();
		void fill_ellipse_aa();
		
		// util methods
		Uint32 gen_color(const std::string &str_color);
		Uint32 gen_color(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
		void lock();
		void unlock();
		void blit(surface &p_surface, SDL_Rect *rect = NULL);
		SDL_Surface *get_surface() { return _surface; }
	};
} // namespace stk
#endif
