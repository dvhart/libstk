/******************************************************************************
 *    FILENAME: image_panel.h
 * DESCRIPTION: An image widget.
 *     AUTHORS: Darren Hart, Marc Straemke
 *  START DATE: 14/Apr/2003  LAST UPDATE: 13/May/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available
 *              with the source as license.txt or at 
 *              http://www.libstk.org/index.php?page=docs/license
 *****************************************************************************/

#ifndef IMAGE_PANEL_H
#define IMAGE_PANEL_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/signal.hpp>
#include "libstk/rectangle.h"
#include "libstk/widget.h"
#include "libstk/container.h"
#include "libstk/image.h"

namespace stk
{
	class image_panel : public widget
	{
		public:
			typedef boost::shared_ptr<image_panel> ptr;
			typedef boost::weak_ptr<image_panel> weak_ptr;
		
		private:
			
		protected:
			image_panel(widget::ptr parent, const rectangle& rect, 
					image::ptr img);
			image::ptr image_;
			int width_;
			int height_;
			
		public:
			static image_panel::ptr create(container::ptr parent, 
					const rectangle& _rect, image::ptr img);
			virtual ~image_panel();

			/********** EVENT HANDLER INTERFACE **********/
			/********** END EVENT HANDLER INTERFACE **********/

			/********** DRAWABLE INTERFACE **********/
			virtual void draw(surface::ptr surface, const rectangle& clip_rect = rectangle());
			/********** END DRAWABLE INTERFACE **********/
			
			/********** PARENT INTERFACE **********/
			/********** END PARENT INTERFACE **********/
			
			/********** WIDGET INTERFACE **********/
			/********** END WIDGET INTERFACE **********/
			
			/********** IMAGE_PANEL INTERFACE **********/
			image::ptr getimage() const {return image_; }
			void setimage(image::ptr img) { image_ = img; }
			int width() { return width_; }
			int height() { return height_; }
			/********** END IMAGE_PANEL INTERFACE **********/
	};
}

#endif
