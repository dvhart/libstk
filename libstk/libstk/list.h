#ifndef LIST_H
#define LIST_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "rectangle.h"
#include "widget.h"
#include "container.h"
#include "list_item.h"

namespace stk
{
	/// \todo is widget right.. not a container ?
	class list : public widget
	{
		public:
			typedef boost::shared_ptr<list> ptr;
			typedef boost::weak_ptr<list> weak_ptr;

		private:
			int selected_, prev_selected_, frame_; // frame_ should probably be part of widget

		protected:
			list(container::ptr parent, const rectangle& rect);
			std::vector<list_item::ptr> items_;

		public:
			static list::ptr create(container::ptr parent, const rectangle& rect);
			virtual ~list();

			/********** EVENT HANDLER INTERFACE **********/
			virtual void handle_event(event::ptr e);
			/********** END EVENT HANDLER INTERFACE **********/

			/********** DRAWABLE INTERFACE **********/
			virtual void draw(surface::ptr surface);
			/********** END DRAWABLE INTERFACE **********/
			
			/********** PARENT INTERFACE **********/
			/// \todo this would have to return a this pointer!!! unless we make list_items a widget and list a container!!!
			//virtual widget::ptr focus_next(); 
			//virtual widget::ptr focus_prev();
			/********** END PARENT INTERFACE **********/


			/********** LIST INTERFACE **********/
			virtual void add_item(list_item::ptr item);
			/********** END LIST INTERFACE **********/

	};
}

#endif
