#include "libstk/list.h"
#include "libstk/list_item.h"
#include "libstk/theme.h"

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
	
	void list_item::draw(surface::ptr surface)
	{
		theme::instance()->draw_list_item(rect_, label_);
	}


}
