#include "libstk/color_manager.h"
#include <iostream>

using std::cout;
using std::endl;

namespace stk
{
	color_manager::Tcolor_map color_manager::color_map_;

	bool color_properties::operator<(const color_properties& rhs) const
	{
		// FIXME: make this include surface in the < calculation
		return (int_color < rhs.int_color) /*&& (surface_ < rhs.surface_)*/;
	}

	color_manager::color_manager()
	{
		cout << "color_manager::color_manager()" << endl;
	}

	color_manager::~color_manager()
	{
		cout << "color_manager::~color_manager()" << endl;
	}

	color color_manager::get_color(const color_properties& properties)
	{
		//cout << "color_manager::get_color()" << endl;
		//cout << "\trgba: " << std::hex << properties.red << " " << properties.green 
		//	   << " " << properties.blue << " " << properties.alpha << " -> ";
		
		Tcolor_map::iterator color_iter = color_map_.find(properties);
		if (color_iter == color_map_.end())
		{
			//cout << "color not yet created...";
			color_map_[properties] = properties.surface_->gen_color(properties.red, 
					properties.green, properties.blue, properties.alpha);
			//cout << "created: " << std::hex << color_map_[properties] << endl;
			return color_map_[properties];
		}
		else
		{
			//cout << "cached: " << std::hex << color_iter->second << endl;
			return color_iter->second;
		}
	}


}
