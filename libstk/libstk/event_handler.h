#ifndef STK_EVENT_HANDLER_H
#define STK_EVENT_HANDLER_H

#include "event.h"

namespace stk
{
	/// An abstract class declaring the event_handler interface.
	class event_handler
	{
		private:
			
		protected:
			
		public:
			event_handler() { };
			virtual ~event_handler() { };

			/// Receive an event and do something with it.
			/// \param e Event to be handled
			virtual void handle_event(event::ptr e) = 0;
			
	}; // class event_handler
} // namespace stk

#endif

