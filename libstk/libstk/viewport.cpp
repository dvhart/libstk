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
#include "libstk/override_return.h"

namespace stk
{
    viewport::ptr viewport::create(container::ptr parent, const rectangle& rect)
    {
        viewport::ptr new_viewport(new viewport(rect));
        new_viewport->on_resize.connect(boost::function<bool()>(
                (boost::bind(&scrollable::update_vis_sizes, new_viewport.get(), 
                    boost::bind(&rectangle::height, new_viewport.get()), 
                    boost::bind(&rectangle::width, new_viewport.get())), true)));
        new_viewport->parent(parent);
        return new_viewport;
    }

    viewport::viewport(const rectangle& rect) : container(rect), scrollable()
    {
        INFO("constructor");
        focusable_ = true;
        v_scroll_->vis_size(height());
        h_scroll_->vis_size(width());
    }

    viewport::~viewport()
    {
        INFO("destructor");
    }

    void viewport::h_scroll(scroll_model::ptr model)
    {
        h_scroll_con.disconnect();
        h_scroll_ = model;
        h_scroll_con = model->on_change.connect(boost::bind(&viewport::redraw, this));
        h_scroll_->size(width());
        if (!children_.empty())
            h_scroll_->size(children_[0]->width());
        h_scroll_->vis_size(width());
    }

    void viewport::v_scroll(scroll_model::ptr model)
    {
        v_scroll_con.disconnect();
        v_scroll_ = model;
        v_scroll_con = model->on_change.connect(boost::bind(&viewport::redraw, this));
        v_scroll_->size(height());
        v_scroll_->vis_size(height());
        if (!children_.empty())
            v_scroll_->size( children_[0]->height());
    }

    void viewport::redraw()
    {
        container::redraw(widget::rect());
    }

    void viewport::handle_event(event::ptr e)
    {
        widget::handle_event(e);
    }

    void viewport::add(widget::ptr item)
    {
        children_.push_back(item);

        rectangle rect;

        // Fixme, this should really be using for each shouldnt it?
        for (unsigned int i = 0; i < children_.size(); i++)
            rect += children_[i]->rect();

        h_scroll()->size(rect.x2());
        v_scroll()->size(rect.y2());
    }

}
