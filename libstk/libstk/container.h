#ifndef STK_CONTAINER_H
#define STK_CONTAINER_H

#include <map>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "application.h"
#include "widget.h"
#include "mouse_event.h"

namespace stk
{
	class container : public stk::widget
	{
		public:
			typedef boost::shared_ptr<container> ptr;
			typedef boost::weak_ptr<container> weak_ptr;

		private:
			
		protected:
			std::vector<widget::ptr> children_;
			container(container::ptr parent, const rectangle& rect);
			container(parent::ptr parent, const rectangle& rect);
			rectangle redraw_rect_; 
			
		public:
			virtual ~container();
			
			/********** EVENT HANDLER INTERFACE **********/
			virtual void handle_event(event::ptr e);
			/********** END EVENT HANDLER INTERFACE **********/

			/********** DRAWABLE INTERFACE **********/
			//virtual surface::ptr surface(); 
			virtual void draw(surface::ptr surface);
			virtual void redraw(const rectangle& rect);
			/********** END DRAWABLE INTERFACE **********/

			/********** PARENT INTERFACE **********/
			/// Return the next focusable widget.
			/// Find the focused widget, and return the next focusable one if we have 
			/// no children, or reach the end, ask parent_ for the next widget.
			virtual widget::ptr focus_next();
			
			/// Return the prev focusable widget.
			/// Find the focused widget, and return the previous focusable one if we 
			/// have no children, or reach the beginning, ask parent_ for the next 
			/// widget.
			virtual widget::ptr focus_prev();
			virtual void add_child(widget::ptr w)
			{ children_.push_back(w); }
			/********** END PARENT INTERFACE **********/
			
			/********** WIDGET INTERFACE **********/
			virtual bool is_container() { return true; }
			/********** END WIDGET INTERFACE **********/
			
			/********** CONTAINER INTERFACE **********/
			/// \todo arent these redundant (aren't what redundant ??)
			/// Add a widget to the children vector
			void add(widget::ptr item);
			/// Remove a child widget from the children vector
			void remove(widget::ptr item);
			
			/// Returns a widget::ptr to the container's first child that contains
			/// x,y.  It will call widget_at on a child that is a container.
			virtual widget::ptr widget_at(int x, int y);
			
			/// Pass mouse events down to widgets.
			virtual void delegate_mouse_event(mouse_event::ptr me);
			
			// FIXME :carter: implement all this
			virtual widget::ptr get_active_child() // called when the tree parser for
			{ return  *children_.begin(); }        // widget cycling switches focus 
																						 // to a container
			                                       // default behaviour for a container
			                                       // is to switch into the FIRST child first 
			virtual rectangle redraw_rect() { return redraw_rect_; } 
			/********** END CONTAINER INTERFACE **********/
	};

} // namespace stk

#endif
