/******************************************************************************
 *    FILENAME: viewport.cpp
 * DESCRIPTION: Viewport container implementation.
 *     AUTHORS: Darren Hart, Marc Straemke 
 *  START DATE: 29/Apr/2003  LAST UPDATE: 28/May/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available
 *              with the source as license.txt or at 
 *              http://www.libstk.org/index.php?page=docs/license
 *****************************************************************************/

#include <iostream>
#include <boost/bind.hpp>
#include "libstk/viewport.h"
#include "libstk/event.h"
#include "libstk/key_event.h"

using std::cout;
using std::endl;

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
        cout << "viewport::viewport()" << endl;
        focusable_ = true;
        h_scroll(scroll_model::create());
        v_scroll(scroll_model::create());
    }

    viewport::~viewport()
    {
        cout << "viewport::~viewport()" << endl;
    }


    scroll_model::ptr viewport::h_scroll()
    {
        return h_scroll_;
    }
    void viewport::h_scroll(scroll_model::ptr value)
    {
        h_scroll_con.disconnect();
        h_scroll_ = value;
        h_scroll_con = value->on_change.connect(boost::bind(&viewport::redraw,this));
        h_scroll_->size(rect_.width());
        if (!children_.empty())
            h_scroll_->size(children_[0]->width());
        h_scroll_->vis_size(rect_.width());
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
        v_scroll_->size(rect_.height());
        v_scroll_->vis_size(rect_.height());
        if (!children_.empty())
            v_scroll_->size( children_[0]->height());
    }
    void viewport::redraw()
    {
        container::redraw(rect_);
    }
    void viewport::handle_event(event::ptr e)
    {
        cout << "viewport::handle_event()" << endl;
        widget::handle_event(e);
    }

    void viewport::draw(surface::ptr surface, const rectangle& clip_rect)
    {
        surface->clip_rect(clip_rect.empty() ? rect_ : clip_rect);
        point scroll_offset;
        scroll_offset.x(h_scroll()->begin());
        scroll_offset.y(v_scroll()->begin());
        surface->offset(surface->offset() + scroll_offset);

        if (children_.size() > 0)
        {
            children_[0]->draw(surface, rect_);
        }

        surface->offset(surface->offset() - scroll_offset);
    }

}
