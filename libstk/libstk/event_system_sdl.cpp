#include <SDL/SDL.h>
#include <iostream>
#include "event_system_sdl.h"
#include "key_event.h"
#include "mouse_event.h"

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
	}

	event_system_sdl::~event_system_sdl()
	{
	}

	boost::shared_ptr<stk::event> event_system_sdl::poll_event()
	{
		//cout << "event_system_sdl::poll_event()" << endl;
		// enter the event loop
		SDL_Event new_event;
		event::ptr event_(new event(no_event));
		if (SDL_PollEvent(&new_event))
		{
			//cout << "event_system_sdl::poll_event() - event received of type: " << std::dec << (int)(new_event.type) << endl;
			switch(new_event.type)
			{
				case SDL_KEYDOWN:
					//cout << "SDL_KEYDOWN" << endl;
					return key_event::ptr(new key_event(sdl2stk_key(new_event.key.keysym.sym), key_down));
					break;
				case SDL_KEYUP:
					//cout << "SDL_KEYUP" << endl;
					return key_event::ptr(new key_event(sdl2stk_key(new_event.key.keysym.sym), key_up));
					break;
				case SDL_MOUSEBUTTONDOWN:
					//cout << "SDL_MOUSEBUTTONDOWN" << endl;
					return mouse_event::ptr(new mouse_event(new_event.button.x, new_event.button.y, 
							               new_event.button.button, mouse_down));
					break;
				case SDL_MOUSEBUTTONUP:
					//cout << "SDL_MOUSEBUTTONUP" << endl;
					return mouse_event::ptr(new mouse_event(new_event.button.x, new_event.button.y, 
							               new_event.button.button, mouse_up));
					break;
				case SDL_MOUSEMOTION:
					//cout << "SDL_MOUSEMOTION" << endl;
					return mouse_event::ptr(new mouse_event(new_event.motion.x, new_event.motion.y, 
							               -1, mouse_motion));
					break;
				case SDL_QUIT:
					//cout << "SDL_QUIT" << endl;
					return event::ptr(new event(event_quit));
					break;
				default:
					// throw something
					cout << "event_system_sdl::poll_event() - unknown event type" << endl;
					return event::ptr(new event(unknown_event));
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
