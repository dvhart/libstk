#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <string>
#include <boost/lexical_cast.hpp>
#include "exceptions.h"

#include "application.h"
namespace stk
{

	application::application(Surface surface, EventSystem event_system) :
		surface_(surface), event_system_(event_system)
	{
	}

	application::~application()
	{
	}

	void application::run()
	{
		int retval = 0;
		Event event_;
		while (!done_)
		{
			while (event_ = event_system_->poll_event())
			{
				current_widget_->handle_event(event_);
			}
		}
		return retval;
	}

	void application::quit()
	{
		done_ = true;
	}
	
	void application::add_state(boost::shared_ptr<state> state)
	{
		states_.push_back(state);
	}

	// drawable interface
	boost::shared_ptr<surface> application::surface()
	{
		return surface_;
	}

	// event_handler interface
	void application::handle_event(event& e)
	{
		switch(e.type())
		{
			case key_down:
				((key_event)event).key(); // this is ugly, should we just use unions ?
				break;
			case key_up:
				break;
			case mouse_down:
				break;
			case mouse_up:
				break;
			case mouse_motion:
				break;
			default:
		}
	}
	
	// parent interface
	void application::add_child(boost::shared_ptr<widget> widget)
	{
	}
	
	boost::weak_ptr<state> application::current_state()
	{
		return current_state_;
	}
}


