#ifndef STK_APP_H
#define STK_APP_H

#include <list>
#include <boost/smart_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include "parent.h"
#include "surface.h"
#include "state.h"
#include "widget.h"

namespace stk
{
	class application : public parent
	{
		private:
			boost::smart_ptr<stk::surface> surface_;
			stl::list<boost::smart_ptr<stk::state>> states_;
			boost::weak_ptr<stk::state> current_state_; // why weak?
			boost::weak_ptr<stk::widget> current_widget_; // why weak?
			boost::weak_ptr<stk::state> hover_widget_; // why weak?
			
			
		protected:
			
		public:
			application( /* what parameters */ );
			~application();

			void run();
			
			// weak_ptrs?
			boost::weak_ptr<state> current_state();
			boost::weak_ptr<widget> current_widget();
			boost::weak_ptr<state> hover_widget();
			
	}; // class application
} // namespace stk

#endif
