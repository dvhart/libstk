#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include "application.h"
#include "event_system.h"
#include "surface.h"
#include "exceptions.h"
#include "event.h"
#include "key_event.h"
#include "mouse_event.h"

using std::cout;
using std::endl;

namespace stk
{

	application::application(Surface surface, EventSystem event_system) :
		surface_(surface), event_system_(event_system)
	{
		cout << "application::application()" << endl;
	}

	application::~application()
	{
	}

	int application::run()
	{
		cout << "application::run()" << endl;
		int retval = 0;
	
		if (states_.size() == 0) 
		{
			// FIXME: throw something
			cout << "application::run() - application state count is zero!" << endl;
			return 1;
		}
		else
		{
			current_state_ = *states_.begin();
			// CARTER
			// FIXME: it refuses to let me assign current_widget_, I have tried some of the following
			//boost::shared_ptr<widget> tptr( new widget( *states_.begin() ) );
			//boost::weak_ptr<widget> tptr( (widget *)((*(states_.begin())).get()) );
			//current_widget_ = tptr;
		}
		
		event event_(no_event);
		while (!done_)
		{
			event_ = event_system_->poll_event();
			cout << "application::run() - event received of type: " << event_.type() << endl;
			if (event_.type() != no_event)
			{
				current_widget_.get()->handle_event(event_);
			}
		}
		cout << "application::run() - leaving" << endl;
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
				//((key_event)e).key(); // this is ugly, should we just use unions ?
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
				cout << "application::handle_event - unknown event" << endl;
		}
	}
	
	// parent interface
	// FIXME
	boost::shared_ptr<widget> application::focus_next()
	{ 
		return boost::shared_ptr<widget>((widget *)((*states_.begin()).get()));
	}
	// FIXME
	boost::shared_ptr<widget> application::focus_prev()
	{ 
		return boost::shared_ptr<widget>((widget *)((*states_.begin()).get()));
	}

	void application::add_child(boost::shared_ptr<widget> widget)
	{
	}
	
	boost::weak_ptr<state> application::current_state()
	{
		return current_state_;
	}
}


