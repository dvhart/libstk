#ifndef STK_KEY_EVENT_H
#define STK_KEY_EVENT_H

#include <boost/shared_ptr.hpp>
#include "event.h"
#include "keycode.h"

namespace stk
{
	class key_event : public event
	{
		private:

		protected:
			keycode keycode_;

		public:
			key_event(keycode key=key_null, event_type type=key_up);
			virtual ~key_event();
			keycode key() { return keycode_; }
			
	}; //class key_event

	typedef boost::shared_ptr<stk::key_event> KeyEvent;
	
} // namespace stk

#endif
