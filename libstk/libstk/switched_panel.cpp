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
    switched_panel::switched_panel(container::ptr parent, rectangle rect)
        : container(parent, rect)
    {
        INFO("constructor");
        focusable(true);
    }
    
    void switched_panel::active_child(widget::ptr child) throw(error_no_child)
    {
        INFO("Switching active child of switched panel");
        active_widget=child;
        component::ptr parent_ptr=parent_.lock();
        if(parent_ptr)
            parent_ptr->redraw(rect());
    }
    
    switched_panel::ptr switched_panel::create(stk::container::ptr parent, rectangle rect)
    {
        switched_panel::ptr ptr(new switched_panel(parent, rect));
        parent->add(ptr);
        return ptr;
    }
    
    widget::ptr switched_panel::widget_at(int x, int y)
    {
        widget::ptr active_widget_ptr=active_widget.lock();
        if(active_widget_ptr)
        {
            return active_widget_ptr->widget_at(x-rect().x1(),y-rect().y1());
        }
        return widget::ptr();
    } 
    widget::ptr switched_panel::delegate_mouse_event(mouse_event::ptr me)
    {
        widget::ptr active_widget_ptr=active_widget.lock();
        if(active_widget_ptr)
        {
            mouse_event::ptr me_to_child(new mouse_event(me->x()-rect().x1(),me->y()-rect().y1(),
                                                         me->button(),me->type()));
            return active_widget_ptr->delegate_mouse_event(me_to_child);
        }
        return widget::ptr();
    }

    void switched_panel::draw(surface::ptr surface, const rectangle& clip_rect)
    {
        widget::ptr active_widget_ptr=active_widget.lock();
        if(active_widget_ptr)
        {
            rectangle redraw_rect = clip_rect;
            // Intersection tests should be in local coordinates
            redraw_rect.position(redraw_rect.position()-surface->offset());
            redraw_rect=redraw_rect.intersection(rect());
            
            surface->offset(surface->offset() + active_widget_ptr->rect().p1());
            active_widget_ptr->draw(surface,clip_rect);
            surface->offset(surface->offset() - active_widget_ptr->rect().p1());
        }
    }

}
