/***************************************************************************
			  app.h  -  stk::app header
			     -------------------
    begin		: Sat Apr 27 2002
    copyright	    : (C) 2002 by Darren Hart
    email		: dvhart@byu.edu
***************************************************************************/

/***************************************************************************
 *									 *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.				   *
 *									 *
 ***************************************************************************/

#ifndef _STK_APP_H_
#define _STK_APP_H_

#include <SDL.h>
#include <map>

#include "surface.h"
#include "widget.h"

namespace stk
{

class app
{
private:
    stk::widget *focused_widget_;

public:
    app();
    ~app();

    void init(Uint32 sdl_flags); /* inits our toolkit (and the SDL) */

    /* init our video output */
    void init_video(int width, int height, int bpp,
                    Uint32 sdl_flags);
    void init_video(SDL_Surface *screen);

    /* the main-loop */
    void run();
};
} // namespace stk

#endif
