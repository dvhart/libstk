#ifndef STK_EVENT_H
#define STK_EVENT_H

#include <boost/shared_ptr.hpp>

namespace stk
{
	enum event_type
	{
		key_down,
		key_up,
		mouse_down,
		mouse_up,
		mouse_motion,
		event_quit,
		no_event,
		unknown_event
	};
	
	class event
	{
		private:

		protected:
			event_type type_;

		public:
			event(event_type type) : type_(type) { };
			virtual ~event() { };
			event_type type() { return type_; };
			
	}; //class event

	typedef boost::shared_ptr<stk::event> Event;
	
} // namespace stk

#endif
