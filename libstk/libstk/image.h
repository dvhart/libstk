/******************************************************************************
 *    FILENAME: image.h
 * DESCRIPTION: A generic image class.
 *     AUTHORS: Darren Hart, Marc Straemke
 *  START DATE: 03/Mar/2003  LAST UPDATE: 13/May/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available
 *              with the source as license.txt or at 
 *              http://www.libstk.org/index.php?page=docs/license
 *****************************************************************************/

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
