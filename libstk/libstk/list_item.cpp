#include "libstk/list_item.h"

namespace stk
{
	list_item::ptr list_item::create(std::wstring label, std::string data)
	{
		list_item::ptr new_list_item(new list_item(label, data));
		return new_list_item;
	}

	list_item::list_item(std::wstring label, std::string data)
		: label_(label), data_(data)
	{
	}

	list_item::~list_item()
	{
	}

}
