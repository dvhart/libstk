#ifndef STK_EVENT_H
#define STK_EVENT_H

namespace stk
{
	enum event_type
	{
		key_down,
		key_up,
		mouse_down,
		mouse_up,
		mouse_motion
	}
	
	class event
	{
		private:
			event_type type_;

		protected:

		public:
			event();
			virtual ~event();
			event_type type();
			
	} //class event
} // namespace stk

#endif
