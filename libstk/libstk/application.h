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

	/// Application class, Runs the Mainloop, dispatches events etc
	class application : public parent
	{
		private:
			boost::shared_ptr<stk::surface> surface_;
			boost::shared_ptr<stk::event_system> event_system_;
			std::list<boost::shared_ptr<stk::state> > states_;
			
			boost::weak_ptr<stk::state> current_state_; 
			boost::weak_ptr<stk::widget> current_widget_;
			boost::weak_ptr<stk::widget> hover_widget_; 
			
			bool done_;
			
		protected:
			
		public:
			/// Constructs a new Application Object
			/// \param surface Target surface for drawing 
			/// \param event_system Source for events
			application(boost::shared_ptr<stk::surface> surface, 
			            boost::shared_ptr<stk::event_system> event_system);
			~application();

			/// Runs The Application, returns when the application is shut down
			int run();
			void quit();
			
			/// Adds a new state to the Application
			void add_state(boost::shared_ptr<stk::state>);
			
			/// Returns the Target surface for drawing operations
			boost::shared_ptr<stk::surface> surface();

			// event_handler interface
			virtual void handle_event(boost::shared_ptr<stk::event> e);
			
			/// parent interface - is this necessary ? 
			/// \todo Carter: I think it is, have to talk about this one
			virtual boost::shared_ptr<widget> focus_next();
			virtual boost::shared_ptr<widget> focus_prev();
			/// \todo Maybe this one should throw an exception ?
			void add_child(boost::shared_ptr<widget>);
			
			// do we want to make these available?
			// or should they be strictly internal to application ? 
			/// \todo might be better to make them property's with only public getters
			boost::weak_ptr<state> current_state();
			boost::weak_ptr<widget> current_widget();
			boost::weak_ptr<state> hover_widget();
			
	}; // class application
	
	typedef boost::shared_ptr<application> Application;

} // namespace stk

#endif
