#include "event.h"
#include "key_event.h"

namespace stk
{
	key_event::key_event(keycode key, event_type type) : 
		event(type), keycode_(key)
	{
	}

	key_event::~key_event()
	{
	}
	
} // namespace stk
