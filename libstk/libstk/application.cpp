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

	application::application(surface::ptr surface, event_system::ptr event_system) :
		surface_(surface), event_system_(event_system), done_(false)
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
			// FIXME: ask current_state_ for its first focusable widget
			current_widget_ = (*states_.begin())->get_active_child();
			hover_widget_ = *states_.begin();
		}
		
		event::ptr event_(new event(no_event));
		while (!done_)
		{
			event_ = event_system_->poll_event();
			if (event_->type() != no_event)
			{
				//cout << "application::run() - event received of type: " << event_.type() << endl;
				widget::ptr ptr=make_shared(current_widget_);
				if (!ptr)
					cout << "application::run() - no current widget" << endl;
				else
					ptr->handle_event(event_);
			}
		}
		cout << "application::run() - leaving" << endl;
		return retval;
	}

	bool application::quit()
	{
		done_ = true;
		return true;
	}
	
	void application::add_state(state::ptr state)
	{
		states_.push_back(state);
	}

	// drawable interface
	surface::ptr application::surface()
	{
		return surface_;
	}

	// event_handler interface
	void application::handle_event(event::ptr e)
	{
		cout << "application::handle_event()" << endl;
		switch(e->type())
		{
			case key_down:
			{		
				// FIXME :Carter: shouldnt this be a polymorphic cast?
				key_event::ptr ke = boost::shared_static_cast<key_event>(e);
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
	widget::ptr application::focus_next()
	{ 
		return boost::shared_ptr<widget>((widget *)((*states_.begin()).get()));
	}
	// FIXME
	widget::ptr application::focus_prev()
	{ 
		return boost::shared_ptr<widget>((widget *)((*states_.begin()).get()));
	}

	// FIXME :Carter: Throw something? add_child is meaningless for an application
	void application::add_child(widget::ptr widget)
	{
	}
	
	state::weak_ptr application::current_state()
	{
		return current_state_;
	}
}


