#include "libstk/image_panel.h"
#include "libstk/image.h"

namespace stk
{
	image_panel::ptr image_panel::create(widget::ptr parent, const rectangle& rect, 
			image::ptr img)
	{
		image_panel::ptr new_image_panel(new image_panel(parent, rect, img));
		parent->add(new_image_panel);
		return new_image_panel;
	}
	
	image_panel::image_panel(widget::ptr parent, const rectangle& rect, image::ptr img)
		: widget(parent, rect), image_(img)
	{
		cout << "image_panel::image_panel()" << endl;
		focusable_ = true; // FIXME: this should be false, it is set for testing the scroll decorator
	}
	
	image_panel::~image_panel()
	{
	}
	
}
