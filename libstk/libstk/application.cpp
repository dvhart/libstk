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
#include "state.h"

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
		done_ = false;
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
			// FIXME: is there a better way ?
			current_widget_ = boost::shared_dynamic_cast<widget>(*states_.begin());
			hover_widget_ = boost::shared_dynamic_cast<widget>(*states_.begin());
		}
		
		Event event_(new event(no_event));
		while (!done_)
		{
			event_ = event_system_->poll_event();
			if (event_->type() != no_event)
			{
				//cout << "application::run() - event received of type: " << event_.type() << endl;
				if (current_widget_.get() == 0)
					cout << "application::run() - null current widget" << endl;
				else
					current_widget_.get()->handle_event(event_);
				//cout << "application::run() - done is currently " << done_ << endl;
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
	void application::handle_event(boost::shared_ptr<stk::event> e)
	{
		cout << "application::handle_event()" << endl;
		switch(e->type())
		{
			case key_down:
			{		
				KeyEvent ke = boost::shared_static_cast<key_event>(e);
				switch ( ke->key() )
				{
					case key_esc:
						quit();
						break;
				}
				break;
			}
			case key_up:
				break;
			case mouse_down:
				break;
			case mouse_up:
				break;
			case mouse_motion:
				break;
			case event_quit:
				quit();
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


