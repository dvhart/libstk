#include <iostream>
#include <boost/weak_ptr.hpp>

#include "libstk/widget.h"
#include "libstk/container.h"
#include "libstk/exceptions.h"
#include "libstk/mouse_event.h"

using std::cout;
using std::endl;

namespace stk
{

	container::container(boost::shared_ptr<container> parent, const rectangle& rect) : 
		widget(parent, rect)
	{
		cout << "container::container(container)" << endl;
		focusable_ = false;
		redraw_rect_ = parent->surface()->rect();
	}
	
	container::container(boost::shared_ptr<parent> parent, const rectangle& rect) : 
		widget(parent, rect)
	{
		cout << "container::container(parent)" << endl;
		focusable_ = false;
		redraw_rect_ = parent->surface()->rect();
	}

	container::~container()
	{}

	widget::ptr container::widget_at(int x, int y)
	{ 
		std::vector<widget::ptr>::iterator iter = children_.begin();
		for (iter; iter != children_.end(); iter++)
		{
			if ((*iter)->contains(x, y)) return *iter;
		}
		//cout << "widget_at not found, returning an empty shared_ptr" << endl;
		return widget::ptr();
		//return container::ptr(this); // FIXME: why does this cause a segfault
	}
	
	void container::delegate_mouse_event(mouse_event::ptr me)
	{
		// pass a mouse event to the appropriate widget
		std::vector<widget::ptr>::iterator iter = children_.begin();
		for (iter; iter != children_.end(); iter++)
		{
			if ((*iter)->contains(me->x(), me->y()))
			{
				(*iter)->handle_event(me);
			}
		}
	}
	
	void container::add(widget::ptr item)
	{
		children_.push_back(item);
	}

	void container::remove(widget::ptr item)
	{
		std::vector<boost::shared_ptr<stk::widget> >::iterator iter;
		iter=std::find(children_.begin(), children_.end(), item);
		children_.erase(iter);
	}

	void container::draw(surface::ptr surface)
	{
		std::vector<widget::ptr>::iterator iter = children_.begin();
		rectangle temp_rect = redraw_rect_;
		redraw_rect_ = rectangle();
		for (iter; iter != children_.end(); iter++)
		{
			if ((*iter)->intersects(temp_rect)) (*iter)->draw(surface);
		}
	}

	void container::redraw(const rectangle& rect)
	{
		redraw_rect_ += rect;
		// augment redraw_rect_ if it intersects any other children_
		std::vector<widget::ptr>::iterator iter = children_.begin();
		for (iter; iter != children_.end(); iter++)
		{
			if ((*iter)->intersects(redraw_rect_)) redraw_rect_ += (*iter)->rect();
		}
		parent_.lock()->redraw(redraw_rect_);
	}
	
	// FIXME: decide how event handling gets done
	// when a widget gets an event, it can:
	//   handle the event
	//   ask its base class to handle the event
	//   pass the event to its parent
	// should it do all three?
	// the base class and the widget can't both pass to the parent, or we will 
	//   send the event twice
	void container::handle_event(event::ptr e)
	{
		// handle standard container events
		// ... FIXME: writeme

		// if we don't handle it, give it to the parent
		//parent_.lock()->handle_event(e);
	}

	widget::ptr container::focus_next()
	{
		cout << "container::focus_next()" << endl;
		// walk through the children, find the focused, and return the next
		// call parent if it is the last one
		std::vector<widget::ptr>::iterator iter = children_.begin();
		for (iter; iter != children_.end(); iter++)
		{
			if ((*iter)->focused())
			{
				// find the next focusable widget
				while (++iter != children_.end() )
				{
					if ((*iter)->focusable())
					{
						return *iter;
					}
				}
				break;
			}
		}
		return parent_.lock()->focus_next();
	}
	
	widget::ptr container::focus_prev()
	{
		cout << "container::focus_prev()" << endl;
		// walk through the children, find the focused, and return the prev
		// call parent if it is the first one
		std::vector<widget::ptr>::iterator iter = children_.begin();
		for (iter; iter != children_.end(); iter++)
		{
			if ((*iter)->focused())
			{
				// find the previous focusable widget
				while (iter != children_.begin())
				{
					if ((*--iter)->focusable())
					{
						return *iter;
					}
				}
				break;
			}
		}
		return parent_.lock()->focus_prev();
	}

}
