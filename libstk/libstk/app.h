/***************************************************************************
                          stk_app.h  -  description
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

#ifndef STK_APP_H
#define STK_APP_H

#include <SDL/SDL.h>
#include <map>

#include "stk_state.h"

namespace stk {

	class app {
	public:
		app();
		~app();

		int init(Uint32 sdl_flags);
		int init_video(int width, int height, int bpp, 
			       Uint32 sdl_flags);
		int run();

		int set_state(const state &val);
		int set_state(const std::string &name);
		inline state_ptr get_state() { return _state_iter.data(); };
		int add_state(const state &val);

		int load_theme(const std::string &name);
		
		widget_ptr lookup_widget(std::string &name);

		void set_focused(widget &val);
		void set_focused(const std::string &name);
		inline widget_ptr get_focused() { return _focused_widget; };

		inline void register_key(SDLKey k, keyslot acall) { 
			keybindings[k] = acall;
		};

	private:
		bool good_event(SDL_Event *event);

		SDL_Surface *_screen;
		theme_ptr _theme;
		
		map<std::string, state_ptr> _states;
		map<SDLKey, keyslot> _keybindings;

		state_map::iterator _state_iter;
		widget_ptr _focused_widget;
	};
} // namespace stk

#endif
