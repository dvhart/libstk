#ifndef STK_IMAGE_H
#define STK_IMAGE_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "libstk/rectangle.h"

namespace stk
{
	class image
	{
		public:
			typedef boost::shared_ptr<image> ptr;
			typedef boost::weak_ptr<image> weak_ptr;
		
		private:
			
		protected:
			image(const std::string& filename=""); 
			image(const rectangle& rect); 
			void init_pixels();
			int width_;
			int height_;
			color** pixels_;
			
		public:
			static image::ptr create(const std::string& filename="");
			static image::ptr create(const rectangle& _rect);
			virtual ~image();

			/********** IMAGE INTERFACE **********/
			void load_ppmx(const std::string& filename);
			color pixel(int x, int y) const;
			void pixel(int x, int y, color new_color);
			int width() { return width_; }
			int height() { return height_; }
			/********** END IMAGE INTERFACE **********/
	};
}

#endif
