#ifndef STK_EVENT_H
#define STK_EVENT_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

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
		public:
			typedef boost::shared_ptr<event> ptr;
			typedef boost::weak_ptr<event> weak_ptr;
		private:

		protected:
			event_type type_;

		public:
			event(event_type type) : type_(type) { };
			virtual ~event() { };
			event_type type() { return type_; };
			
	}; //class event
	
} // namespace stk

#endif
