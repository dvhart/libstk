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
        INFO("destructor");
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
            // set the current state and find the first focusable widget
            current_state_ = *states_.begin();
            focused_widget_ = (*states_.begin())->focus_next();
            if (focused_widget_.lock())
                focused_widget_.lock()->handle_event(event::create(event::focus));
            else
                // FIXME: throw something
                ERROR("current state has no focusable widgets");

        }

        current_state_.lock()->redraw(surface_->rect());

        // enter the main application loop: handle_events, update timers, redraw
        event::ptr event_;
        while (!done_)
        {
            // handle all available events before redrawing
            event_ = event_system_->poll_event();
            while (event_->type() != event::none)
            {
                //INFO("event received of type: 0x" << std::hex << event_->type());
                bool handled_by_on_event = on_event(event_);
                if (!handled_by_on_event)
                { 
                    // if it's a mouse event
                    if (event_->type() == event::mouse_motion ||
                            event_->type() == event::mouse_down ||
                            event_->type() == event::mouse_up)
                    {
                        mouse_event::ptr me = boost::shared_static_cast<mouse_event>(event_);
                        // let current_state_ determine who to send it to
                        widget::ptr new_hover_ptr = current_state_.lock()->delegate_mouse_event(me);
                        // temporary shared pointers (avoid so many .lock() calls below)
                        widget::ptr hover_ptr = hover_widget_.lock();
                        widget::ptr focused_ptr = focused_widget_.lock();

                        if (new_hover_ptr != hover_ptr) 
                        {
                            // NOTE: only leaf widgets can be hover widgets!!!
                            if (hover_ptr) 
                                hover_ptr->handle_event(event::create(event::mouse_leave));
                            hover_ptr = new_hover_ptr;
                            if (hover_ptr) 
                                hover_ptr->handle_event(event::create(event::mouse_enter));
                        } 

                        // update focused widget as necessary
                        // NOTE:: boost docs discourage temporary smart pointers (event::create) as
                        // arguments, in this case they are safe since they are the only argument
                        if (event_->type() == event::mouse_down && 
                                hover_ptr && focused_ptr != hover_ptr)
                        {
                            if (focused_ptr) 
                                focused_ptr->handle_event(event::create(event::un_focus));
                            focused_ptr = hover_ptr;
                            if (hover_ptr)
                                hover_ptr->handle_event(event::create(event::focus));
                        } 

                        // assign the weak pointers from the temporary shared pointers
                        hover_widget_ = hover_ptr;
                        focused_widget_ = focused_ptr;
                    }
                    else // it isn't a mouse event
                    {
                        INFO("passing event to focused_widget_");
                        widget::ptr focused_widget = focused_widget_.lock();
                        if (!focused_widget)
                        {
                            WARN("no current widget, pass to state ?");
                        }
                        else
                        {
                            focused_widget->handle_event(event_);
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

            // redraw the necessary widgets / regions
            try
            {
                stk::state::ptr current_state_ptr = current_state_.lock();
                if (current_state_ptr)
                {    
                    // FIXME: why do we need t_rect at all?
                    rectangle t_rect = redraw_rect;
                    if (!t_rect.empty())
                    {
                        INFO("applicaiton::run() - redrawing state\n\tWith Cliprect " << t_rect);
                        on_predraw(t_rect);

                        current_state_ptr->draw(surface_, t_rect);                                                
                        redraw_rect.width(0);
                        redraw_rect.height(0);
                        assert(redraw_rect.empty()); // FIXME: what does this accomplish? of course it's empty.

                        on_postdraw(t_rect);
                        surface_->update(t_rect);
                    }
                    else
                    {
                        //INFO("nothing to redraw");
                    }
                }
                else
                {
                    // FIXME: throw something
                    WARN("Current State is invalid");
                }
            }
            catch(std::exception &excp)
            {
                ERROR("Exception " << excp.what() << " while redrawing");
            }
            catch(...)
            {
                ERROR("Unknown exception while redrawing");
            } 

            // FIXME: should we try and get 60 fps by using a timer around the above routines
            // and sleeping the difference?
            usleep(1000); // 1 ms
        }
        INFO("Done");
        return retval;
    }

    void application::quit() { done_ = true; }
    void application::add_state(state::ptr state) { states_.push_back(state); }
    void application::add_timer(timer::ptr timer) { timers_.push_back(timer); }

    // drawable interface
    surface::ptr application::surface() { return surface_; }

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
        try
        {
            INFO("Switching states");
            // FIXME...
            ///\todo check if this state is null and if it exists in our list, else through exception, return false,  or add it
            if(!new_cur_state)
            {
                ERROR("Target State Invalid");
                return;
            }

            // remove focus from the current widget
            if (focused_widget_.lock())
                focused_widget_.lock()->handle_event(event::create(event::un_focus));

            INFO("Current widget unfocused");

            current_state_ = new_cur_state;
            INFO("State Switched");

            // focus the first focusable widget of the new current state
            focused_widget_ = current_state_.lock()->focus_next();
            if (focused_widget_.lock())
                focused_widget_.lock()->handle_event(event::create(event::focus));
            else
                // FIXME: throw something
                ERROR("application::run() - current state has no focusable widgets");

            new_cur_state->redraw(new_cur_state->rect());
        }
        catch(...)
        {
            ERROR("Unknown Exception");
        }
    }

    state::ptr application::current_state() const
    {
        return current_state_.lock();
    }
    void application::redraw(const rectangle& rect, drawable* source, bool transform)
    {
        if((source!=current_state_.lock().get()) && source)
            return;
        INFO("Application redraw");
        redraw_rect+=rect;
    }
}



