#include "container.h"
#include "exceptions.h"

namespace stk
{
	

container::container(app* app,widget* parent) : widget(app,parent)
{}

container::~container()
{}

void container::add(Twidget_ptr widget)
{
	childs_.push_back(widget);
}

void container::erase(Twidget_ptr widget)
{
	Tchilds::iterator iter;
	iter=std::find(childs_.begin(),childs_.end(),widget);
	childs_.erase(iter);
}

Twidget_ptr lookup_widget(std::string name)
{
	/* FIXME , havent implemented search comparison function yet
    Tchilds::const_iterator iter;

    iter = childs_.find(name);
    if (iter = _childs.end()) // no pair found 
        throw stk::error_message_exception("stk::container::lookup_widget(\""+name+"\"): no such widget found!");

    return *(iter->second);*/
}

Twidget_ptr container::prev()
{
 /*   if (_iter == childs.begin())
        _iter = childs.end();
    else
        _iter--;

    return *(_iter->second);*/
}

Twidget_ptr container::next()
{
 /*   if (_iter == childs.end())
        _iter = childs.begin();
    else
        _iter++;

    return *(_iter->second);*/
}

}
