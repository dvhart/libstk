/******************************************************************************
 *    FILENAME: list_item.cpp 
 * DESCRIPTION: List_item class implementation.
 *     AUTHORS: Darren Hart, Marc Straemke 
 *  START DATE: 03/Mar/2003  LAST UPDATE: 14/May/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available
 *              with the source as license.txt or at 
 *              http://www.libstk.org/index.php?page=docs/license
 *****************************************************************************/

#include "libstk/list.h"
#include "libstk/list_item.h"

namespace stk
{
	list_item::ptr list_item::create(list::ptr parent, std::wstring label, std::string data)
	{
		list_item::ptr new_list_item(new list_item(parent, label, data));
		parent->add_item(new_list_item);
		return new_list_item;
	}

	list_item::list_item(list::ptr parent, std::wstring label, std::string data)
		: parent_(parent), label_(label), data_(data)
	{
	}

	list_item::~list_item()
	{
	}

	void list_item::handle_event(event::ptr e)
	{
		parent_.lock()->handle_event(e);
	}

	surface::ptr list_item::surface() 
	{ 
		parent_.lock()->surface();
	}
	
}
