/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Libstk application, handles events, timers, and widget drawing.
 *     AUTHORS: Darren Hart, Marc Strämke
 *  START DATE: 2003/Feb/23
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_APPLICATION_H
#define STK_APPLICATION_H

#include <list>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/thread/mutex.hpp>

#include <libstk/component.h>
#include <libstk/surface.h>
#include <libstk/widget.h>
#include <libstk/event_system.h>
#include <libstk/timer.h>

namespace stk
{
    class state;

    /// Application class, Runs the Mainloop, dispatches events, etc.
    class application : public component
    {
    public:
        typedef boost::shared_ptr<application> ptr;
        typedef boost::weak_ptr<application> weak_ptr;
        typedef std::vector<timer::ptr> Ttimers;
        typedef std::vector<boost::shared_ptr<state> > Tstates;

    protected:
        static application::weak_ptr instance_;
        surface::ptr surface_;
        event_system::ptr event_system_;
        Tstates states_;
        Ttimers timers_;
        Ttimers new_timers_;

        boost::weak_ptr<state> current_state_;  // FIXME: should these be a weak_ptr ?
        widget::weak_ptr focused_widget_;
        widget::weak_ptr hover_widget_;

        rectangle redraw_rect;
        
        bool done_;

    protected:
        application(surface::ptr surface);

    public:
        /// Constructs a new Application Object
        /// This should be called only once near the beginning of every Libstk program.
        /// Throws error_message_exception if called more than once.
        /// \param surface Target surface for drawing
        static application::ptr create(surface::ptr surface);
        /// Return the singleton instance of application.
        /// Throws error_message_exception if called prior to a call to create.
        static application::ptr get();
        virtual ~application();

        /// Runs The Application, returns when the application is shut down
        virtual int run();
        virtual void quit(); 

        /// Adds a new state to the Application
        virtual void add_state(boost::shared_ptr<state>);
        virtual void remove_state(boost::shared_ptr<state>);

        /// Adds a new timer to the Application
        virtual void add_timer(timer::ptr timer);
        /// Removed the timer
        virtual void remove_timer(timer::ptr timer);

        /// Returns the surface for drawing operations
        virtual surface::ptr surface();

        // event_handler interface
        virtual void handle_event(event::ptr e);

        virtual widget::ptr focus_next();
        virtual widget::ptr focus_prev();

        /// Sets the current focused widget
        virtual void focused_widget(widget::ptr new_focused_widget);
        /// returns the current focused widget - returns NULL if none
        virtual widget::ptr focused_widget();

        virtual void current_state(boost::shared_ptr<stk::state> new_cur_state);
        virtual boost::shared_ptr<stk::state> current_state() const;

        virtual void redraw(const rectangle& rect, drawable* source=NULL, bool transform=false);

        // Triggered before STK's drawing code ran  \FIXME what does the return value mean?
        boost::signal<bool (stk::rectangle), combiner::logical_or<bool> > on_predraw;
        // Triggered after STK's drawing code ran    \FIXME what does the return value mean?
        boost::signal<bool (stk::rectangle), combiner::logical_or<bool> > on_postdraw;
        // Triggered before STK receives an event, return true if event was handled in the signal handler
        boost::signal<bool (event::ptr), combiner::logical_or<bool> > on_event;

        // Mutex to lock the mainloop in a multi threaded environment
        boost::mutex mainloop_mutex;
    }; // class application
} // namespace stk

#endif
