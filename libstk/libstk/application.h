/******************************************************************************
 *    FILENAME: application.h
 * DESCRIPTION: Libstk application, handles events, timers, and widget drawing.
 *     AUTHORS: Darren Hart, Marc Straemke
 *  START DATE: 23/Feb/2003  LAST UPDATE: 09/Jun/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available
 *              with the source as license.txt or at 
 *              http://www.libstk.org/index.php?page=docs/license
 *****************************************************************************/

#ifndef STK_APPLICATION_H
#define STK_APPLICATION_H

#include <list>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

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

		private:
			surface::ptr surface_;
			event_system::ptr event_system_;
			std::list<boost::shared_ptr<state> > states_;
			std::list<boost::shared_ptr<timer> > timers_;
			
			// FIXME: what can we do to use state::weak_ptr
			boost::weak_ptr<state> current_state_;  // FIXME: should these be a weak_ptr ?
			widget::weak_ptr focused_widget_;
			widget::weak_ptr hover_widget_; 
			
			bool done_;
			
		protected:
			application(surface::ptr surface);

			
		public:
			/// Constructs a new Application Object
			/// \param surface Target surface for drawing 
			/// \param event_system Source for events
			static application::ptr create(surface::ptr surface);
			~application();

			/// Runs The Application, returns when the application is shut down
			int run();
			bool quit(); // returns bool so it can use it as a slot
			
			/// Adds a new state to the Application
			void add_state(boost::shared_ptr<state>);
			
			/// Adds a new timer to the Application
			void add_timer(timer::ptr timer);
			///todo: writeme
			void del_timer(timer::ptr timer) { }
			
			/// Returns the surface for drawing operations
			virtual surface::ptr surface();

			// event_handler interface
			virtual void handle_event(event::ptr e);
			
			virtual widget::ptr focus_next();
			virtual widget::ptr focus_prev();
			
	}; // class application
} // namespace stk

#endif
