#ifndef STATE_H
#define STATE_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "widget.h"
#include "container.h"
#include "application.h"

namespace stk
{
	class state : public stk::container
	{
		public:
			typedef boost::shared_ptr<state> ptr;
			typedef boost::weak_ptr<state> weak_ptr;
			
		private:

		protected:
			state(application::ptr parent);
			boost::weak_ptr<widget> focused_widget_;
			
		public:
			virtual ~state();
			static state::ptr create(application::ptr parent);
			widget::weak_ptr focused_widget();
			void focused_widget(widget::weak_ptr value);

			// event_handler interface
			virtual void handle_event(event::ptr e);

	};

	
}

#endif
