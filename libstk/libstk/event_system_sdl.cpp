#include <SDL/SDL.h>
#include <iostream>
#include "event_system_sdl.h"
#include "key_event.h"
#include "mouse_event.h"
#include "quit_event.h"

using std::cout;
using std::endl;

namespace stk
{
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
		Event event_(new event(no_event));
		if (SDL_PollEvent(&new_event))
		{
			cout << "event_system_sdl::poll_event() - event received of type: " << std::dec << (int)(new_event.type) << endl;
			switch(new_event.type)
			{
				case SDL_KEYDOWN:
					cout << "SDL_KEYDOWN" << endl;
					return KeyEvent(new key_event(sdl2stk_key(new_event.key.keysym.sym), key_up));
					break;
				case SDL_KEYUP:
					cout << "SDL_KEYUP" << endl;
					return KeyEvent(new key_event(sdl2stk_key(new_event.key.keysym.sym), key_down));
					break;
				case SDL_MOUSEBUTTONDOWN:
					cout << "SDL_MOUSEBUTTONDOWN" << endl;
					return MouseEvent(new mouse_event(new_event.button.x, new_event.button.y, 
							               new_event.button.button, mouse_down));
					break;
				case SDL_MOUSEBUTTONUP:
					cout << "SDL_MOUSEBUTTONUP" << endl;
					return MouseEvent(new mouse_event(new_event.button.x, new_event.button.y, 
							               new_event.button.button, mouse_up));
					break;
				case SDL_MOUSEMOTION:
					cout << "SDL_MOUSEMOTION" << endl;
					return MouseEvent(new mouse_event(new_event.motion.x, new_event.motion.y, 
							               -1, mouse_motion));
					break;
				case SDL_QUIT:
					cout << "SDL_QUIT" << endl;
					// FIXME: perhaps we don't need a QuitEvent, we can just use Event(new event(quit)); ??
					return QuitEvent(new quit_event());
					break;
				default:
					// throw something
					cout << "event_system_sdl::poll_event() - unknown event type" << endl;
					return Event(new event(unknown_event));
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
			default:
				key = key_unknown;
				break;
		}
		return key;
	}

} // namespace stk
