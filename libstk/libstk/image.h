#ifndef IMAGE_H
#define IMAGE_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/signal.hpp>

#include "rectangle.h"
#include "widget.h"
#include "container.h"

namespace stk
{
	class image : public widget
	{
		public:
			typedef boost::shared_ptr<image> ptr;
			typedef boost::weak_ptr<image> weak_ptr;
		
		private:
			
		protected:
			image(container::ptr parent, const rectangle& rect, 
					const std::string& filename=""); 
			void init_pixels();
			int width_;
			int height_;
			color** pixels_;
			
		public:
			static image::ptr create(container::ptr parent, const rectangle& _rect,
					const std::string& filename="");
			virtual ~image();

			/********** EVENT HANDLER INTERFACE **********/
			/********** END EVENT HANDLER INTERFACE **********/

			/********** DRAWABLE INTERFACE **********/
			virtual void draw(surface::ptr surface);
			/********** END DRAWABLE INTERFACE **********/
			
			/********** PARENT INTERFACE **********/
			/********** END PARENT INTERFACE **********/
			
			/********** WIDGET INTERFACE **********/
			/********** END WIDGET INTERFACE **********/
			
			/********** IMAGE INTERFACE **********/
			color pixel(int x, int y) const;
			void pixel(int x, int y, color new_color);
			int width() { return width_; }
			int height() { return height_; }
			/********** END IMAGE INTERFACE **********/

	};
}

#endif
