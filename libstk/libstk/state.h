#ifndef STATE_H
#define STATE_H

#include <boost/weak_ptr.hpp>
#include "widget.h"
#include "container.h"
#include "application.h"

namespace stk
{
	class state : public stk::container
	{
		private:

		protected:
			boost::weak_ptr<widget> focused_widget_;
			
		public:
			state(boost::shared_ptr<stk::application> parent);
			virtual ~state();
			boost::weak_ptr<widget> focused_widget();
			void focused_widget(boost::weak_ptr<widget> value);

			// event_handler interface
			virtual void handle_event(boost::shared_ptr<stk::event> e);

	};

	typedef boost::shared_ptr<state> State;
	
}

#endif
