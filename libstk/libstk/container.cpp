/******************************************************************************
 *    FILENAME: container.cpp
 * DESCRIPTION: Container abstract base class implementation.
 *     AUTHORS: Dirk Hoerner, Marc Straemke, Darren Hart
 *  START DATE: 17/Sep/2002  LAST UPDATE: 14/May/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available
 *              with the source as license.txt or at 
 *              http://www.libstk.org/index.php?page=docs/license
 *****************************************************************************/

#include <iostream>
#include <boost/weak_ptr.hpp>

#include "libstk/component.h"
#include "libstk/widget.h"
#include "libstk/container.h"
#include "libstk/exceptions.h"
#include "libstk/mouse_event.h"

using std::cout;
using std::endl;

namespace stk
{
	container::container(component::ptr parent, const rectangle& rect) : 
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
	
	void container::add(widget::ptr w)
	{ 
		children_.push_back(w); 
	}
	
	void container::remove(widget::ptr item)
	{
		std::vector<boost::shared_ptr<stk::widget> >::iterator iter;
		iter=std::find(children_.begin(), children_.end(), item);
		children_.erase(iter);
	}

	void container::draw(surface::ptr surface, const rectangle& clip_rect)
	{
		// FIXME: do something with clip_rect
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

		widget::handle_event(e);
		
		// FIXME: only pass it up if we haven't handled it
		// if we don't handle it, give it to the parent
		parent_.lock()->handle_event(e);
	}

	widget::ptr container::focus_next()
	{
		cout << "container::focus_next()" << endl;
		// walk through the children, find the focused, and return the next
		// return an empty pointer if we reach the end. 
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
		return widget::ptr();
	}
	
	widget::ptr container::focus_prev()
	{
		cout << "container::focus_prev()" << endl;
		// walk through the children, find the focused, and return the prev
		// return an empty pointer if we reach the beginning. 
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
		return widget::ptr();
	}

	bool container::focused()
	{
		cout << "container::focused()" << endl;
		
		std::vector<widget::ptr>::iterator iter = children_.begin();
		for (iter; iter != children_.end(); iter++)
		{
			if ((*iter)->focused()) return true;
		}
		return false;
	}
	
}
