/**************************************************************************************************
 *    FILENAME: list_item.cpp 
 * DESCRIPTION: List_item class implementation.
 *     AUTHORS: Darren Hart, Marc Straemke 
 *  START DATE: 03/Mar/2003  LAST UPDATE: 14/Jul/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *     license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include <iostream>
#include "libstk/list.h"
#include "libstk/list_item.h"

using std::cout;
using std::endl;

namespace stk
{
    list_item::ptr list_item::create(list::ptr parent, std::wstring label)
    {
        list_item::ptr new_list_item(new list_item(parent, label));
        parent->add_item(new_list_item);
        return new_list_item;
    }

    list_item::list_item(list::ptr parent, std::wstring label)
            : parent_(parent), label_(label)
    {
        cout << "list_item::list_item()" << endl;
    }

    list_item::~list_item()
    {
        cout << "list_item::~list_item()" << endl;
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
        parent_.lock()->surface();
    }

}
