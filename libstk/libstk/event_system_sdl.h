#ifndef STK_EVENT_SYSTEM_SDL_H
#define STK_EVENT_SYSTEM_SDL_H

#include <SDL/SDL.h>
#include "libstk/keycode.h"
#include "libstk/event_system.h"

/* possible event calls in SDL
	 currently we only use SDL_PollEvent, which calls SDL_PumpEvents
	 SDL_PumpEvents - Pumps the event loop, gathering events from the input devices.
	 SDL_PeepEvents - Checks the event queue for messages and optionally returns them.
	 SDL_PollEvent - Polls for currently pending events.
	 SDL_WaitEvent - Waits indefinitely for the next available event.
	 SDL_PushEvent - Pushes an event onto the event queue
	 SDL_SetEventFilter - Sets up a filter to process all events before they are posted to the event queue.
	 SDL_GetEventFilter - Retrieves a pointer to he event filter
	 SDL_EventState - This function allows you to set the state of processing certain events.
	 SDL_GetKeyState - Get a snapshot of the current keyboard state
	 SDL_GetModState - Get the state of modifier keys.
	 SDL_SetModState - Set the current key modifier state
	 SDL_GetKeyName - Get the name of an SDL virtual keysym
	 SDL_EnableUNICODE - Enable UNICODE translation
	 SDL_EnableKeyRepeat - Set keyboard repeat rate.
	 SDL_GetMouseState - Retrieve the current state of the mouse
	 SDL_GetRelativeMouseState - Retrieve the current state of the mouse
	 SDL_GetAppState - Get the state of the application
	 SDL_JoystickEventState - Enable/disable joystick event polling
	 */
namespace stk
{
	class event_system_sdl : public event_system
	{
		private:
			keycode sdl2stk_key(SDLKey sdl_key);

		protected:
			event_system_sdl();
			
		public:
			static event_system_sdl::ptr create();
			virtual ~event_system_sdl();
			virtual boost::shared_ptr<stk::event> poll_event();
	};
}

#endif
