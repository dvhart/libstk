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

#include <libstk/event_producer_sdl.h>
#include <libstk/surface_sdl.h>
#include <libstk/event_producer_elotouch.h>

using namespace stk;

int main(int argc, char* argv[])
{
    int retval = 0;

    try
    {
        logger::get()->add_target(&std::cout, LL_Info);

        // select the surface and event system
        surface::ptr screen;
        event_producer::ptr ep;
        INFO("creating sdl surface");
        screen = surface_sdl::create(rectangle(0, 0, 640, 480));
        INFO("creating sdl event_producer");
        ep = event_producer_sdl::create();

        
        // create the application
        application::ptr app = application::create(screen);

        // create the main state and a label
        state::ptr test_state = state::create(app);

        
        text_area::ptr test_area = text_area::create(test_state, std::wstring(L"Hello World\nthisisareallylonglonglonglonglongstring\nHello World"), rectangle(30, 30, 180, 80));
        
        INFO("hello_world - creating timer to quit after 15 seconds");
        timer::ptr quit_timer = timer::create(15000, false);
        quit_timer->on_timer.connect(boost::function<bool()>((boost::bind(&application::quit, app.get()), true)));
        app->add_timer(quit_timer);

        // run the program
        retval = app->run();
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
