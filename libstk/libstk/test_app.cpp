#include <iostream>

#include "application.h"

using namespace stk;
using std::cout;
using std::endl;

// something like this defined in the class headers... not sure on naming convention yet
typedef boost::shared_ptr<application> Application;
typedef boost::shared_ptr<state> State;
typedef boost::shared_ptr<button> Button;

void main(int argc, char* argv[])
{
	// create the app
	Application test_app(new application(/* event/surface parameters */));

	// create the main state
	State test_state(new state(test_app));
	Label test_label(new label(test_state), "Test Label", 10, 10, 100, 30);
	Button test_button(new button(test_state), "Test Button", 120, 10, 100, 30);
	// bind button click to quit
	// do we need to dereference test_app ?
	test_button->on_click.connect(boost::bind(&stk::app::quit, test_app); 

	// run the program
	return test_app->run();
}

