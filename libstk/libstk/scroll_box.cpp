/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Scroll Box container implementation.
 *     AUTHORS: Darren Hart
 *  START DATE: 2003/May/22
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include <algorithm>
#include <boost/bind.hpp>
#include "libstk/scroll_box.h"
#include "libstk/event.h"
#include "libstk/key_event.h"

namespace stk
{
    scroll_box::ptr scroll_box::create(container::ptr parent, const rectangle& rect, 
            bool v_scroll, bool h_scroll)
    {
        scroll_box::ptr new_scroll_box(new scroll_box(parent, rect));

        INFO("creating viewport");
        // FIXME: get the 20s from the theme somehow
        rectangle vp_rect = rect;
        if (v_scroll) vp_rect.x2(vp_rect.x2() - 20);
        if (h_scroll) vp_rect.y2(vp_rect.y2() - 20);
        new_scroll_box->viewport_ = viewport::create(new_scroll_box, vp_rect);
        
        if (v_scroll)
        {
            INFO("creating v_scroll_bar");
            rectangle v_rect(rect.width()-20, 0, 20, rect.height());
            new_scroll_box->v_scroll_bar_ = scroll_bar::create(new_scroll_box, v_rect, 
                        new_scroll_box->viewport_->v_scroll());
        }
 
        if (h_scroll)
        {
            INFO("creating h_scroll_bar");
            rectangle h_rect(0, rect.height()-20, rect.width(), 20);
            new_scroll_box->h_scroll_bar_ = scroll_bar::create(new_scroll_box, h_rect, 
                        new_scroll_box->viewport_->h_scroll());
        }

        parent->add(new_scroll_box);
        return new_scroll_box;
    }

    scroll_box::scroll_box(container::ptr parent, const rectangle& rect) : container(parent, rect)
    {
        INFO("constructor");
        focusable_ = true;
    }

    scroll_box::~scroll_box()
    {
        INFO("destructor");
    }

    void scroll_box::h_scroll(scroll_model::ptr model)
    {
        viewport_->h_scroll(model);
        if (h_scroll_bar_) h_scroll_bar_->model(model);
    }

    void scroll_box::v_scroll(scroll_model::ptr model)
    {
        viewport_->v_scroll(model);
        if (v_scroll_bar_) v_scroll_bar_->model(model);
    }

    void scroll_box::redraw()
    {
        container::redraw(rect());
    }

    void scroll_box::handle_event(event::ptr e)
    {
        widget::handle_event(e);
    }

    void scroll_box::add(widget::ptr item)
    {
        container::add(item);
    }

    void scroll_box::draw(surface::ptr surface, const rectangle& clip_rect)
    {
        container::draw(surface, clip_rect);
    }

}
