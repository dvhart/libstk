#ifndef STK_EVENT_SYSTEM_H
#define STK_EVENT_SYSTEM_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "event.h"

namespace stk
{
	class event_system
	{
		public:
			typedef boost::shared_ptr<event_system> ptr;
			typedef boost::weak_ptr<event_system> weak_ptr;
		private:
		
		protected:
			
		public:
			event_system() { };
			virtual ~event_system() { };
			virtual event::ptr poll_event() = 0;
	};

}

#endif
