/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Application class implementation.
 *     AUTHORS: Darren Hart, Marc Strämke
 *  START DATE: 2003/Mar/03
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <unistd.h>
#include <iostream>
#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include "libstk/application.h"
#include "libstk/event_system.h"
#include "libstk/surface.h"
#include "libstk/exceptions.h"
#include "libstk/event.h"
#include "libstk/key_event.h"
#include "libstk/mouse_event.h"
#include "libstk/state.h"
#include "libstk/theme.h"
#include "libstk/logging.h"

namespace stk
{
    application::ptr application::instance_;
    
    application::ptr application::create(surface::ptr surface)
    {
        if (instance_) throw error_message_exception("application::create() - "
                "application already instantiated (create can only be called once)");
        instance_.reset(new application(surface));
        return instance_;
    }

    application::ptr application::get()
    {
        if (!instance_) throw error_message_exception("application::get() - "
                "application not instantiated (create has not been called)");
        return instance_;
    }
    
    application::application(surface::ptr surface) :
            surface_(surface), event_system_(event_system::get()), done_(false)
    {
        // initialize the theme
        theme::create(surface);
    }

    application::~application()
    {
    }

    int application::run()
    {
        INFO("Start");
        int retval = 0;

        // set the current state and focused_widget_
        if (states_.size() == 0)
        {
            // FIXME: throw something
            ERROR("application state count is zero!");
            return 1;
        }
        else
        {
            current_state_ = *states_.begin();
            // FIXME: ask current_state_ for its first focusable widget
            //focused_widget_ = (*states_.begin())->get_active_child();
            focused_widget_ = (*states_.begin())->focus_next();
            if (focused_widget_.lock())
                focused_widget_.lock()->handle_event(event::create(event::focus));
            else
                // FIXME: throw something
                ERROR("current state has no focusable widgets");
                
        }
        
        current_state_.lock()->redraw(surface_->rect());
        
        // enter the main application loop: draw, handle_events, call timers
        // FIXME: we have to do something about all these .lock() calls!!!
        event::ptr event_ = event::create(event::none); // should we use create here ?
        while (!done_)
        {
            rectangle t_rect = current_state_.lock()->redraw_rect();
            if ( !t_rect.empty() )
            {
                INFO("applicaiton::run() - redrawing state\n\tWith Cliprect " << t_rect);
                on_predraw(t_rect);
                current_state_.lock()->draw(surface_,t_rect);
                on_postdraw(t_rect);
                surface_->update( t_rect );
            }
            else
            {
                //INFO("nothing to redraw");
            }

            // handle all available events before redrawing
            event_ = event_system_->poll_event();
            while (event_->type() != event::none)
            {
                //INFO("event received of type: 0x" << std::hex << event_->type());
		bool handled_by_on_event = on_event(event_);
		if (!handled_by_on_event)
		{
		    // if it's a mouse event, let current_state_ determine who to send it too
		    if (event_->type() == event::mouse_motion ||
                        event_->type() == event::mouse_down ||
                        event_->type() == event::mouse_up)
		    {
			mouse_event::ptr me = boost::shared_static_cast<mouse_event>(event_);
			
			// update hover_widget as necessary
			widget::ptr hover_ptr = hover_widget_.lock();
                        // FIXME: this is broken, me->x is world coords, contains uses local coords
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
			INFO("passing event to focused_widget_");
			widget::ptr ptr = focused_widget_.lock();
			if (!ptr)
                        {
			    WARN("no current widget, pass to state ?");
                        }
			else
                        {
			    ptr->handle_event(event_);
                        }
		    }
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
        INFO("Done");
        return retval;
    }

    void application::quit()
    {
        done_ = true;
    }

    void application::add_state(state::ptr state)
    {
        states_.push_back(state);
    }

    void application::add_timer(timer::ptr timer)
    {
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
        //INFO("application::handle_event()");
        switch(e->type())
        {
        case event::key_down:
        {
            // FIXME :Carter: shouldnt this be a polymorphic cast?
            key_event::ptr ke = boost::shared_static_cast<key_event>(e);
            switch (ke->fn_key())
            {
            case key_escape:
                quit();
                break;
            case key_tab:
            case key_rightarrow:
            case key_downarrow:
            {
                INFO("next pressed");
                component::weak_ptr prev_focused_widget = focused_widget_;
                focused_widget_ = prev_focused_widget.lock()->focus_next();
                component::weak_ptr temp_widget = prev_focused_widget;
                while (!focused_widget_.lock())
                {
                    INFO("no more focusable widgets, asking parent");
                    temp_widget = temp_widget.lock()->parent();
                    if (!temp_widget.lock())
                        throw error_message_exception("application::handle_event() - "
                                                      "unable to find next focusable widget");
                    focused_widget_ = temp_widget.lock()->focus_next();
                }
                prev_focused_widget.lock()->handle_event(event::create(event::un_focus));
                focused_widget_.lock()->handle_event(event::create(event::focus));
                break;
            }
            case key_enter:
                INFO("enter pressed");
                break;
            case key_leftarrow:
            case key_uparrow:
            {
                INFO("prev pressed");
                component::weak_ptr prev_focused_widget = focused_widget_;
                focused_widget_ = focused_widget_.lock()->focus_prev();
                component::weak_ptr temp_widget = prev_focused_widget;
                while (!focused_widget_.lock())
                {
                    temp_widget = temp_widget.lock()->parent();
                    if (!temp_widget.lock())
                        throw error_message_exception("application::handle_event() - "
                                                      "unable to find previous focusable widget");
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
        case event::key_up:
            break;
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
            //INFO("application::handle_event - unknown event");
        }
    }

    // component interface
    // FIXME: this is wrong, what should we do? change to the next state ?
    widget::ptr application::focus_next()
    {
        return current_state_.lock()->focus_first();
    }

    // FIXME: this is wrong, what should we do? change to the previous state ?
    widget::ptr application::focus_prev()
    {
        return current_state_.lock()->focus_last();
    }

    void application::current_state(state::ptr new_cur_state)
    {
        // FIXME...
        ///\todo check if this state is null and if it exists in our list, else through exception, return false,  or add it
        
        // remove focus from the current widget
        if (focused_widget_.lock())
            focused_widget_.lock()->handle_event(event::create(event::un_focus));
        
        current_state_ = new_cur_state;
        
        // focus the first focusable widget of the new current state
        focused_widget_ = current_state_.lock()->focus_next();
        if (focused_widget_.lock())
            focused_widget_.lock()->handle_event(event::create(event::focus));
        else
            // FIXME: throw something
            ERROR("application::run() - current state has no focusable widgets");
        
        new_cur_state->redraw(new_cur_state->rect());
    }

    state::ptr application::current_state() const
    {
        return current_state_.lock();
    }

}
