/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Widget abstract base class implementation.
 *     AUTHORS: Darren Hart, Marc Strämke, Dirk Hörner
 *  START DATE: 2003/Sep/08
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <string>

#include "libstk/application.h"
#include "libstk/component.h"
#include "libstk/container.h"
#include "libstk/key_event.h"
#include "libstk/mouse_event.h"
#include "libstk/widget.h"

namespace stk
{

    widget::widget(const rectangle& rect) : rect_(rect), focusable_(false), frame_(0), 
    pressed_(false), focused_(false), hover_(false)
    {
        INFO("constructor");
    }

    widget::~widget()
    {
        INFO("destructor");
    }

    // wrap the rectangle interface, emitting on_resize as appropriate
    void widget::rect(const rectangle& rect) 
    { 
        if (rect_ != rect)
        {
            rect_ = rect;
            on_resize();
        }
    }
    void widget::width(int val)
    {
        if (width() != val)
        {
            rect_.width(val);
            on_resize();
        }
    }
    void widget::height(int val)
    {
        if (height() != val)
        {
            rect_.height(val);
            on_resize();
        }
    }
    void widget::position(const point& p)
    {
        if (rect_.p1() != p)
        {
            rect_.position(p);
            on_resize();
        }
    }

    /// Handle common widget events
    /// Try to handle the passed event, if there is nothing to do with it pass it up to the parent.
    /// Every non container should call widget::handle_event(e) from within its handle_event
    /// routine if it hasn't handled the event on its own. 
    /// (or container::handle_event(e) which does just that)
    void widget::handle_event(event::ptr e)
    {
        // default handlers for focus/un_focus mouse_enter/leave events
        // a derived widget should call widget::handle_event() for these basic events
        switch (e->type())
        {
        case event::focus:
            if (focusable_)
            {
                focused_ = true;
                redraw(rect());
            }
            return;
            break;
        case event::un_focus:
            if (focusable_)
            {
                focused_ = false;
                pressed_ = false;
                redraw(rect());
            }
            return;
            break;
        case event::mouse_enter:
            hover_ = true;
            redraw(rect());
            return;
            break;
        case event::mouse_leave:
            hover_ = false;
            pressed_ = false;
            redraw(rect());
            return;
            break;
        case event::key_up:
        {
            key_event::ptr ke = boost::shared_static_cast<key_event>(e);
            if (on_keyup(ke->fn_key())) return;
            break;
        }
        case event::mouse_motion:
        case event::mouse_up:
        case event::mouse_down:
        {
            // if this is a mouse event, translate the coordinates back relative to the parent
            mouse_event::ptr me = boost::shared_static_cast<mouse_event>(e);
            widget::ptr widget_parent = boost::shared_dynamic_cast<widget>(parent_.lock());
            if (widget_parent)
            {
                me->x(me->x() + widget_parent->position().x());
                me->y(me->y() + widget_parent->position().y());
            }
            // this is handed up to the parent below
        }
        }

        parent_.lock()->handle_event(e);
    }

    // drawable interface
    surface::ptr widget::surface()
    {
        return parent_.lock()->surface();
    }

    void widget::redraw(const rectangle& rect, drawable* source, bool transform)
    {
        // FIXME: what is the benefit of copying the rectangle?
        rectangle redraw_rect(rect);
        parent_.lock()->redraw(redraw_rect, this, true);
    }

    void widget::parent(container::ptr parent) 
    { 
        if (parent_.lock()) parent_.lock()->remove(shared_from_this());
        parent_ = parent;
        if (parent_.lock()) parent_.lock()->add(shared_from_this());
    }

    widget::ptr widget::widget_at(int x, int y)
    {
        if (rect_.contains(x, y))
            return shared_from_this();
        else 
            return widget::ptr();
    }

    widget::ptr widget::delegate_mouse_event(mouse_event::ptr me)
    {
	handle_event(me);
	return shared_from_this();
    }
    
}
