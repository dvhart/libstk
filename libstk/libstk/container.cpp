#include <iostream>
#include <boost/weak_ptr.hpp>

#include "widget.h"
#include "container.h"
#include "exceptions.h"

using std::cout;
using std::endl;

namespace stk
{

	container::container(boost::shared_ptr<container> parent) : widget(parent)
	{
		cout << "container::container(container)" << endl;
	}
	
	container::container(boost::shared_ptr<application> parent) : widget(parent)
	{
		cout << "container::container(application)" << endl;
	}

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
