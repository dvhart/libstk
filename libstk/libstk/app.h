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

#include "state.h"

namespace stk {

	class app {
	private:
		SDL_Surface *_screen; /* pointer to the screen we are drawing to */
		stk::theme  *_theme;  /* our theme we use to draw the widgets */
		stk::state  *_state;  /* our current state */

		map<std::string, stk::state *> _states; /* the place where all states are stored */
		map<SDLKey, stk::keyslot> _keybindings; /* the place where all our keybinds are stored */

		state_map::iterator _state_iter;
		stk::widget *_focused_widget;

		bool good_event(SDL_Event *event);

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

		/* sets */
		void set_state(const state &val);	  /* sets "state" as current state */
		void set_state(const std::string &name);     /* looks up a state by its "name" and sets it as the current */
		void set_theme(const std::string &name);     /* sets the theme by its "name" */
		void set_focused(const std::widget &val); /* focuses the "widget" */
		void set_focused(const std::string &name);   /* looks up a widget by its "name" and gives it the focus */
		/* ---- */

		/* gets */
		stk::state  *get_state()   { return _state_iter.data(); } /* returns the current state */
		stk::theme  *get_theme()   { return _theme }	      /* returns the current theme */
		stk::widget *get_focused() { return _focused_widget; }    /* returns the currently focused widget */
		stk::widget *get_widget(const std::string &name);	 /* looks up a widget by its "name" and returns it */
		/* ---- */

		void add_state(const state &val);
		void register_key(SDLKey key, stk::keyslot callback) { _keybindings[key] = callback; };
	};
} // namespace stk

#endif
