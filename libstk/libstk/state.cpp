#include <iostream>
#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include "state.h"
#include "container.h"
#include "application.h"
#include "theme.h"

using std::cout;
using std::endl;

namespace stk
{
	
	state::ptr state::create(application::ptr parent)
	{
		state::ptr new_state(new state(parent));
		parent->add_state(new_state);
		return new_state;
	}

	state::state(boost::shared_ptr<application> parent) : 
		container(boost::shared_static_cast<stk::parent>(parent), parent->surface()->rect())
	{
		cout << "state::state()" << endl;
	}

	state::~state()
	{
	}
	
	void state::draw(surface::ptr surface)
	{
		//cout << "state::draw() - redraw_rect_: " << redraw_rect_ << endl;
		theme::instance()->draw_state(redraw_rect_, focused_);
		container::draw(surface);
	}
	
	// event_handler interface - default back to parent
	void state::handle_event(event::ptr e)
	{
		//cout << "state::handle_event()" << endl;
		// handle appropriate events here FIXME
		// ...

		container::handle_event(e); 

		// if we don't handle it, pass up to the parent
		// mstr: broken in Boost_1_30_0 and in general! FIXME
		// FIXME: only run if not handled ???
		if (!parent_.lock())
		{ 
			// throw something
			cout << "state::handle_event() - null parent_ pointer" << endl;
		}
		parent_.lock()->handle_event(e); 
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


