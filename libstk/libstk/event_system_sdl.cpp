/******************************************************************************
 *    FILENAME: event_system_sdl.cpp 
 * DESCRIPTION: SDL event system implementation.
 *     AUTHORS: Darren Hart, Marc Straemke
 *  START DATE: 22/Feb/2003  LAST UPDATE: 28/May/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available
 *              with the source as license.txt or at 
 *              http://www.libstk.org/index.php?page=docs/license
 *****************************************************************************/

#include <SDL/SDL.h>
#include <iostream>
#include "libstk/exceptions.h"
#include "libstk/event_system_sdl.h"
#include "libstk/key_event.h"
#include "libstk/mouse_event.h"
#include "libstk/sdl_data.h"

using std::cout;
using std::endl;

namespace stk
{
	event_system_sdl::ptr event_system_sdl::create()
	{
		event_system_sdl::ptr new_event_system_sdl(new event_system_sdl());
		return new_event_system_sdl;
	}
	
	event_system_sdl::event_system_sdl()
	{
		cout << "event_system_sdl::event_system_sdl()" << endl;
		// make sure SDL has been initialized
		sdl_data_ = sdl_data::get(); // reference counting
		sdl_data_->init();
	}

	event_system_sdl::~event_system_sdl()
	{
		cout << "event_system_sdl::~event_system_sdl()" << endl;
	}

	boost::shared_ptr<stk::event> event_system_sdl::poll_event()
	{
		//cout << "event_system_sdl::poll_event()" << endl;
		// enter the event loop
		SDL_Event new_event;
		event::ptr event_(new event(event::none));
		if (SDL_PollEvent(&new_event))
		{
			//cout << "event_system_sdl::poll_event() - event received of type: " << std::dec << (int)(new_event.type) << endl;
			switch(new_event.type)
			{
				case SDL_KEYDOWN:
					//cout << "SDL_KEYDOWN" << endl;
					return key_event::ptr(new key_event(sdl2stk_key(new_event.key.keysym.sym), event::key_down));
					break;
				case SDL_KEYUP:
					//cout << "SDL_KEYUP" << endl;
					return key_event::ptr(new key_event(sdl2stk_key(new_event.key.keysym.sym), event::key_up));
					break;
				case SDL_MOUSEBUTTONDOWN:
					//cout << "SDL_MOUSEBUTTONDOWN" << endl;
					return mouse_event::ptr(new mouse_event(new_event.button.x, new_event.button.y, 
							               new_event.button.button, event::mouse_down));
					break;
				case SDL_MOUSEBUTTONUP:
					//cout << "SDL_MOUSEBUTTONUP" << endl;
					return mouse_event::ptr(new mouse_event(new_event.button.x, new_event.button.y, 
							               new_event.button.button, event::mouse_up));
					break;
				case SDL_MOUSEMOTION:
					//cout << "SDL_MOUSEMOTION" << endl;
					return mouse_event::ptr(new mouse_event(new_event.motion.x, new_event.motion.y, 
							               -1, event::mouse_motion));
					break;
				case SDL_QUIT:
					//cout << "SDL_QUIT" << endl;
					return event::ptr(new event(event::quit));
					break;
				default:
					// throw something
					cout << "event_system_sdl::poll_event() - unknown event type" << endl;
					return event::ptr(new event(event::unknown));
			}
		}
		return event_;
	}

	keycode event_system_sdl::sdl2stk_key(SDLKey sdl_key)
	{
		keycode key;
		switch (sdl_key)
		{
			// FIXME: fill this in, take advantage of ascii codes if possible
			case SDLK_ESCAPE:
				key = key_esc;
				break;
			case SDLK_RETURN:
				key = key_enter;
				break;
			case SDLK_TAB:
				key = key_tab;
				break;
			case SDLK_LEFT:
				key = left_arrow;
				break;
			case SDLK_RIGHT:
				key = right_arrow;
				break;
			case SDLK_UP:
				key = up_arrow;
				break;
			case SDLK_DOWN:
				key = down_arrow;
				break;
			default:
				key = key_unknown;
				break;
		}
		return key;
	}

} // namespace stk
