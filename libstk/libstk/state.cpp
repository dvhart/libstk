#include <boost/weak_ptr.hpp>

#include "state.h"
#include "app.h"

namespace stk
{
	
state::state(boost::weak_ptr<app> parent):parent_(parent) , container(boost::weak_ptr<container>())
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
