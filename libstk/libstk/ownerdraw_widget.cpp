/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Ownerdraw_Widget widget implementation.
 *     AUTHORS: Darren Hart, Vernon Mauery, Marc Strämke 
 *  START DATE: 2003/Mar/03
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include <iostream>
#include <libstk/ownerdraw_widget.h>
#include <libstk/widget.h>

namespace stk
{
    ownerdraw_widget::ptr ownerdraw_widget::create(container::ptr parent, const rectangle& rect)
    {     
        ownerdraw_widget::ptr new_ownerdraw_widget(new ownerdraw_widget(rect));
        new_ownerdraw_widget->parent(parent);
        return new_ownerdraw_widget;
    }

    ownerdraw_widget::ownerdraw_widget(const rectangle& rect) 
	: widget(rect)
    {
        INFO("constructor");
    }

    ownerdraw_widget::~ownerdraw_widget()
    {
        INFO("destructor");
    }
    void ownerdraw_widget::draw(surface::ptr surface, rectangle const& clip_rect)
    {
        if(draw_callback)
            draw_callback(surface, clip_rect);
    }
    void ownerdraw_widget::handle_event(event::ptr e)
    {
        bool handled = false;
        if(event_callback)
            handled = event_callback(e);
        
        // Pass trough!
        if(!handled)
            widget::handle_event(e);
    }
}
