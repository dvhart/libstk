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

#include <boost/shared_ptr.hpp>
#include <SDL/SDL.h>

namespace stk {
	class surface;
	typedef boost::shared_ptr<surface> surface_ptr;
	
	class surface {
	public:
		surface();
		~surface();
		
		int draw_pixel();
		int draw_line();
		int draw_rect();
		int draw_arc();
		int draw_ellipse();
		int fill_rect();
		int fill_ellipse();
	private:
		SDL_Surface *_surface;
	};
}

#endif
