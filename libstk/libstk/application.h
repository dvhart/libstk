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
			application(boost::shared_ptr<stk::surface>, 
			            boost::shared_ptr<stk::event_system>);
			~application();

			int run();
			void quit();
			
			void add_state(boost::shared_ptr<stk::state>);
			
			// drawable interface
			boost::shared_ptr<stk::surface> surface();

			// event_handler interface
			virtual void handle_event(boost::shared_ptr<stk::event> e);
			
			// parent interface - is this necessary ?
			virtual boost::shared_ptr<widget> focus_next();
			virtual boost::shared_ptr<widget> focus_prev();
			void add_child(boost::shared_ptr<widget>);
			
			// do we want to make these available?
			// or should they be strictly internal to application ? 
			boost::weak_ptr<state> current_state();
			boost::weak_ptr<widget> current_widget();
			boost::weak_ptr<state> hover_widget();
			
	}; // class application
	
	typedef boost::shared_ptr<application> Application;

} // namespace stk

#endif
