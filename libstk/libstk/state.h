#ifndef STATE_H
#define STATE_H

#include <boost/weak_ptr.hpp>
#include "widget.h"
#include "container.h"

namespace stk
{
class app;

class state : public container
{
public:
    state(boost::weak_ptr<app> parent);
	boost::weak_ptr<widget> focused_widget();
	void focused_widget(boost::weak_ptr<widget> value);
private:
    boost::weak_ptr<widget> focused_widget_;
	boost::weak_ptr<app> parent_;
};

}

#endif
