#include <iostream>

#include "event.h"
#include "quit_event.h"

using std::cout;
using std::endl;

namespace stk
{
	quit_event::quit_event(event_type type) : 
		event(type)
	{
		cout << "quit_event::quit_event()" << endl;
	}

	quit_event::~quit_event()
	{
	}
	
} // namespace stk
