/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: List_item class implementation.
 *     AUTHORS: Darren Hart, Marc Strämke 
 *  START DATE: 2003/Mar/03
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include <iostream>
#include "libstk/list.h"
#include "libstk/list_item.h"

namespace stk
{
    list_item::ptr list_item::create(list::ptr parent, std::wstring label)
    {
        list_item::ptr new_list_item(new list_item(parent, label));
        parent->add_item(new_list_item);
        return new_list_item;
    }

    list_item::list_item(list::ptr parent, std::wstring label)
            : parent_(parent), label_(label), selected_(false), current_(false)
    {
        INFO("constructor");
    }

    list_item::~list_item()
    {
        INFO("destructor");
    }

    void list_item::handle_event(event::ptr e)
    {
        // handle list_item events
        // WRITEME...

        // list_item does not inherit from widget, so we have to go to the parent directly
        parent_.lock()->handle_event(e);
    }

    surface::ptr list_item::surface()
    {
        return parent_.lock()->surface();
    }

    void list_item::redraw(const rectangle& rect, drawable* source, bool transform)
    {
        // FIXME: what is the benefit of copying the rectangle?
        rectangle redraw_rect(rect);
        parent_.lock()->redraw(redraw_rect, this, true);
    }


}
