/******************************************************************************
 *    FILENAME: hello_world.cpp
 * DESCRIPTION: The typical hello_world application using Libstk, customized to Directfb backend. 
 *     AUTHORS: Darren Hart, Marc Strämke
 *  START DATE: 30/May/2003  LAST UPDATE: 28/Jun/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available
 *              with the source as license.txt or at 
 *              http://www.libstk.org/index.php?page=docs/license
 *****************************************************************************/

#include <iostream>
#include <fstream>
#include <libstk/stk.h>

// backend specific headers
#include <libstk/surface_dfb.h>

using namespace stk;

void exit_prog()
{
}


int main(int argc, char* argv[])
{
    int retval = 0;

    std::ofstream debug("debug.log");
    stk::logger::get()->add_target(&std::cerr, stk::LL_Error);
    stk::logger::get()->add_target(&debug, stk::LL_Info);
    
    try
    {
        
        // create the primary surface
        surface::ptr test_surface = surface_dfb::create(stk::rectangle(0,0,800,600));
        // create the application
        application::ptr test_app = application::create(test_surface);

        // create the main state and a label
        state::ptr test_state = state::create(test_app);
        label::ptr test_label = label::create(test_state, std::wstring(L"Hello World"), 
                rectangle(260, 210, 100, 30));

        timer::ptr test_timer = timer::create(10000, true); // every 5 seconds

        test_timer->on_timer.connect(boost::bind(&stk::application::quit, test_app.get()));
        test_app->add_timer(test_timer);
        // run the program
        retval = test_app->run();
    }
    catch (const exception& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    stk::logger::get()->remove_target(&std::cerr);
    stk::logger::get()->remove_target(&debug);
    return retval;
}
