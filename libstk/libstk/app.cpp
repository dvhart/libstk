/***************************************************************************
			  app.cpp  -  stk::app implementation
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <string>
#include <boost/lexical_cast.hpp>
#include "exceptions.h"

#include "app.h"
namespace stk
{

app::app()
{}

app::~app()
{}

void app::init(Uint32 sdl_flags)
{
    /* set some defaults */
    if (!sdl_flags)
        sdl_flags = SDL_INIT_VIDEO;

    if (SDL_Init(sdl_flags | SDL_INIT_TIMER) < 0) /* we always pass SDL_INIT_TIMER since exec needs it */
        throw stk::error_message_exception(std::string("app::init - Unable to init SDL: ")+boost::lexical_cast<std::string>(SDL_GetError()));

    /* SDL cleanup on exit (ESC or window close) */
    atexit(SDL_Quit);
}

void app::init_video(int width, int height, int bpp, Uint32 sdl_flags)
{
}

void app::run()
{
}

}
