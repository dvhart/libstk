#ifndef STK_EVENT_SYSTEM_H
#define STK_EVENT_SYSTEM_H

#include <boost/shared_ptr.hpp>
#include "event.h"

namespace stk
{
	class event_system
	{
		private:
		
		protected:
			
		public:
			event_system() { };
			virtual ~event_system() { };
			virtual boost::shared_ptr<stk::event> poll_event() = 0;
	};

	typedef boost::shared_ptr<stk::event_system> EventSystem;

}

#endif
