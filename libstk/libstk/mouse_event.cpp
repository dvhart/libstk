#include <iostream>
#include "event.h"
#include "mouse_event.h"

using std::cout;
using std::endl;

namespace stk
{
	mouse_event::mouse_event(int x, int y, int button, event_type type) : 
		event(type), x_(x), y_(y), button_(button) 
	{
		/*
		cout << "mouse_event::mouse_event()" << endl;
		cout << "\ttype:   " << type << endl;
		cout << "\tx:      " << x_ << endl;
		cout << "\ty:      " << y_ << endl;
		cout << "\tbutton: " << button << endl;
		*/
	}

	mouse_event::~mouse_event()
	{
	}
	
} // namespace stk
