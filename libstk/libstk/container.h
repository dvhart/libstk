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
			
		public:
			~container();
			
			/********** EVENT HANDLER INTERFACE **********/
			virtual void handle_event(event::ptr e);
			/********** END EVENT HANDLER INTERFACE **********/

			/********** DRAWABLE INTERFACE **********/
			//virtual surface::ptr surface(); 
			virtual void draw(surface::ptr surface);
			// FIXME: perhaps ALL our classes should use the same construction, ie rectangle::create
			// and we should always pass object::ptr's, consistency is appreciated in an API --dvhart
			virtual void redraw(const rectangle& rect);
			/********** END DRAWABLE INTERFACE **********/

			/********** PARENT INTERFACE **********/
			/// unfocus the current_widget, find the next, focus it and return it
			/// if we have no children, or reach the end, call ask parent_ for the 
			/// next widget.
			virtual widget::ptr focus_next();
			
			/// unfocus the current_widget, find the prev, focus it and return it
			/// if we have no children, or reach the beginning, call ask parent_ for the
			/// prev widget.
			virtual widget::ptr focus_prev();
			virtual void add_child(widget::ptr w)
			{ children_.push_back(w); }
			/********** END PARENT INTERFACE **********/
			
			/********** CONTAINER INTERFACE **********/
			/// \todo arent these redundant
			void add(widget::ptr item);
			void remove(widget::ptr item);
			// widget_at returns a widget::ptr to the container's child that contains
			// x,y.  It will call widget_at on a child that is a container to find
			virtual widget::ptr widget_at(int x, int y);
			virtual void delegate_mouse_event(mouse_event::ptr me);
			virtual bool is_container() { return true; } // FIXME: shouldn't this be in widget too
			// FIXME :carter: implement all this
			virtual widget::ptr get_active_child() // called when the tree parser for
			{ return  *children_.begin(); }        // widget cycling switches focus 
																						 // to a container
			                                       // default behaviour for a container
			                                       // is to switch into the FIRST child first 
			/********** END CONTAINER INTERFACE **********/
	};

} // namespace stk

#endif
