#include "libstk/scroll_decorator.h"

namespace stk
{
	scroll_decorator::ptr scroll_decorator::create(container::ptr parent, 
			const rectangle& rect)
	{
		scroll_decorator::ptr new_scroll_decorator(
				new scroll_decorator(parent, rect));
		parent->add(new_scroll_decorator);
		return new_scroll_decorator;
	}
	
	scroll_decorator::scroll_decorator(container::ptr parent, 
			const rectangle& rect) : decorator(parent, rect), vrange_(0), 
			hrange_(0), vposition_(0), hposition_(0)
	{
		//focusable(true);
	}
			
	scroll_decorator::~scroll_decorator()
	{
	}

}

