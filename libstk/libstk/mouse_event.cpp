/******************************************************************************
 *    FILENAME:  
 * DESCRIPTION: 
 *     AUTHORS: First Last, First Last, etc.
 *  START DATE: DD/MMM/YYYY  LAST UPDATE: DD/MMM/YYY
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available
 *              with the source as license.txt or at 
 *              http://www.libstk.org/index.php?page=docs/license
 *****************************************************************************/

#include <iostream>
#include "libstk/event.h"
#include "libstk/mouse_event.h"

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
