#include "libstk/scroll_decorator.h"

namespace stk
{
	scroll_decorator::ptr scroll_decorator::create(container::ptr parent, 
			const rectangle& rect, widget::ptr component)
	{
		scroll_decorator::ptr new_scroll_decorator(new scroll_decorator(parent, 
					rect, component));
		parent->add(new_scroll_decorator);
		return new_scroll_decorator;
	}
	
	scroll_decorator::scroll_decorator(container::ptr parent, 
			const rectangle& rect, widget::ptr component) 
		: decorator(parent, rect, component), vrange_(component->width()), 
	    hrange_(component->height()), vposition_(0), hposition_(0)
	{
	}
			
	scroll_decorator::~scroll_decorator()
	{
	}
}

