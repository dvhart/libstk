#include "container.h"
#include "exceptions.h"

namespace stk
{
	

container::container(boost::weak_ptr<container> parent) : widget(parent)
{}

container::~container()
{}

void container::add(Twidget_ptr item)
{
	childs_.push_back(item);
}

void container::remove(Twidget_ptr item)
{
	Tchilds::iterator iter;
	iter=std::find(childs_.begin(),childs_.end(),item);
	childs_.erase(iter);
}


}
