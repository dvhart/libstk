#ifndef STK_APP_H
#define STK_APP_H

#include <list>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include "libstk/component.h"
#include "libstk/surface.h"
#include "libstk/widget.h"
#include "libstk/event_system.h"

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
			
			// FIXME: what can we do to use state::weak_ptr
			boost::weak_ptr<state> current_state_;  // FIXME: should these be a weak_ptr ?
			widget::weak_ptr focused_widget_;
			widget::weak_ptr hover_widget_; 
			
			bool done_;
			
		protected:
			application(surface::ptr surface, 
			            event_system::ptr event_system);
			
		public:
			/// Constructs a new Application Object
			/// \param surface Target surface for drawing 
			/// \param event_system Source for events
			static application::ptr create(surface::ptr surface, 
			            event_system::ptr event_system);
			~application();

			/// Runs The Application, returns when the application is shut down
			int run();
			bool quit(); // returns bool so it can use it as a slot
			
			/// Adds a new state to the Application
			void add_state(boost::shared_ptr<state>);
			
			/// Returns the surface for drawing operations
			virtual surface::ptr surface();

			// event_handler interface
			virtual void handle_event(event::ptr e);
			
			virtual widget::ptr focus_next();
			virtual widget::ptr focus_prev();
			
	}; // class application
} // namespace stk

#endif
