/******************************************************************************
 *    FILENAME: list.h
 * DESCRIPTION: A listbox widget.  It contains a vector of list_items.
 *     AUTHORS: Darren Hart, Marc Straemke
 *  START DATE: 03/Mar/2003  LAST UPDATE: 13/May/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available
 *              with the source as license.txt or at 
 *              http://www.libstk.org/index.php?page=docs/license
 *****************************************************************************/

#ifndef STK_LIST_H
#define STK_LIST_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "libstk/rectangle.h"
#include "libstk/widget.h"
#include "libstk/container.h"
#include "libstk/list_item.h"

namespace stk
{
	/// \todo is widget right.. not a container ?
	class list : public widget
	{
		public:
			typedef boost::shared_ptr<list> ptr;
			typedef boost::weak_ptr<list> weak_ptr;

		private:

		protected:
			int selected_, prev_selected_;
			list(container::ptr parent, const rectangle& rect);
			std::vector<list_item::ptr> items_;

		public:
			static list::ptr create(container::ptr parent, const rectangle& rect);
			virtual ~list();

			/********** EVENT HANDLER INTERFACE **********/
			virtual void handle_event(event::ptr e);
			/********** END EVENT HANDLER INTERFACE **********/

			/********** DRAWABLE INTERFACE **********/
			virtual void draw(surface::ptr surface, const rectangle& clip_rect = rectangle());
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
