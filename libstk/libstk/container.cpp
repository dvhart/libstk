#include <iostream>
#include <boost/weak_ptr.hpp>

#include "widget.h"
#include "container.h"
#include "exceptions.h"
#include "mouse_event.h"

using std::cout;
using std::endl;

namespace stk
{

	container::container(boost::shared_ptr<container> parent) : widget(parent)
	{
		cout << "container::container(container)" << endl;
	}
	
	container::container(boost::shared_ptr<parent> parent) : widget(parent)
	{
		cout << "container::container(parent)" << endl;
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
		cout << "widget_at not found, returning an empty shared_ptr" << endl;
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
	
	void container::add(boost::shared_ptr<widget> item)
	{
		children_.push_back(item);
	}

	void container::remove(boost::shared_ptr<widget> item)
	{
		std::vector<boost::shared_ptr<stk::widget> >::iterator iter;
		iter=std::find(children_.begin(), children_.end(), item);
		children_.erase(iter);
	}

	void container::draw(boost::shared_ptr<stk::surface> surface)
	{
		std::vector<widget::ptr>::iterator iter = children_.begin();
		for (iter; iter != children_.end(); iter++)
		{
			if ((*iter)->intersects(redraw_rect_)) (*iter)->draw(surface);
		}
		redraw_ = false;
		redraw_rect_ = rectangle();
	}

	void container::redraw(bool val, const rectangle& rect)
	{
		redraw_ = val;
		redraw_rect_ += rect;
		// augment redraw_rect_ if it intersects any other children_
		std::vector<widget::ptr>::iterator iter = children_.begin();
		for (iter; iter != children_.end(); iter++)
		{
			if ((*iter)->intersects(redraw_rect_)) redraw_rect_ += (*iter)->rect();
		}
		make_shared(parent_)->redraw(true, redraw_rect_);
	}
	
	void container::handle_event(event::ptr e)
	{
		// handle standard container events
		// ... FIXME: writeme

		// if we don't handle it, give it to the parent
		boost::make_shared(parent_)->handle_event(e);
	}

}
