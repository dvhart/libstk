#ifndef STK_MOUSE_EVENT_H
#define STK_MOUSE_EVENT_H

#include <boost/shared_ptr.hpp>
#include "event.h"

namespace stk
{
	class mouse_event : public event
	{
		private:

		protected:
			int x_, y_, button_;

		public:
			mouse_event(int x=-1, int y=-1, int button=-1, event_type type=mouse_up);
			virtual ~mouse_event();
			int x() { return x_; }
			int y() { return y_; }
			int button() { return button_; }
			
	}; //class mouse_event

	typedef boost::shared_ptr<stk::mouse_event> MouseEvent;
	
} // namespace stk

#endif
