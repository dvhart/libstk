#include <iostream>
#include "libstk/label.h"
#include "libstk/widget.h"

using std::cout;
using std::endl;

namespace stk
{

	label::ptr label::create(container::ptr parent, std::wstring text, const rectangle& rect)
	{
		label::ptr new_label(new label(parent, text, rect));
		parent->add_child(new_label);
		return new_label;
	}

	label::label(boost::shared_ptr<container> parent, std::wstring text,
					const rectangle& rect) : widget(parent, rect), text_(text)
	{
		cout << "label::label()" << endl;
		focusable_ = false;
	}

	label::~label()
	{
	}

}
