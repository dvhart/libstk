#ifndef STK_QUIT_EVENT_H
#define STK_QUIT_EVENT_H

#include <boost/shared_ptr.hpp>
#include "event.h"

namespace stk
{
	class quit_event : public event
	{
		private:

		protected:

		public:
			quit_event(event_type type=event_quit);
			virtual ~quit_event();
			
	}; //class quit_event

	typedef boost::shared_ptr<stk::quit_event> QuitEvent;
	
} // namespace stk

#endif
