/******************************************************************************
 *    FILENAME: application.cpp
 * DESCRIPTION: Application class implementation.
 *     AUTHORS: Darren Hart, Marc Straemke
 *  START DATE: 03/Mar/2003  LAST UPDATE: 13/May/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available
 *              with the source as license.txt or at 
 *              http://www.libstk.org/index.php?page=docs/license
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <unistd.h>

#include "libstk/application.h"
#include "libstk/event_system.h"
#include "libstk/surface.h"
#include "libstk/exceptions.h"
#include "libstk/event.h"
#include "libstk/key_event.h"
#include "libstk/mouse_event.h"
#include "libstk/state.h"
#include "libstk/theme.h"

using std::cout;
using std::endl;

namespace stk
{

	application::ptr application::create(surface::ptr surface)
	{
		application::ptr new_application(new application(surface));
		return new_application;
	}
	
	application::application(surface::ptr surface) :
		surface_(surface), event_system_(event_system::get()), done_(false)
	{
		cout << "application::application()" << endl;
		// initialize the theme
		theme::create(surface);
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
			focused_widget_.lock()->handle_event(event::create(event::focus));
		}
		
		// enter the main application loop: draw, handle_events, call timers
		// FIXME: we have to do something about all these .lock() calls!!!
		event::ptr event_ = event::create(event::none); // should we use create here ?
		while (!done_)
		{
			rectangle t_rect = current_state_.lock()->redraw_rect();
			if ( !t_rect.empty() ) 
			{
				//cout << "applicaiton::run() - redrawing state" << endl;
				current_state_.lock()->draw(surface_);
				surface_->update( t_rect );
			}
			else
			{
				//cout << "applicaiton::run() - nothing to redraw" << endl;
			}
			
			// handle all available events before redrawing
			event_ = event_system_->poll_event();
			while (event_->type() != event::none)
			{
				//cout << "application::run() - event received of type: " << event_->type() << endl;

				// if it's a mouse event, let current_state_ determine who to send it too
				if (event_->type() == event::mouse_motion ||
						event_->type() == event::mouse_down ||
						event_->type() == event::mouse_up)
				{
					mouse_event::ptr me = boost::shared_static_cast<mouse_event>(event_);
					
					// update hover_widget as necessary
					widget::ptr hover_ptr = hover_widget_.lock();
					if (!hover_ptr || !hover_ptr->contains(me->x(), me->y()))
					{
						// NOTE: only leaf widgets can be hover widgets!!!
						if (hover_ptr) 
							hover_ptr->handle_event(event::create(event::mouse_leave));
						hover_ptr = current_state_.lock()->widget_at(me->x(), me->y());
						if (hover_ptr) 
							hover_ptr->handle_event(event::create(event::mouse_enter));
						hover_widget_ = hover_ptr;
					}

					// FIXME: do some error checking on the widget pointers
					// update focused widget as necessary
					if (event_->type() == event::mouse_down && 
							hover_widget_.lock() && // not a null hover widget
							hover_widget_.lock() != focused_widget_.lock())
					{
						focused_widget_.lock()->handle_event(event::create(event::un_focus));
						focused_widget_ = hover_widget_;
						focused_widget_.lock()->handle_event(event::create(event::focus));
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

			// update all timers
			std::list<timer::ptr>::iterator t_iter = timers_.begin();
			for (t_iter; t_iter != timers_.end(); t_iter++)
			{
				if (!(*t_iter)->update()) 
				{
					// FIXME: delete this timer
				}
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

	void application::add_timer(timer::ptr timer)
	{
		cout << "application::add_timer()" << endl;
		timers_.push_back(timer);
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
			case event::key_down:
				break;
			case event::key_up:
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
					{
						cout << "application::handle_event() - next pressed" << endl;
						component::weak_ptr prev_focused_widget = focused_widget_;
						focused_widget_ = prev_focused_widget.lock()->focus_next();
						component::weak_ptr temp_widget = prev_focused_widget;
						while (!focused_widget_.lock())
						{
							temp_widget = temp_widget.lock()->parent();
							if (!temp_widget.lock())
								throw error_message_exception("application::handle_event() - unable to find next focusable widget");
							focused_widget_ = temp_widget.lock()->focus_next();
						}
						prev_focused_widget.lock()->handle_event(event::create(event::un_focus)); 
						focused_widget_.lock()->handle_event(event::create(event::focus));
						break;
					}
					case key_enter:
						cout << "application::handle_event() - enter pressed" << endl;
						break;
					case left_arrow:
					case up_arrow:
					{
						cout << "application::handle_event() - prev pressed" << endl;
						component::weak_ptr prev_focused_widget = focused_widget_;
						focused_widget_ = focused_widget_.lock()->focus_prev();
						component::weak_ptr temp_widget = prev_focused_widget;
						while (!focused_widget_.lock())
						{
							temp_widget = temp_widget.lock()->parent();
							if (!temp_widget.lock())
								throw error_message_exception("application::handle_event() - unable to find previous focusable widget");
							focused_widget_ = temp_widget.lock()->focus_prev();
						}
						prev_focused_widget.lock()->handle_event(event::create(event::un_focus));
						focused_widget_.lock()->handle_event(event::create(event::focus));
						break;
					}
					default:
						// unhandled key
						break;
				}
				break;
			}
			case event::mouse_down:
				break;
			case event::mouse_up:
				break;
			case event::mouse_motion:
				break;
			case event::quit:
				quit();
				break;
			default:
				break;
				//cout << "application::handle_event - unknown event" << endl;
		}
	}
	
	// component interface
	// FIXME
	widget::ptr application::focus_next()
	{ 
		cout << "application::focus_next()" << endl;
		return current_state_.lock()->focus_first(); 
	}

	// FIXME
	widget::ptr application::focus_prev()
	{ 
		cout << "application::focus_prev()" << endl;
		return current_state_.lock()->focus_last(); 
	}
}
