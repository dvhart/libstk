#include <SDL/SDL.h>
#include <iostream>
#include "event_system_sdl.h"
#include "key_event.h"
#include "mouse_event.h"

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

	event event_system_sdl::poll_event()
	{
		cout << "event_system_sdl::poll_event()" << endl;
		// enter the event loop
		SDL_Event new_event;
		event event_(no_event);
		if (SDL_PollEvent(&new_event))
		{
			cout << "event_system_sdl::poll_event() - event received of type: " << (int)(new_event.type) << endl;
			switch(new_event.type)
			{
				case SDL_KEYDOWN:
					cout << "SDL_KEYDOWN" << endl;
					return key_event();
					break;
				case SDL_KEYUP:
					cout << "SDL_KEYUP" << endl;
					return key_event();
					break;
				case SDL_MOUSEBUTTONDOWN:
					cout << "SDL_MOUSEBUTTONDOWN" << endl;
					return mouse_event();
					break;
				case SDL_MOUSEBUTTONUP:
					cout << "SDL_MOUSEBUTTONUP" << endl;
					return mouse_event();
					break;
				case SDL_MOUSEMOTION:
					cout << "SDL_MOUSEMOTION" << endl;
					return mouse_event();
					break;
				case SDL_QUIT:
					cout << "SDL_QUIT" << endl;
					break;
				default:
					// throw something
					cout << "event_system_sdl::poll_event() - unknown event type" << endl;
					return event(unknown_event);
			}
		}
		return event_;
	}

} // namespace stk
