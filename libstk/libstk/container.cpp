/**************************************************************************************************
 *    FILENAME: container.cpp
 * DESCRIPTION: Container abstract base class implementation.
 *     AUTHORS: Dirk Hoerner, Marc Straemke, Darren Hart
 *  START DATE: 17/Sep/2002  LAST UPDATE: 13/Jul/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

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
            if ((*iter)->contains(x, y))
                return *iter;
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

    void container::add
        (widget::ptr w)
    {
        children_.push_back(w);
    }

    void container::remove
        (widget::ptr item)
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
            if ((*iter)->intersects(temp_rect))
                (*iter)->draw(surface);
        }
    }

    void container::redraw(const rectangle& rect)
    {
        redraw_rect_ += rect;
        // augment redraw_rect_ if it intersects any other children_
        std::vector<widget::ptr>::iterator iter = children_.begin();
        for (iter; iter != children_.end(); iter++)
        {
            if ((*iter)->intersects(redraw_rect_))
                redraw_rect_ += (*iter)->rect();
        }
        parent_.lock()->redraw(redraw_rect_);
    }

    /// Handle common container events
    /// Try to handle the passed event, if there is nothing to do with it, ask widget to handle it.
    /// Every container should call container::handle_event(e) from within its handle_event
    /// routine if it hasn't handled the event on its own.
    void container::handle_event(event::ptr e)
    {
        // handle standard container events
        // ... FIXME: writeme

        widget::handle_event(e);
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
                    if ((*iter)->focusable()) return *iter;
                }
                break;
            }
        }
        // there isn't a focused widget, so find the first focusable widget
        iter = children_.begin();
        while (iter != children_.end() )
        {
            if ((*iter)->focusable()) return *iter;
            iter++;
        }
        // no focusable widgets, so return an empty pointer        
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
                    if ((*--iter)->focusable()) return *iter;
                }
                break;
            }
        }
        // there isn't a focused widget, so find the first focusable widget
        iter = children_.begin();
        while (iter != children_.end() )
        {
            if ((*iter)->focusable()) return *iter;
            iter++;
        }
        // no focusable widgets, so return an empty pointer        
        return widget::ptr();
    }

    bool container::focused()
    {
        cout << "container::focused()" << endl;

        std::vector<widget::ptr>::iterator iter = children_.begin();
        for (iter; iter != children_.end(); iter++)
        {
            if ((*iter)->focused())
                return true;
        }
        return false;
    }

}
