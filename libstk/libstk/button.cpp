#include <iostream>
#include "button.h"
#include "widget.h"

using std::cout;
using std::endl;

namespace stk
{
	//button::button(boost::weak_ptr<container> parent) : widget(parent)
	button::button(boost::shared_ptr<container> parent, std::string label,
					int x, int y, int width, int height) : widget(parent)
	{
		cout << "button::button()" << endl;
	}

	button::~button()
	{
	}
}
