/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: The typical hello_world application using Libstk. 
 *     AUTHORS: Darren Hart
 *  START DATE: 2003/May/30
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include <iostream>
#include <libstk/stk.h>

// backend specific includes (we select it dynamically, so include them all if built)
#ifdef HAVE_SDL
#include <libstk/event_producer_sdl.h>
#include <libstk/surface_sdl.h>
#endif
#ifdef HAVE_DIRECTFB
#include <libstk/surface_dfb.h>
// what would be a good default backend ?
#endif
#ifdef HAVE_FBDEV
#include <libstk/surface_fbdev.h>
// what would be a good default backend ?
#endif
// FIXME: should this be configurable also ?  --enable-elotouch ?
#include <libstk/event_producer_elotouch.h>

using namespace stk;

int main(int argc, char* argv[])
{
    int retval = 0;

    try
    {
#ifdef HAVE_LOGGING
        logger::get()->add_target(&std::cout, LL_Info);
#endif

        std::string surface_type;
        std::string available_surfaces = "[ ";
#ifdef HAVE_SDL
        available_surfaces += "sdl |";
#endif
#ifdef HAVE_DIRECTFB
        available_surfaces += " dfb |";
#endif
#ifdef HAVE_FBDEV
        available_surfaces += " fbdev";
#endif
        available_surfaces += " ]";

        if (argc < 2)
        {
            throw error_message_exception("Usage: hello_world "+available_surfaces);
        }
        else
        {
            surface_type = std::string(argv[1]);
        }
        
        // select the surface and event system
        INFO("selecting surface and event system");
        surface::ptr screen;
        event_producer::ptr ep;
        if (0) { }
#ifdef HAVE_SDL
        else if (surface_type == "sdl")
        {
            screen = surface_sdl::create(rectangle(0, 0, 640, 480));
            ep = event_producer_sdl::create();
        }
#endif
#ifdef HAVE_DIRECTFB
        else if (surface_type == "dfb")
        {
            screen = surface_dfb::create(rectangle(0, 0, 640, 480));
            // FIXME: use something a bit more generic for the test app
            ep = event_producer_elotouch::create("/dev/tts/0");
        }
#endif
#ifdef HAVE_FBDEV
        else if (surface_type == "fbdev")
        {
            screen = surface_fbdev::create(rectangle(0, 0, 1024, 768));
#ifdef HAVE_SDL // needed for the event_system
            ep = event_producer_sdl::create();
#endif
        }
#endif
        else
            throw error_message_exception("Unknown surface type");

        
        // create the application
        application::ptr test_app = application::create(screen);

        // create the main state and a label
        state::ptr test_state = state::create(test_app);
        label::ptr test_label = label::create(test_state, std::wstring(L"Hello World"), 
                rectangle(260, 210, 100, 30));

        // add a timer (quit after 15 seconds)
        INFO("hello_world - creating timer to quit after 15 seconds");
        timer::ptr quit_timer = timer::create(15000, false);
        quit_timer->on_timer.connect( boost::bind(&stk::application::quit, test_app.get()));
        test_app->add_timer(quit_timer);

        // run the program
        retval = test_app->run();
    }
    catch (const exception& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    catch (...)
    {
        std::cout << "Unkown exception" << std::endl;
    }

    return retval;
}
