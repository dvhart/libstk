#ifndef STK_EVENT_SYSTEM_SDL_H
#define STK_EVENT_SYSTEM_SDL_H

namespace stk
{
	class event_system_sdl : public event_system
	{
		private:
		
		protected:
			
		public:
			event_system_sdl();
			virtual ~event_system_sdl();
			virtual Event poll_event();
	};
}

#endif
