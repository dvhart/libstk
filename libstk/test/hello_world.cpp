/******************************************************************************
 *    FILENAME: hello_world.cpp
 * DESCRIPTION: The typical hello_world application using Libstk. 
 *     AUTHORS: Darren Hart
 *  START DATE: 30/May/2003  LAST UPDATE: 09/Jun/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available
 *              with the source as license.txt or at 
 *              http://www.libstk.org/index.php?page=docs/license
 *****************************************************************************/

#include <iostream>
#include <libstk/stk.h>

// backend specific headers
#include <libstk/event_producer_sdl.h>
#include <libstk/surface_sdl.h>

using namespace stk;

int main(int argc, char* argv[])
{
    int retval = 0;

    try
    {
        // create the event producer
        // it is automatically registered with the primary event_system
        event_producer_sdl::ptr test_event_producer = event_producer_sdl::create();
        // create the primary surface
        surface::ptr test_surface = surface_sdl::create(rectangle(0, 0, 640, 480));
        // create the application
        application::ptr test_app = application::create(test_surface);

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
