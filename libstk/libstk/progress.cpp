#include <iostream>
#include <sstream>
#include "progress.h"
#include "widget.h"
#include "theme.h"

using std::cout;
using std::endl;

namespace stk
{

	progress::ptr progress::create(container::ptr parent, const std::wstring label, 
		const rectangle& rect, int range)
	{
		progress::ptr new_progress(new progress(parent, label, rect, range));
		parent->add_child(new_progress);
		return new_progress;
	}

	progress::progress(boost::shared_ptr<container> parent, const std::wstring label, 
		const rectangle& rect, int range) : widget(parent, rect), label_(label), 
		range_(range)
	{
		cout << "progress::progress()" << endl;
	}

	progress::~progress()
	{
	}

	void progress::draw(surface::ptr surface)
	{
		std::wstringstream label_stream;
		// FIXME: create the label based on some properties of what to show
		// be sure to format the percent to no more than 2 decimal places
		label_stream.setf(std::ios::showpoint);
		label_stream.precision(4);
		label_stream << label_ << " - " << percent()*100 << "%";
		theme::instance()->draw_progress(rect_, label_stream.str(), percent());
	}
			
}
