/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Container abstract base class implementation.
 *     AUTHORS: Dirk Hörner, Marc Strämke, Darren Hart
 *  START DATE: 2002/Sep/17
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include <iostream>
#include <algorithm>
#include <boost/weak_ptr.hpp>
#include <boost/bind.hpp>

#include "libstk/component.h"
#include "libstk/widget.h"
#include "libstk/container.h"
#include "libstk/exceptions.h"
#include "libstk/mouse_event.h"

namespace stk
{
    container::container(const rectangle& rect) : widget(rect)
    {
        INFO("constructor");
        focusable_ = false;
    }

    container::~container()
    {
        INFO("destructor");
    }

    widget::ptr container::widget_at(int x, int y)
    {
        x -= x1();
        y -= y1();

        // if we don't contain this point, return a null pointer
        if (x >= width() || y >= height()) return widget::ptr();

        // cycle through the widgets, and return the first that doesn't return a null pointer
        std::vector<widget::ptr>::iterator iter = children_.begin();
        for (; iter != children_.end(); iter++)
            if ((*iter)->widget_at(x, y)) return *iter;

        // no children are at this location, return this
        return shared_from_this();
    }

    widget::ptr container::delegate_mouse_event(mouse_event::ptr me)
    {
        // pass a mouse event to the appropriate widget
        std::vector<widget::ptr>::iterator iter = children_.begin();
        for (; iter != children_.end(); iter++)
        {
            if ((*iter)->contains(me->x(), me->y()))
            {
                mouse_event::ptr me_to_child(new mouse_event(me->x()-x1(), me->y()-y1(), 
                            me->button(), me->type()));
                return (*iter)->delegate_mouse_event(me_to_child);
            }
        }
        return widget::ptr();
    }

    void container::add(widget::ptr w)
    {
        children_.push_back(w);
    }

    void container::remove(widget::ptr item)
    {
        std::vector<boost::shared_ptr<stk::widget> >::iterator iter;
        iter = std::find(children_.begin(), children_.end(), item);
        if(iter!=children_.end())
            children_.erase(iter);
        else
            WARN("Cannot remove item from container, it is not a child!");
    }

    void container::draw_child(surface::ptr surface, const rectangle& clip_rect, widget::ptr child)
    {
        if (!child->intersects(clip_rect)) return;

        // calculate the child redraw rect
        rectangle redraw_rect = child->intersection(clip_rect);
        redraw_rect.position(redraw_rect.position()-child->position());         

        // draw the child only if necessary
        if (!redraw_rect.empty()) 
        {
            // set up the surface offset and clip_rect for drawing the child
            surface->offset(surface->offset()+child->position());
            surface->clip_rect(redraw_rect);
            // draw the child
            child->draw(surface, redraw_rect);
            // restore the surface offset and clip_rect
            surface->offset(surface->offset()-child->position());
            surface->clip_rect(clip_rect);
        }
    }
    
    void container::draw(surface::ptr surface, const rectangle& clip_rect)
    {
        std::for_each(children_.begin(), children_.end(), 
                boost::bind(&container::draw_child, this, surface, clip_rect, _1));
    }

    void container::redraw(const rectangle& rect, drawable* source, bool transform)
    {
        // MSTR: Broken, Rect is in "local" coordinate space, potentially fixed, see below
        rectangle redraw_rect = rect;
        if (transform)
            redraw_rect.position(redraw_rect.position()+position());
        if (parent_.lock())
            parent_.lock()->redraw(redraw_rect, this, true);
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
        INFO("container::focus_next()");
        // walk through the children, find the focused, and return the next
        // return an empty pointer if we reach the end.
        std::vector<widget::ptr>::iterator iter = children_.begin();
        for (; iter != children_.end(); iter++)
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
        INFO("container::focus_prev()");
        // walk through the children, find the focused, and return the prev
        // return an empty pointer if we reach the beginning.
        std::vector<widget::ptr>::iterator iter = children_.begin();
        for (; iter != children_.end(); iter++)
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
        INFO("container::focused()");
        // check ourself
        if (focused_) return true;
        // check our children (return true if one of our children is focused)
        std::vector<widget::ptr>::iterator iter = children_.begin();
        for (; iter != children_.end(); iter++) if ((*iter)->focused()) return true;
        return false;
    }

}
