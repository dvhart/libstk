#include "image_panel.h"
#include "image.h"
#include "theme.h"

namespace stk
{
	image_panel::ptr image_panel::create(container::ptr parent, const rectangle& rect, 
			image::ptr img)
	{
		image_panel::ptr new_image_panel(new image_panel(parent, rect, img));
		parent->add_child(new_image_panel);
		return new_image_panel;
	}
	
	image_panel::image_panel(container::ptr parent, const rectangle& rect, image::ptr img)
		: widget(parent, rect), image_(img)
	{
		cout << "image_panel::image_panel()" << endl;
	}
	
	image_panel::~image_panel()
	{
	}
	
	void image_panel::draw(surface::ptr surface)
	{
		theme::instance()->draw_image_panel(rect_, image_);
	}
}
