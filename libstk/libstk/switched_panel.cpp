/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: A container of "tabs"
 *     AUTHORS: Marc Strämke
 *  START DATE: 2003/Sep/11
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available
 *              with the source as license.txt or at 
 *              http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/
#include <libstk/switched_panel.h>
#include <libstk/widget.h>

namespace stk
{
    switched_panel::ptr switched_panel::create(stk::container::ptr parent, rectangle rect)
    {
        switched_panel::ptr new_switched_panel(new switched_panel(rect));
        new_switched_panel->parent(parent);
        return new_switched_panel;
    }

    switched_panel::switched_panel(rectangle rect) : container(rect)
    {
        INFO("constructor");
        focusable(true);
    }
    
    void switched_panel::active_child(widget::ptr child) throw(error_no_child)
    {
        INFO("Switching active child of switched panel");
        active_widget = child;
        component::ptr parent_ptr = parent_.lock();
        if (parent_ptr)
            parent_ptr->redraw(rect());
    }
        
    widget::ptr switched_panel::widget_at(int x, int y)
    {
        widget::ptr active_widget_ptr = active_widget.lock();
        if (active_widget_ptr)
        {
            return active_widget_ptr->widget_at(x-rect_.x1(), y-rect_.y1());
        }
        return widget::ptr();
    } 
    widget::ptr switched_panel::delegate_mouse_event(mouse_event::ptr me)
    {
        widget::ptr active_widget_ptr = active_widget.lock();
        if (active_widget_ptr)
        {
            mouse_event::ptr me_to_child(new mouse_event(me->x()-rect_.x1(), me->y()-rect_.y1(),
                        me->button(), me->type()));
            return active_widget_ptr->delegate_mouse_event(me_to_child);
        }
        return widget::ptr();
    }

    void switched_panel::draw(surface::ptr surface, const rectangle& clip_rect)
    {
        widget::ptr active_widget_ptr = active_widget.lock();
        if (active_widget_ptr)
        {
            rectangle redraw_rect = clip_rect;
            // Intersection tests should be in local coordinates
            redraw_rect.position(redraw_rect.position()-position());
            redraw_rect = redraw_rect.intersection(rect());
            
            surface->offset(surface->offset() + active_widget_ptr->position());
            active_widget_ptr->draw(surface, clip_rect);
            surface->offset(surface->offset() - active_widget_ptr->position());
        }
    }

    void switched_panel::redraw(const rectangle& rect, drawable* source, bool transform)
    {
        if (source != active_widget.lock().get() && source)
            return;
        // MSTR: Broken, Rect is in "local" coordinate space, potentially fixed, see below
        rectangle redraw_rect = rect;
        if (transform)
            redraw_rect.position(redraw_rect.position()+position());
        parent_.lock()->redraw(redraw_rect, this, true);
    }
}
