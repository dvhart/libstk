#ifndef STK_MOUSE_EVENT_H
#define STK_MOUSE_EVENT_H

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
} // namespace stk

#endif
