/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: List_item class implementation.
 *     AUTHORS: Darren Hart, Marc Str�mke 
 *  START DATE: 2003/Mar/03
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Str�mke, Dirk H�rner
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
        list_item::ptr new_list_item(new list_item(label));
        parent->add_item(new_list_item);
        new_list_item->parent_ = parent;
        return new_list_item;
    }

    list_item::list_item(std::wstring label) : widget(), label_(label), selected_(false), 
    current_(false)
    {
        INFO("constructor");
        //focusable_ = true;
    }

    list_item::~list_item()
    {
        INFO("destructor");
    }

    // the parent list handles all events
    /*
    void list_item::handle_event(event::ptr e)
    {
        // handle list_item events
        // ... none atm
        widget::handle_event(e);
    }
    */

    void list_item::selected(bool val) 
    { 
        if (selected_ != val)
        {
            selected_ = val; 
            redraw(rect());
        }
    }

    void list_item::current(bool val) 
    { 
        if (current_ != val)
        {
            current_ = val; 
            redraw(rect());
        }
    }
}
