#include <iostream>
#include "label.h"
#include "widget.h"
#include "theme.h"

using std::cout;
using std::endl;

namespace stk
{

	label::ptr label::create(container::ptr parent, std::string text, const rectangle& rect)
	{
		label::ptr new_label(new label(parent, text, rect));
		parent->add_child(new_label);
		return new_label;
	}

	label::label(boost::shared_ptr<container> parent, std::string text,
					const rectangle& rect) : widget(parent), text_(text), rect_(rect)
	{
		cout << "label::label()" << endl;
	}

	label::~label()
	{
	}

	void label::draw(boost::shared_ptr<stk::surface> surface)
	{
		redraw_ = false;
		theme::instance()->draw_label(rect_, text_);
	}
			
}
