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
	// initialize sdl
	// FIXME: where should this be done ?
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		throw std::string("Unable to init SDL: " + std::string(SDL_GetError()));
	atexit(SDL_Quit);
		
	// create the app
	cout << "test_app - creating surface" << endl;
	Surface test_surface(new surface_sdl(rectangle(0, 0, 640, 480)));
	cout << "test_app - creating event system" << endl;
	EventSystem test_event_system(new event_system_sdl());
	cout << "test_app - creating application" << endl;
	Application test_app(new application(test_surface, test_event_system));

	// create the main state
	cout << "test_app - creating state" << endl;
	State test_state(new state(test_app));
	test_app->add_state(test_state);
	//Label test_label(new label(test_state, "Test Label", 10, 10, 100, 30));
	
	cout << "test_app - creating button" << endl;
	Button test_button(new button(test_state, "Test Button", 120, 10, 100, 30));
	
	test_state->add_child(test_button);
	// bind button click to quit
	test_button->on_click.connect( boost::bind(&stk::application::quit, test_app) );

	// run the program
	cout << "test_app - run" << endl;
	return test_app->run();
}

