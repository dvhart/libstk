#include <iostream>
#include <boost/weak_ptr.hpp>

#include "state.h"
#include "container.h"
#include "application.h"

using std::cout;
using std::endl;

namespace stk
{

	state::state(boost::shared_ptr<application> parent) : container(parent)
	{
		cout << "state::state()" << endl;
		cout << "state::state() - parent pointer is " << std::hex << parent.get() << endl;
		parent->add_state(boost::shared_ptr<state>(this));
	}

	state::~state()
	{
	}
	
	// event_handler interface - default back to parent
	// FIXME: this is currently exactly like widget::handle_event()
	void state::handle_event(boost::shared_ptr<stk::event> e)
	{
		cout << "state::handle_event()" << endl;
		// FIXME: what is the best way to access the weak_ptr parent_
		// should we make it shared first ? (see all other uses torc/te)
		if (parent_.get() == 0)
		{
			// throw something
			cout << "state::handle_event() - null parent_ pointer" << endl;
		}
		parent_.get()->handle_event(e);
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
