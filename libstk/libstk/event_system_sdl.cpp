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
	}

	event_system_sdl::~event_system_sdl()
	{
	}

	Event event_system_sdl::poll_event()
	{
		// enter the event loop
		SDL_Event new_event;
		Event event_;
		if (SDL_PollEvent(&new_event))
		{
			switch(new_event.type)
			{
				case SDL_KEYDOWN:
					event_.reset(new key_event());
					break;
				case SDL_KEYUP:
					event_.reset(new key_event());
					break;
				case SDL_MOUSEBUTTONDOWN:
					event_.reset(new mouse_event());
					break;
				case SDL_MOUSEBUTTONUP:
					event_.reset(new mouse_event());
					break;
				case SDL_MOUSEMOTION:
					event_.reset(new mouse_event());
					break;
				case SDL_QUIT:
					break;
				default:
					// throw something
					cout << "event_system_sdl::poll_event - unknown event type" << endl;
			}
		}
		return event_;
	}

} // namespace stk
