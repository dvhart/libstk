#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/signal.hpp>
#include <boost/bind.hpp>

#include "application.h"
#include "state.h"
#include "button.h"
#include "event_system.h"
#include "event_system_sdl.h"
#include "surface.h"
#include "surface_sdl.h"

using namespace stk;
using std::cout;
using std::endl;

int main(int argc, char* argv[])
{
	int retval = 0;

	// initialize sdl
	// FIXME: where should this be done ?, perhaps in an application factory?
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		throw std::string("Unable to init SDL: " + std::string(SDL_GetError()));
	atexit(SDL_Quit);
		
	// create the surface
	cout << "test_app - creating surface" << endl;
	//surface::ptr test_surface(new surface_sdl(rectangle(0, 0, 640, 480)));
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
	
	//Label test_label(new label(test_state, "Test Label", 10, 10, 100, 30));
	
	// create a button and bind it to application::quit()
	cout << "test_app - creating button, binding on_click to test_app->quit()" << endl;
	button::ptr test_button = button::create(test_state, "Test Button", 120, 10, 100, 30);
	test_button->on_click.connect( boost::bind(&stk::application::quit, test_app) );

	// check use count prior to run
	cout << "SHARED POINTER USE COUNT PRIOR TO RUN" << endl;
	cout << "test_surface.use_count() = " << test_surface.use_count() << endl;
	cout << "test_event_system.use_count() = " << test_event_system.use_count() << endl;
	cout << "test_app.use_count() = " << test_app.use_count() << endl;
	cout << "test_state.use_count() = " << test_state.use_count() << endl;
	cout << "test_button.use_count() = " << test_button.use_count() << endl;
	
	// run the program
	cout << "test_app - run" << endl;
	retval = test_app->run();
	cout << "test_app - returning " << std::dec << retval << endl;

	return retval;
}

