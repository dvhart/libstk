#include <boost/weak_ptr.hpp>

#include "state.h"
#include "application.h"

namespace stk
{

	//state::state(boost::weak_ptr<application> parent) : parent_(parent)
	state::state(boost::shared_ptr<application> parent) : parent_(parent)
	{
	}

	state::~state()
	{
	}

	boost::weak_ptr<widget> state::focused_widget()
	{
		return focused_widget_;
	}

	void state::focused_widget(boost::weak_ptr<widget> value)
	{
		focused_widget_=value;
	}
}
