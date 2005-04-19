/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: An simple STK frontend to xine, demonstrating the overlay mechanism
 *     AUTHORS: Darren Hart
 *  START DATE: 2003/Jul/20
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include <iostream>
#include <SDL/SDL.h> // temporary: to get sdl overlay formats, remove once we define our own
#include <libstk/stk.h>

// backend specific includes (we select it dynamically, so include them all if built)
#ifdef HAVE_SDL
#include <libstk/event_producer_sdl.h>
#include <libstk/surface_sdl.h>
#endif
#ifdef HAVE_DFB
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
        std::string surface_type, filename;
        if (argc < 3)
        {
            throw error_message_exception("Usage: xine_test sdl|dfb|fbdev filename");
        }
        else
        {
            surface_type = std::string(argv[1]);
            filename = std::string(argv[2]);
        }
        
        // select the surface and event system
        INFO("selecting surface and event system");
        surface::ptr screen;
        application::ptr app;
        event_producer::ptr ep;
        if (0) {}
#ifdef HAVE_SDL
        else if (surface_type == "sdl")
        {
            screen = surface_sdl::create(rectangle(0, 0, 640, 480), true);
            app = application::create(screen);
            ep = event_producer_sdl::create();
        }
#endif
#ifdef HAVE_DFB
        else if (surface_type == "dfb")
        {
            screen = surface_dfb::create(rectangle(0, 0, 640, 480));
            app = application::create(screen);
            // FIXME: use something a bit more generic for the test app
            ep = event_producer_elotouch::create("/dev/tts/0");
        }
#endif
#ifdef HAVE_FBDEV
#ifdef HAVE_SDL // needed for the event_system
        else if (surface_type == "fbdev")
        {
            screen = surface_fbdev::create(rectangle(0, 0, 1024, 768));
            app = application::create(screen);
            ep = event_producer_sdl::create();
        }
#endif
#endif
        else
            throw error_message_exception("Unknown surface type");

        // create the main state
        state::ptr main_state = state::create(app);
        
        // create the xine panel
	INFO("creating the xine panel");
        xine_panel::ptr xp = xine_panel::create(main_state, rectangle(50, 50, 540, 380));
        xp->open(filename);

	INFO("creating the buttons");
        // buttons
        button::ptr prev_button = button::create(main_state, std::wstring(L"|<"), 
                rectangle(50, 440, 81, 30));
        prev_button->on_release.connect(boost::function<bool()>((boost::bind(&xine_panel::play, xp.get(), 0, 0), true) ));
        
        button::ptr play_button = button::create(main_state, std::wstring(L"Play"), 
                rectangle(141, 440, 81, 30));
        play_button->on_release.connect(boost::function<bool()>((boost::bind(&xine_panel::play, xp.get(), 0, 0), true)));
        
        button::ptr pause_button = button::create(main_state, std::wstring(L"Pause"), 
                rectangle(232, 440, 81, 30));
        pause_button->on_release.connect(boost::function<bool()>((boost::bind(&xine_panel::playpause, xp.get()), true)));
        
        button::ptr slower_button = button::create(main_state, std::wstring(L"Slower"), 
                rectangle(323, 440, 81, 30));
        slower_button->on_release.connect(boost::function<bool()>((boost::bind(&xine_panel::slower, xp.get()), true)));
        
        button::ptr faster_button = button::create(main_state, std::wstring(L"Faster"), 
                rectangle(414, 440, 81, 30));
        faster_button->on_release.connect(boost::function<bool()>((boost::bind(&xine_panel::faster, xp.get()), true)));
        
        button::ptr quit_button = button::create(main_state, std::wstring(L"Quit"), 
                rectangle(505, 440, 81, 30));
        quit_button->on_release.connect(boost::function<bool()>((boost::bind(&application::quit, app.get()), true)));


        // add a timer (quit after 30 seconds)
        /*
        INFO("xine_test - creating timer to quit after 30 seconds");
        timer::ptr quit_timer = timer::create(30000, false);
        quit_timer->on_timer.connect(boost::bind(&application::quit, app.get()));
        app->add_timer(quit_timer);
        */

        INFO("running the libstk app");
        // run the program
        retval = app->run();

        INFO("shared pointer automatic destuction follows:");
    }
    catch (const exception& e)
    {
        ERROR("Exception: " << e.what());
    }

    return retval;
}
