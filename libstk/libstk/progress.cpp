#include <iostream>
#include <sstream>
#include "libstk/progress.h"
#include "libstk/widget.h"

using std::cout;
using std::endl;

namespace stk
{

	progress::ptr progress::create(container::ptr parent, const std::wstring& format_label, 
		const rectangle& rect, int range)
	{
		progress::ptr new_progress(new progress(parent, format_label, rect, range));
		parent->add_child(new_progress);
		return new_progress;
	}

	progress::progress(boost::shared_ptr<container> parent, const std::wstring& format_label, 
		const rectangle& rect, int range) : widget(parent, rect), format_label_(format_label), 
		range_(range)
	{
		cout << "progress::progress()" << endl;
		focusable_ = false;
		build_label();
	}

	progress::~progress()
	{
	}

	void progress::range(int val)
	{ 
		range_ = MAX(0, val); 
		build_label();
		redraw(rect_); 
	}
	
	void progress::position(int val)
	{ 
		position_ = MAX(0, MIN(range_, val));
		build_label();
		redraw(rect_); 
	}
	
	void progress::percent(float val)
	{ 
		position_ = MAX(0, (int)(range_*MIN(1.0, val))); 
		build_label();
		redraw(rect_); 
	}
	
	void progress::build_label()
	{
		// %p, percent_*100 to two decimal places, followed by %
		// %d, position
		// %f, percent_ to four decimal places

		std::wstringstream num_stream;
		label_ = format_label_;
		
		// get the percent string
		num_stream.setf(std::ios::showpoint);
		num_stream.precision(4);
		num_stream << percent()*100 << "%";
		std::wstring percent_str = num_stream.str();

		// get the decimal string
		num_stream.str(L"");
		num_stream.precision(4);
		num_stream << percent();
		std::wstring decimal_str = num_stream.str();
		
		// get the position string
		num_stream.str(L"");
		num_stream.unsetf(std::ios::showpoint);
		num_stream << position_;
		std::wstring position_str = num_stream.str();
		
		int index;
		while ((index = label_.find(L"%p")) >= 0)
			label_.replace(index, 2, percent_str);
		while ((index = label_.find(L"%d")) >= 0)
			label_.replace(index, 2, position_str);
		while ((index = label_.find(L"%f")) >= 0)
			label_.replace(index, 2, decimal_str);
	}
	
}
