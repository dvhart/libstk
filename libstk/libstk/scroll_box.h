#ifndef STK_SCROLL_BOX_H
#define STK_SCROLL_BOX_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "libstk/container.h"

namespace stk
{
	class scroll_box : public container
	{
		public:
			typedef boost::shared_ptr<scroll_box> ptr; 
			typedef boost::weak_ptr<scroll_box> weak_ptr;

		private:

		protected:
			scroll_box(container::ptr parent, const rectangle& rect); 

			// FIXME: remove these in favor of the scroll_properties objects
			/// the height of the scroll area
			int vrange_;
			/// the width of the scroll area
			int hrange_;
			/// the verticle position of the scroll area (first y value in the window)
			int vposition_;
			/// the horizontal position of the scroll area (first x value in the window)
			int hposition_;
			
		public:
			static scroll_box::ptr create(container::ptr parent, 
					const rectangle& rect);
			~scroll_box();
			
			/********** DRAWABLE INTERFACE **********/
			virtual void draw(surface::ptr surface, const rectangle& clip_rect = rectangle());
			/********** END DRAWABLE INTERFACE **********/
				
			/********** EVENT HANDLER INTERFACE **********/
			virtual void handle_event(event::ptr e);
			/********** END EVENT HANDLER INTERFACE **********/
			
			/********** PARENT INTERFACE **********/
			/// Set the internal widget, replace it if already set.
			/// scroll_box only manages one child.
			virtual void add(widget::ptr item) 
			{ 
				if (children_.size() == 0)
				{
					children_.push_back(item);
				}
				else
				{
					children_[0] = item;
				}
				// FIXME: adjust the new scroll_properties widgets here instead
				vrange_ = children_[0]->height();
				hrange_ = children_[0]->width();
			}
			/********** END PARENT INTERFACE **********/

			/********** SCROLL BOX INTERFACE **********/
			/// Whether or not to draw the verticle scrollbar
			/// \todo: perhaps this should take an enum: show, hide, auto ?
			void show_vscroll(bool val);
			/// Whether or not to draw the horizontal scrollbar
			/// \todo: perhaps this should take an enum: show, hide, auto ?
			void show_hscroll(bool val);
			/// Whether or not to show the arrows on the scrollbars
			/// You want these if you have a mouse, but they are superfluous and maybe 
			/// confusing if you don't.
			void show_arrows(bool val);
			/// Set the verticle scroll position as a percentage [0-1.0]
			void vpos(float percent);
			/// Set the horizontal scroll position as a percentage [0-1.0]
			void hpos(float percent);
			/********** END SCROLL BOX INTERFACE **********/
	};
}

#endif
