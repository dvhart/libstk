#ifndef STK_SPINNER_H
#define STK_SPINNER_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "libstk/rectangle.h"
#include "libstk/container.h"
#include "libstk/list.h"
#include "libstk/list_item.h"

namespace stk
{
	class spinner : public list
	{
		public:
			typedef boost::shared_ptr<spinner> ptr;
			typedef boost::weak_ptr<spinner> weak_ptr;

		private:

		protected:
			spinner(container::ptr parent, const rectangle& rect);

		public:
			static spinner::ptr create(container::ptr parent, const rectangle& rect);
			virtual ~spinner();

			/********** EVENT HANDLER INTERFACE **********/
			//virtual void handle_event(event::ptr e);
			/********** END EVENT HANDLER INTERFACE **********/

			/********** DRAWABLE INTERFACE **********/
			virtual void draw(surface::ptr surface);
			/********** END DRAWABLE INTERFACE **********/
			
			/********** PARENT INTERFACE **********/
			/// \todo this would have to return a this pointer!!! unless we make list_items a widget and spinner a container!!!
			//virtual widget::ptr focus_next(); 
			//virtual widget::ptr focus_prev();
			/********** END PARENT INTERFACE **********/

			/********** LIST INTERFACE **********/
			//virtual void add_item(list_item::ptr item);
			/********** END LIST INTERFACE **********/

			/********** SPINNER INTERFACE **********/
			/********** END SPINNER INTERFACE **********/
	};
}

#endif
