/******************************************************************************
 *    FILENAME: hello_world.cpp
 * DESCRIPTION: The typical hello_world application using Libstk. 
 *     AUTHORS: Darren Hart
 *  START DATE: 30/May/2003  LAST UPDATE: 30/May/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available
 *              with the source as license.txt or at 
 *              http://www.libstk.org/index.php?page=docs/license
 *****************************************************************************/

#include <iostream>

// libstk headers (FIXME: reduce this list, i.e. stk.h)
#include "libstk/application.h"
#include "libstk/exceptions.h"
#include "libstk/label.h"
#include "libstk/state.h"

// backend specific headers
#include "libstk/event_system_sdl.h"
#include "libstk/surface_sdl.h"

using namespace stk;

int main(int argc, char* argv[])
{
	int retval = 0;

	try
	{
		// create the surface, event system, and application
		// FIXME: simplify application construction
		surface::ptr test_surface = surface_sdl::create(rectangle(0, 0, 640, 480));
		event_system::ptr test_event_system = event_system_sdl::create();
		application::ptr test_app = application::create(test_surface, test_event_system);
	
		// create the main state and a label
		state::ptr test_state = state::create(test_app);
		label::ptr test_label = label::create(test_state, std::wstring(L"Hello World"), 
			rectangle(260, 210, 100, 30));
		
		// run the program
		retval = test_app->run();
	}
	catch (const exception& e)
	{
		std::cout << "Exception: " << e.what() << std::endl;
	}
	
	return retval;
}
