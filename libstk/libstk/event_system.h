#ifndef STK_EVENT_SYSTEM_H
#define STK_EVENT_SYSTEM_H

#include "event.h"

namespace stk
{
	class event_system
	{
		private:
		
		protected:
			
		public:
			event_system();
			virtual ~event_system();
			virtual Event poll_event() = 0;
	};
}

#endif
