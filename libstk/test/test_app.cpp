/***************************************************************************************************
 *    FILENAME: test_app.cpp
 * DESCRIPTION: An example application using Libstk.
 *     AUTHORS: Darren Hart, Vernon Mauery, Marc Straemke
 *  START DATE: 22/Feb/2003  LAST UPDATE: 14/Jul/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 **************************************************************************************************/

#include <iostream>

// stk common include
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
using std::cout;
using std::endl;

struct no_op
{
    bool operator()()
    {
        cout << "no_op::operator()" << endl;
        return true;
    }
};

bool scroll_slot(scroll_model::ptr target, int increment)
{
    std::cout << "Scrolling, old begin=" << target->begin() << " visible area=" 
        << target->vis_size() << " size=" << target->size();
    target->begin(target->begin()+increment);
    return true;
}

bool print_edit_box_text(std::wstring text)
{
    // FIXME: better idea how to output a wstring?
    std::cout << "edit_box text: \"" << std::string(text.begin(), text.end()) << "\"" << endl;
    return true;
}

int main(int argc, char* argv[])
{
    int retval = 0;
 
    try
    {
        std::string surface_type;
        if (argc < 2)
        {
            throw error_message_exception("Usage: test_app sdl|dfb|fbdev");
        }
        else
        {
            surface_type = std::string(argv[1]);
        }
        
        // select the surface and event system
        cout << "test_app - selecting surface and event system" << endl;
        surface::ptr test_surface;
        event_producer::ptr test_event_producer;
#ifdef HAVE_SDL
        if (surface_type == "sdl")
        {
            test_surface = surface_sdl::create(rectangle(0, 0, 640, 480));
            test_event_producer = event_producer_sdl::create();
        }
#endif
#ifdef HAVE_DFB
        else if (surface_type == "dfb")
        {
            test_surface = surface_dfb::create(rectangle(0, 0, 640, 480));
            // FIXME: use something a bit more generic for the test app
            test_event_producer = event_producer_elotouch::create("/dev/tts/0");
        }
#endif
#ifdef HAVE_FBDEV
#ifdef HAVE_SDL // needed for the event_system
        else if (surface_type == "fbdev")
        {
            test_surface = surface_dfb::create(rectangle(0, 0, 640, 480));
            test_event_producer = event_producer_sdl::create();
        }
#endif
#endif
        else
            throw error_message_exception("Unknown surface type");

        // create the application
        cout << "test_app - creating application" << endl;
        application::ptr test_app = application::create(test_surface);

        // create the main state
        cout << "test_app - creating state" << endl;
        state::ptr test_state = state::create(test_app);

        // create a button and bind it to application::quit()
        cout << "test_app - creating button, binding on_click to test_app->quit()" << endl;
        button::ptr test_button = button::create(test_state, L"Quit",
                rectangle(120, 10, 100, 30));
        // NOTE: we must use test_app.get() here or we inadvertantly create a circular shared_ptr reference
        // and application (and therefore nothing else) won't get destroyed when we exit()!!!
        test_button->on_release.connect( boost::bind(&application::quit, test_app.get()) );
        button::ptr test_button2 = button::create(test_state, L"No-Op",
                rectangle(240, 10, 100, 30));

        // create a couple labels using ÜberScript
        cout << "test_app - creating labels" << endl;
        label::ptr test_label = label::create(test_state, std::wstring(L"ÜberScript"),
                rectangle(10, 10, 100, 30));
        label::ptr test_label2 = label::create(test_state,
                std::wstring(L"stk::tribal_theme"),
                rectangle(490, 10, 150, 30));

        // create some buttons with hotkeys F1-F4
        cout << "test_app - creating buttons, binding F1-F4" << endl;
        button::ptr test_button_F1 = button::create(test_state, L"F1 (Quit)", 
                rectangle(10, 400, 100, 30));
        test_button_F1->on_release.connect( test_button->on_release );
        button::ptr test_button4 = button::create(test_state, L"F2", rectangle(120, 400, 100, 30));
        button::ptr test_button5 = button::create(test_state, L"F3", rectangle(230, 400, 100, 30));
        button::ptr test_button6 = button::create(test_state, L"F4", rectangle(340, 400, 100, 30));

        // HOTKEYS
        hotkey hotkey_F1(f1);
        hotkey_F1.sig.connect(test_button_F1->on_release);
        test_state->on_keyup.connect(boost::ref(hotkey_F1));
        // END HOTKEYS

        // create a progress bar
        cout << "test_app - creating a progress bar" << endl;
        progress::ptr test_progress = progress::create(test_state, 
                std::wstring(L"Loading - %p %d %f"), rectangle(10, 440, 320, 30), 100);
        test_progress->percent(0.64);

        // create an image in a scroll panel
        cout << "test_app - creating an image_panel in a scroll panel" << endl;
        label::ptr test_label3 = label::create(test_state, std::wstring(L"Scrollable Image"),
                rectangle(10, 50, 150, 30));
        viewport::ptr test_viewport = viewport::create(test_state, rectangle(10, 90, 350, 200));
        // fixme: having to define the rectangle like this is lame, perhaps
        // rect should be relative to the parent container?
        image_panel::ptr test_image_panel = image_panel::create(test_viewport, 
                rectangle(10, 90, 400, 300), 
                image::create(test_viewport->surface(),"parrots.png"));

        //scroll(test_viewport->h_scroll(),-10);
        button::ptr scroll_left=button::create(test_state,L"Scroll-", rectangle(100, 300, 90, 40));
        scroll_left->on_release.connect(boost::bind(&scroll_slot, test_viewport->h_scroll(), -10));
        button::ptr scroll_right=button::create(test_state,L"Scroll+", rectangle(200, 300, 90, 40));
        scroll_right->on_release.connect(boost::bind(&scroll_slot, test_viewport->h_scroll(), 10));
        //void scroll(scroll_model::ptr target,int increment)

        // create a list
        cout << "test_app - creating a list with items" << endl;
        list::ptr test_list = list::create(test_state, rectangle(370, 90, 150, 200));
        list_item::ptr test_item_1 = list_item::create(test_list, L"Armenia");
        list_item::ptr test_item_2 = list_item::create(test_list, L"Canada");
        list_item::ptr test_item_3 = list_item::create(test_list, L"China");
        list_item::ptr test_item_4 = list_item::create(test_list, L"Germany");
        list_item::ptr test_item_5 = list_item::create(test_list, L"Great Britain");
        list_item::ptr test_item_6 = list_item::create(test_list, L"Guatemala");
        list_item::ptr test_item_7 = list_item::create(test_list, L"Honduras");
        list_item::ptr test_item_8 = list_item::create(test_list, L"Japan");
        list_item::ptr test_item_9 = list_item::create(test_list, L"Nicaragua");
        list_item::ptr test_item_10 = list_item::create(test_list, L"United States");
        list_item::ptr test_item_11 = list_item::create(test_list, L"Uruguay");

        // create a spinner
        cout << "test_app - creating a spinner with items" << endl;
        spinner::ptr test_spinner = spinner::create(test_state, rectangle(530, 90, 100, 30), false);
        list_item::ptr test_item_20 = list_item::create(test_spinner, L"Apple");
        list_item::ptr test_item_21 = list_item::create(test_spinner, L"Banana");
        list_item::ptr test_item_22 = list_item::create(test_spinner, L"Orange");
        list_item::ptr test_item_23 = list_item::create(test_spinner, L"Plum");

        // create a numeric_spinner
        cout << "test_app - creating a spinner with items" << endl;
        numeric_spinner::ptr test_numeric_spinner = numeric_spinner::create(test_state, 
                rectangle(530, 130, 100, 30), 0.0, 10.0, .5, 2, false);

        // create an edit_box
        cout << "test_app - creating edit_box" << endl;
        edit_box::ptr test_edit = edit_box::create(test_state, L"edit me", rectangle(340, 440, 260, 30));
        test_edit->on_release.connect(&print_edit_box_text);
        
        // add a timer (quit after 30 seconds)
        cout << "test_app - creating timer to quit after 30 seconds" << endl;
        timer::ptr test_timer = timer::create(30000, true); // every 20 seconds
        test_timer->on_timer.connect( boost::bind(&stk::application::quit, test_app.get()));
        test_app->add_timer(test_timer);

        // run the program
        retval = test_app->run();
    }
    catch (const exception& e)
    {
        cout << "Exception: " << e.what() << endl;
    }

    return retval;
}
