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
			container(container::ptr parent);
			container(parent::ptr parent);
			
		public:
			~container();

			virtual void draw(boost::shared_ptr<stk::surface> surface);
			
			virtual bool is_container() { return true; }
			// FIXME :carter: implement all this
			virtual widget::ptr get_active_child()  // called when the tree parser for widget cycling switches focus to a container
			{ return  *children_.begin(); } // default behaviour for a container is to switch into the FIRST child first 
			
			// FIXME: write me
			virtual widget::ptr widget_at(int x, int y);
			virtual void delegate_mouse_event(mouse_event::ptr me);
			
			// event_handler interface
			virtual void handle_event(event::ptr e);

			// drawable interface
			//virtual surface::ptr surface() 
			//{ return make_shared(parent_)->surface(); }

			// parent interface
			// FIXME
			virtual widget::ptr focus_next()
			{ return *children_.begin(); }
			
			virtual widget::ptr focus_prev()
			{ return *children_.begin(); }
			
			virtual void add_child(widget::ptr w)
			{ children_.push_back(w); }
			
			/// container specific methods
			/// \todo arent these redundant
			void add(widget::ptr item);
			void remove(widget::ptr item);
	};

} // namespace stk

#endif
