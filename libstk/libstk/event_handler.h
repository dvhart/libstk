#ifndef STK_EVENT_HANDLER_H
#define STK_EVENT_HANDLER_H

#include "event.h"

namespace stk
{
	class event_handler
	{
		private:
			
		protected:
			
		public:
			event_handler() { };
			virtual ~event_handler() { };
			virtual void handle_event(boost::shared_ptr<stk::event> e) = 0;
			
	}; // class event_handler
} // namespace stk

#endif

