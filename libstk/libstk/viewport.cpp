/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Viewport container implementation.
 *     AUTHORS: Darren Hart, Marc Strämke 
 *  START DATE: 2003/Apr/29
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include <iostream>
#include <algorithm>
#include <boost/bind.hpp>
#include "libstk/viewport.h"
#include "libstk/event.h"
#include "libstk/key_event.h"

namespace stk
{
    viewport::ptr viewport::create(container::ptr parent,
                                   const rectangle& rect)
    {
        viewport::ptr new_viewport(new viewport(parent, rect));
        parent->add
        (new_viewport);
        return new_viewport;
    }

    viewport::viewport(container::ptr parent,
                       const rectangle& rect) : container(parent, rect)
    {
        INFO("constructor");
        focusable_ = true;
        h_scroll(scroll_model::create());
        v_scroll(scroll_model::create());
    }

    viewport::~viewport()
    {
        INFO("destructor");
    }


    scroll_model::ptr viewport::h_scroll()
    {
        return h_scroll_;
    }
    void viewport::h_scroll(scroll_model::ptr value)
    {
        h_scroll_con.disconnect();
        h_scroll_ = value;
        h_scroll_con = value->on_change.connect(boost::bind(&viewport::redraw, this));
        h_scroll_->size(width());
        if (!children_.empty())
            h_scroll_->size(children_[0]->width());
        h_scroll_->vis_size(width());
    }

    scroll_model::ptr viewport::v_scroll()
    {
        return v_scroll_;
    }
    void viewport::v_scroll(scroll_model::ptr value)
    {
        v_scroll_con.disconnect();
        v_scroll_=value;
        v_scroll_con=value->on_change.connect(boost::bind(&viewport::redraw,this));
        v_scroll_->size(height());
        v_scroll_->vis_size(height());
        if (!children_.empty())
            v_scroll_->size( children_[0]->height());
    }
    void viewport::redraw()
    {
        container::redraw(rect());
    }
    void viewport::handle_event(event::ptr e)
    {
        widget::handle_event(e);
    }

    void viewport::draw(surface::ptr surface, const rectangle& clip_rect)
    {
        INFO("clip_rect: " << clip_rect);
        INFO("                surface offset: " << surface->offset().x() << "," << surface->offset().y());
        point scroll_offset(h_scroll()->begin(), v_scroll()->begin());
        surface->offset(surface->offset() + scroll_offset);
        INFO("surface offset + scroll offset: " << surface->offset().x() << "," << surface->offset().y());
        container::draw(surface, rect()); // draw all the children
        surface->offset(surface->offset() - scroll_offset);
    }

}
