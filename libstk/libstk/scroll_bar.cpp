/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Scroll bar class 
 *     AUTHORS: Darren Hart
 *  START DATE: 2003/Oct/8
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include <iostream>
#include <boost/bind.hpp>
#include "libstk/scroll_bar.h"

namespace stk
{
    scroll_bar::ptr scroll_bar::create(container::ptr parent, const rectangle& rect, 
            scroll_model::ptr model)
    {
        scroll_bar::ptr new_scroll_bar(new scroll_bar(rect, model));
        new_scroll_bar->parent(parent);
        new_scroll_bar->scroll_con_ = new_scroll_bar->model_->on_change.connect(
                boost::bind(&scroll_bar::redraw, new_scroll_bar.get()));
        return new_scroll_bar;
    }

    scroll_bar::scroll_bar(const rectangle& rect, scroll_model::ptr model) : widget(rect)
    {
        INFO("constructor");
        model_ = (model ? model : scroll_model::create()); 
        focusable_ = true;
    }

    scroll_bar::~scroll_bar()
    {
        INFO("destructor");
    }

    void scroll_bar::model(scroll_model::ptr model)
    {
        scroll_con_.disconnect();
        model_ = model;
        scroll_con_ = model_->on_change.connect(boost::bind(&scroll_bar::redraw, this));
    }

    void scroll_bar::redraw()
    {
        widget::redraw(rect());
    }

    void scroll_bar::handle_event(event::ptr e)
    {
        widget::handle_event(e);
    }

}
