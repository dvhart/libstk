/***************************************************************************
                          stk_widget.h  -  description
                             -------------------
    begin                : Sat Apr 27 2002
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
#include <boost/shared_ptr.hpp>

namespace stk {
	class surface;
	typedef boost::shared_ptr<surface> surface_ptr;
	
	class surface {
	private:
		SDL_Surface *_surface;

	public:
		surface();
		~surface();
		
		// non antialiased draw routines
		int draw_pixel();
		int draw_line();
		int draw_rect();
		int draw_arc();
		int draw_ellipse();
		int draw_poly();

		// antialiased draw routines
		int draw_pixel_aa();
		int draw_line_aa();
		int draw_rect_aa();
		int draw_arc_aa();
		int draw_ellipse_aa();
		int draw_poly_aa();

		// non antialiased fill routines
		int fill_rect();
		int fill_ellipse();

		// antialiased fill routines
		int fill_rect_aa();
		int fill_ellipse_aa();

		// util methods
		void lock();
		void unlock();
		void blit(surface &p_surface, SDL_Rect *rect = NULL);
	};
}

#endif
