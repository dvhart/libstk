#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <unistd.h>

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

	application::ptr application::create(surface::ptr surface, event_system::ptr event_system)
	{
		application::ptr new_application(new application(surface, event_system));
		return new_application;
	}
	
	application::application(surface::ptr surface, event_system::ptr event_system) :
		surface_(surface), event_system_(event_system), done_(false)
	{
		cout << "application::application()" << endl;
	}

	application::~application()
	{
		cout << "application::~application()" << endl;
	}

	int application::run()
	{
		cout << "application::run()" << endl;
		int retval = 0;
	
		// set the current state and focused_widget_
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
			focused_widget_ = (*states_.begin())->get_active_child();
			focused_widget_.lock()->focused(true);
		}
		
		// FIXME: we have to do something about all these .lock() calls!!!
		event::ptr event_(new event(no_event)); // should we use create here ?
		while (!done_)
		{
			rectangle t_rect = current_state_.lock()->redraw_rect();
			if ( !t_rect.empty() ) 
			{
				current_state_.lock()->draw(surface_);
				surface_->update( t_rect );
			}
			
			// handle all available events before redrawing
			event_ = event_system_->poll_event();
			while (event_->type() != no_event)
			{
				//cout << "application::run() - event received of type: " << event_->type() << endl;

				// if it's a mouse event, let current_state_ determine who to send it too
				if (event_->type() == mouse_motion ||
						event_->type() == mouse_down ||
						event_->type() == mouse_up)
				{
					mouse_event::ptr me = boost::shared_static_cast<mouse_event>(event_);
					
					// update hover_widget as necessary
					widget::ptr hover_ptr = hover_widget_.lock();
					if (!hover_ptr || !hover_ptr->contains(me->x(), me->y()))
					{
						// NOTE: only leaf widgets can be hover widgets!!!
						//cout << "changing hover_widget_" << endl;
						if (hover_ptr) 
						{
							hover_ptr->hover(false);
							hover_ptr->active(false);
						}
						hover_ptr = current_state_.lock()->widget_at(me->x(), me->y());
						if (hover_ptr) hover_ptr->hover(true);
						hover_widget_ = hover_ptr;
					}

					// FIXME: do some error checking on the widget pointers
					// update focused widget as necessary
					if (event_->type() == mouse_down && hover_widget_.lock() != focused_widget_.lock())
					{
						// FIXME: the container with the previous focused_widget is now out of sync
						// with the system!!!!!
						focused_widget_.lock()->focused(false);
						focused_widget_ = hover_widget_;
						focused_widget_.lock()->focused(true);
					}
					
					current_state_.lock()->delegate_mouse_event(me);
				}
				else
				{
					cout << "application::run() - passing event to focused_widget_" << endl;
					widget::ptr ptr = focused_widget_.lock();
					if (!ptr)
						cout << "application::run() - no current widget, pass to state ???" << endl;
					else
						ptr->handle_event(event_);
				}
				
				event_ = event_system_->poll_event();
			}
			usleep(1000); // 1 ms
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
		cout << "application::add_state()" << endl;
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
		//cout << "application::handle_event()" << endl;
		switch(e->type())
		{
			case key_down:
				break;
			case key_up:
			{
				// FIXME :Carter: shouldnt this be a polymorphic cast?
				key_event::ptr ke = boost::shared_static_cast<key_event>(e);
				//cout << "application::handle_event() - key pressed: " << ke->key() << endl;
				switch ( ke->key() )
				{
					case key_esc:
						quit();
						break;
					case key_tab:
					case right_arrow:
					case down_arrow:
						cout << "application::handle_event() - next pressed" << endl;
						cout << "\tfocused_widget_: " << focused_widget_.lock().get() << endl;
						focused_widget_ = focused_widget_.lock()->focus_next();
						focused_widget_.lock()->focused(true);
						break;
					case key_enter:
						cout << "application::handle_event() - enter pressed" << endl;
						break;
					case left_arrow:
					case up_arrow:
						cout << "application::handle_event() - prev pressed" << endl;
						cout << "\tfocused_widget_: " << focused_widget_.lock().get() << endl;
						focused_widget_ = focused_widget_.lock()->focus_prev();
						focused_widget_.lock()->focused(true);
						break;
				}
				break;
			}
			case mouse_down:
				break;
			case mouse_up:
				break;
			case mouse_motion:
				break;
			case event_quit:
				quit();
				break;
			//default:
				//cout << "application::handle_event - unknown event" << endl;
		}
	}
	
	// parent interface
	// FIXME
	widget::ptr application::focus_next()
	{ 
		cout << "application::focus_next()" << endl;
		return *states_.begin();
	}
	// FIXME
	widget::ptr application::focus_prev()
	{ 
		cout << "application::focus_prev()" << endl;
		return *states_.begin();
	}

	state::weak_ptr application::current_state()
	{
		return current_state_;
	}
}


