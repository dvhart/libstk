#include "event.h"
#include "mouse_event.h"

namespace stk
{
	mouse_event::mouse_event(int x, int y, int button, event_type type) : 
		event(type), x_(x), y_(y), button_(button) 
	{
	}

	mouse_event::~mouse_event()
	{
	}
	
} // namespace stk
