#ifndef STK_APP_H
#define STK_APP_H

#include <list>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include "parent.h"
#include "surface.h"
#include "widget.h"
#include "event_system.h"

namespace stk
{
	class state;
	//class application;
	/// Application class, Runs the Mainloop, dispatches events etc
	class application : public parent
	{
		public:
			typedef boost::shared_ptr<application> ptr;
			typedef boost::weak_ptr<application> weak_ptr;
		private:
			boost::shared_ptr<stk::surface> surface_;
			boost::shared_ptr<stk::event_system> event_system_;
			std::list<boost::shared_ptr<stk::state> > states_;
			
			boost::weak_ptr<state> current_state_; 
			widget::weak_ptr current_widget_;
			widget::weak_ptr hover_widget_; 
			
			bool done_;
			
		protected:
			
		public:
			/// Constructs a new Application Object
			/// \param surface Target surface for drawing 
			/// \param event_system Source for events
			application(surface::ptr surface, 
			            event_system::ptr event_system);
			~application();

			/// Runs The Application, returns when the application is shut down
			int run();
			bool quit(); // bool is so we can use it as a slot
			
			/// Adds a new state to the Application
			void add_state(boost::shared_ptr<state>);
			
			/// Returns the Target surface for drawing operations
			surface::ptr surface();

			// event_handler interface
			virtual void handle_event(event::ptr e);
			
			/// parent interface - is this necessary ? 
			/// \todo Carter: I think it is, have to talk about this one
			virtual widget::ptr focus_next();
			virtual widget::ptr focus_prev();
			/// \todo Maybe this one should throw an exception ?
			void add_child(widget::ptr);
			
			// do we want to make these available?
			// or should they be strictly internal to application ? 
			boost::weak_ptr<state> current_state();
			widget::weak_ptr current_widget();
			widget::weak_ptr hover_widget();
			
	}; // class application
	

} // namespace stk

#endif
