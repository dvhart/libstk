#include <iostream>

#include "application.h"
#include "event_system.h"
#include "event_system_sdl.h"
#include "surface.h"
#include "surface_sdl.h"

using namespace stk;
using std::cout;
using std::endl;

// something like this defined in the class headers... not sure on naming convention yet
typedef boost::shared_ptr<surface> Surface;
typedef boost::shared_ptr<event_system> EventSystem;
typedef boost::shared_ptr<application> Application;
typedef boost::shared_ptr<state> State;
typedef boost::shared_ptr<button> Button;

int main(int argc, char* argv[])
{
	// create the app
	Surface test_surface(new surface_sdl());
	EventSystem test_event_system(new event_system_sdl());
	Application test_app(new application(test_surface, test_event_system));

	// create the main state
	State test_state(new state(test_app));
	Label test_label(new label(test_state, "Test Label", 10, 10, 100, 30));
	Button test_button(new button(test_state, "Test Button", 120, 10, 100, 30));
	// bind button click to quit
	// do we need to dereference test_app ?
	test_button->on_click.connect(boost::bind(&stk::application::quit, test_app)); 

	// run the program
	return test_app->run();
}

