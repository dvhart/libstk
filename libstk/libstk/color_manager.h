#ifndef COLOR_MANAGER_H
#define COLOR_MANAGER_H

#include <boost/smart_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <string>
#include <map>
#include "libstk/surface.h"

namespace stk
{	
	/// Public color properties class.
	struct color_properties
	{
		unsigned char red, green, blue, alpha;
		unsigned int int_color;
		surface::ptr surface_;
		color_properties(unsigned char r, unsigned char g, unsigned char b, 
				             unsigned char a, surface::ptr s) 
			: red(r), green(g), blue(b), surface_(s) 
		{
			int_color = (red << 24)|(green << 16)|(blue << 8)|(alpha);
		}
		color_properties(const std::string& rgba, surface::ptr s) 
			: surface_(s)
		{
			int_color = strtoll(rgba.c_str(), NULL, 16);
			red = (int_color & 0xFF000000) >> 24;
			green = (int_color & 0x00FF0000) >> 16;
			blue = (int_color & 0x0000FF00) >> 8;
			alpha = (int_color & 0x000000FF);
		}

		bool operator<(const color_properties& rhs) const;
	};

	/// \brief Class for manaing Surface Colors
	/// This class manages all creation and retrieveal of color instances, the
	/// colors are stored according to surface and rgba values.
	class color_manager
	{
		public:
			typedef std::map<color_properties, color> Tcolor_map;
			typedef boost::shared_ptr<color_manager> ptr;
			typedef boost::weak_ptr<color_manager> weak_ptr;

		private:
			static Tcolor_map color_map_;
			color_manager();

		public:
			/// Get a color with the provided Properties
			static color get_color(const color_properties& properties);
			~color_manager();
	};

}

#endif
