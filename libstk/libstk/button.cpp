#include <iostream>
#include "button.h"
#include "widget.h"

using std::cout;
using std::endl;

namespace stk
{
	button::button(boost::weak_ptr<container> parent) : widget(parent)
	{
		cout << "button::button()" << endl;
	}

	button::~button()
	{
	}
}
