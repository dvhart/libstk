#include <iostream>
#include <sstream>
#include "image.h"
#include "widget.h"
#include "theme.h"

using std::cout;
using std::endl;

namespace stk
{
	image::ptr image::create(container::ptr parent, const rectangle& rect,
			const std::string& filename)
	{
		image::ptr new_image(new image(parent, rect, filename));
		parent->add_child(new_image);
		return new_image;
	}
	
	image::image(container::ptr parent, const rectangle& rect,
			const std::string& filename) : widget(parent, rect)
	{
		cout << "image::image()" << endl;

		if (filename != "")
		{
			cout << "\tloading image: " << filename << endl;
		}
		else
		{
			cout << "\tcreating empty image in: " << rect << endl;
			width_ = rect.width();
			height_ = rect.height();
			init_pixels();
		}
	}
	
	image::~image()
	{
		for (int x = 0; x < width_; x++)
		{
			delete[] pixels_[x];
		}
		delete[] pixels_;
	}
	
	void image::draw(surface::ptr surface)
	{
		// FIXME: why can't we create this pointer ?
		// because of circular dependency?
		// then how do we get the pixel data to the theme/surface ?
		image::ptr temp_image;
		theme::instance()->draw_image(rect_, temp_image);
	}
	
	color image::pixel(int x, int y) const
	{
		color cur_color;
		return cur_color;
	}
	
	void image::pixel(int x, int y, color new_color)
	{
	}

	void image::init_pixels()
	{
		pixels_ = new color*[width_];
		for (int x = 0; x < width_; x++)
		{
			pixels_[x] = new color[height_];
		}
	}
}
