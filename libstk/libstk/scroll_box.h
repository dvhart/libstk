#ifndef STK_SCROLL_BOX_H
#define STK_SCROLL_BOX_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "libstk/container.h"
#include "scroll_modell.h"

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
			/*/// the height of the scroll area
			int vrange_;
			/// the width of the scroll area
			int hrange_;
			/// the verticle position of the scroll area (first y value in the window)
			int vposition_;
			/// the horizontal position of the scroll area (first x value in the window)
			int hposition_; */
		
			scroll_modell::ptr h_scroll_; 
			boost::signals::connection h_scroll_con;
			scroll_modell::ptr v_scroll_; 
			boost::signals::connection v_scroll_con;
		
			/// helper function to redraw the whole widget
			void redraw(); 
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
				h_scroll()->size(children_[0]->width());
				v_scroll()->size(children_[0]->height());
			}
			/********** END PARENT INTERFACE **********/

			/********** SCROLL BOX INTERFACE **********/
			scroll_modell::ptr h_scroll(); 
			void h_scroll(scroll_modell::ptr value); 
			
			scroll_modell::ptr v_scroll(); 
			void v_scroll(scroll_modell::ptr value); 
			/********** END SCROLL BOX INTERFACE **********/
	};
}

#endif
