#include "libstk/scroll_decorator.h"
#include "libstk/event.h"
#include "libstk/key_event.h"

namespace stk
{
	scroll_decorator::ptr scroll_decorator::create(container::ptr parent, 
			const rectangle& rect)
	{
		scroll_decorator::ptr new_scroll_decorator(
				new scroll_decorator(parent, rect));
		parent->add(new_scroll_decorator);
		return new_scroll_decorator;
	}
	
	scroll_decorator::scroll_decorator(container::ptr parent, 
			const rectangle& rect) : decorator(parent, rect), vrange_(0), 
			hrange_(0), vposition_(0), hposition_(0)
	{
		//focusable_ = true;
	}
			
	scroll_decorator::~scroll_decorator()
	{
	}

	void scroll_decorator::handle_event(event::ptr e)
	{
		cout << "scroll_decorator::handle_event()" << endl;	
		switch (e->type())
		{
			case event::key_up:
			{
				// FIXME: these increments need to update the vposition and hposition
				// and not move the component passed the scrollable ranges.
				key_event::ptr ke = boost::shared_static_cast<key_event>(e);
				switch ( ke->key() )
				{
					case up_arrow:
						cout << "scroll_decorator::handle_event() - scroll up" << endl;
						component_->position(component_->x1(), component_->y1()+1);
						redraw(rect_);
						return;
						break;
					case down_arrow:
						cout << "scroll_decorator::handle_event() - scroll down" << endl;
						component_->position(component_->x1(), component_->y1()-1);
						redraw(rect_);
						return;
						break;
					case left_arrow:
						cout << "scroll_decorator::handle_event() - scroll left" << endl;
						component_->position(component_->x1()+1, component_->y1());
						redraw(rect_);
						return;
						break;
					case right_arrow:
						cout << "scroll_decorator::handle_event() - scroll right" << endl;
						component_->position(component_->x1()-1, component_->y1());
						redraw(rect_);
						return;
						break;
				}
				break; // key_up
			}
		}
		// fixme: do we want to be calling widget::handle_event ?
		widget::handle_event(e); 
		parent_.lock()->handle_event(e);
	}

}

