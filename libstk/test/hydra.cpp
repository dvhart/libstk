/**************************************************************************************************
 *    FILENAME: hydra.cpp
 * DESCRIPTION: A simple concept app of the Hydra multi-client distributed home media network 
 *     AUTHORS: Darren Hart
 *  START DATE: 13/Jul/2003  LAST UPDATE: 14/Jul/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

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
#ifdef HAVE_LOGGING
        logger::get()->add_target(&cout, LL_Info);
#endif
        // create the event producer
        // it is automatically registered with the primary event_system
        event_producer_sdl::ptr ep = event_producer_sdl::create();
        // create the primary surface
        surface::ptr screen = surface_sdl::create(rectangle(0, 0, 640, 480));
        // create the application
        application::ptr app = application::create(screen);

        // create the states
        state::ptr main_state = state::create(app);
        state::ptr config_state = state::create(app);

        // set up the main state
        label::ptr main_label = label::create(main_state, L"MAIN", rectangle(10, 10, 100, 30));
        // several buttons
        // go to the config state
        button::ptr config_button = button::create(main_state, L"Config",
                rectangle(120, 10, 100, 30));
        config_button->on_release.connect( boost::bind(&application::current_state, 
                    app.get(), config_state) );
        // quit the application
        button::ptr main_quit_button = button::create(main_state, L"Quit (ESC)",
                rectangle(530, 440, 100, 30));
        main_quit_button->on_release.connect(boost::bind(&application::quit, app.get()));

        // set up the config state
        label::ptr config_label = label::create(config_state, L"CONFIG", 
                rectangle(10, 10, 100, 30));
        // several spinners
        // Keyboard spinner
        label::ptr kb_label = label::create(config_state, L"Keyboard: ", 
                rectangle(400, 90, 100, 30));
        spinner::ptr kb_spinner = spinner::create(config_state, 
                rectangle(500, 90, 125, 30), false);
        list_item::ptr kb_item_phy = list_item::create(kb_spinner, L"Physical");
        list_item::ptr kb_item_osd = list_item::create(kb_spinner, L"On Screen");
        list_item::ptr kb_item_none = list_item::create(kb_spinner, L"None");
        // done button (return to main)
        button::ptr config_done_button = button::create(config_state, L"Done",
                rectangle(530, 440, 100, 30));
        config_done_button->on_release.connect(boost::bind(&application::current_state, 
                    app.get(), main_state));

        // run the program
        retval = app->run();
    }
    catch (const exception& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    catch (...)
    {
        std::cout << "Unkown exception caught, aborting" << std::endl;
    }

    return retval;
}
