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
		public:
			state(boost::weak_ptr<stk::application> parent);
			virtual ~state();
			boost::weak_ptr<widget> focused_widget();
			void focused_widget(boost::weak_ptr<widget> value);

		private:
			boost::weak_ptr<widget> focused_widget_;
			boost::weak_ptr<stk::application> parent_;
	};

	typedef boost::shared_ptr<state> State;
	
}

#endif
