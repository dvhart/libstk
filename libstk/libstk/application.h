#ifndef STK_APP_H
#define STK_APP_H

#include <list>
#include <boost/smart_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include "parent.h"
#include "surface.h"
#include "state.h"
#include "widget.h"
#include "event_system.h"

namespace stk
{
	class application : public parent
	{
		private:
			boost::shared_ptr<stk::surface> surface_;
			boost::shared_ptr<stk::event_system> event_system_;
			stl::list<boost::smart_ptr<stk::state>> states_;
			
			boost::weak_ptr<stk::state> current_state_; 
			boost::weak_ptr<stk::widget> current_widget_;
			boost::weak_ptr<stk::state> hover_widget_; 
			
			bool done_;
			
		protected:
			
		public:
			application(boost::shared_ptr<stk::surface>, 
			            boost::shared_ptr<stk::event_system>);
			~application();

			int run();
			void quit();
			
			// do we want to make these available?
			// or should they be strictly internal to application ? 
			boost::weak_ptr<state> current_state();
			boost::weak_ptr<widget> current_widget();
			boost::weak_ptr<state> hover_widget();
			
	}; // class application
} // namespace stk

#endif
