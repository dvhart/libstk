#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/signal.hpp>
#include <boost/bind.hpp>

#include "libstk/application.h"
#include "libstk/button.h"
#include "libstk/event_system.h"
#include "libstk/event_system_sdl.h"
#include "libstk/exceptions.h"
#include "libstk/image.h"
#include "libstk/image_panel.h"
#include "libstk/label.h"
#include "libstk/list.h"
#include "libstk/list_item.h"
#include "libstk/progress.h"
#include "libstk/spinner.h"
#include "libstk/scroll_decorator.h"
#include "libstk/state.h"
#include "libstk/surface.h"
#include "libstk/surface_sdl.h"

using namespace stk;
using std::cout;
using std::endl;

// FIXME: delete this when we solve the bug
// a no-op slot, since it segfaults if on_click is empty in button
struct no_op
{
	bool operator()() const
	{
		cout << "no_op slot" << endl;
		return true;
	}
};

int main(int argc, char* argv[])
{
	int retval = 0;

	try
	{
		// initialize sdl
		// FIXME: where should this be done ?, perhaps in an application factory?
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
			throw error_message_exception(std::string("Unable to init SDL: ") + std::string(SDL_GetError()));
		atexit(SDL_Quit);
			
		// create the surface
		cout << "test_app - creating surface" << endl;
		surface::ptr test_surface = surface_sdl::create(rectangle(0, 0, 640, 480));
		
		// create the event system
		cout << "test_app - creating event system" << endl;
		event_system::ptr test_event_system = event_system_sdl::create();
	
		// create the application
		cout << "test_app - creating application" << endl;
		application::ptr test_app = application::create(test_surface, test_event_system);
	
		// create the main state
		cout << "test_app - creating state" << endl;
		state::ptr test_state = state::create(test_app);
		
		// create a button and bind it to application::quit()
		cout << "test_app - creating button, binding on_click to test_app->quit()" << endl;
		button::ptr test_button = button::create(test_state, L"Quit", 
			rectangle(120, 10, 100, 30));
		test_button->on_release.connect( boost::bind(&stk::application::quit, test_app) );
		button::ptr test_button2 = button::create(test_state, L"No-Op", 
			rectangle(240, 10, 100, 30));
		
		// create a couple labels using ÜberScript
		cout << "test_app - creating labels" << endl;
		label::ptr test_label = label::create(test_state, std::wstring(L"ÜberScript"), 
			rectangle(10, 10, 100, 30));
		label::ptr test_label2 = label::create(test_state, 
			std::wstring(L"stk::tribal_theme"), 
			rectangle(490, 10, 150, 30));
		
		// create a progress bar
		cout << "test_app - creating a progress bar" << endl;
		progress::ptr test_progress = progress::create(test_state, 
			std::wstring(L"Loading - %p %d %f"), rectangle(10, 440, 320, 30), 100);
		test_progress->percent(0.64);
	
		// create an image in a scroll panel
		cout << "test_app - creating an image_panel in a scroll panel" << endl;
		label::ptr test_label3 = label::create(test_state, std::wstring(L"Clipped Image"), 
			rectangle(10, 50, 150, 30));
		/*
		scroll_decorator::ptr test_scroll = scroll_decorator::create(test_state, 
				rectangle(10, 90, 350, 200));
		image_panel::ptr test_image_panel = image_panel::create(test_scroll, 
				rectangle(10, 90, 400, 400), image::create("parrots.ppm"));
		*/

		// create a list
		cout << "test_app - creating a list with items" << endl;
		list::ptr test_list = list::create(test_state, rectangle(370, 90, 150, 200));
		list_item::ptr test_item_1 = list_item::create(test_list, L"Armenia", "armenia");
		list_item::ptr test_item_2 = list_item::create(test_list, L"Germany", "germany");
		list_item::ptr test_item_3 = list_item::create(test_list, L"Japan", "japan");
		list_item::ptr test_item_4 = list_item::create(test_list, L"United States", "united_states");
		
		// create a spinner
		cout << "test_app - creating a spinner with items" << endl;
		spinner::ptr test_spinner = spinner::create(test_state, rectangle(530, 90, 100, 30));
		list_item::ptr test_item_5 = list_item::create(test_spinner, L"Apple", "apple");
		list_item::ptr test_item_6 = list_item::create(test_spinner, L"Banana", "banana");
		list_item::ptr test_item_7 = list_item::create(test_spinner, L"Orange", "orange");
		list_item::ptr test_item_8 = list_item::create(test_spinner, L"Plum", "plum");

		// run the program
		retval = test_app->run();
	}
	catch (const exception& e)
	{
		cout << "Exception: " << e.what() << endl;
	}

	return retval;
}
