#include <boost/weak_ptr.hpp>
#include "container.h"
#include "exceptions.h"

namespace stk
{

	container::container(boost::weak_ptr<container> parent) : stk::widget(parent)
	{}

	container::~container()
	{}

	void container::add(boost::shared_ptr<widget> item)
	{
		children_.push_back(item);
	}

	void container::remove(boost::shared_ptr<widget> item)
	{
		std::vector<boost::shared_ptr<stk::widget> >::iterator iter;
		iter=std::find(children_.begin(), children_.end(), item);
		children_.erase(iter);
	}

}
