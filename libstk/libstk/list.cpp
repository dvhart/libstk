/******************************************************************************
 *    FILENAME: list.cpp
 * DESCRIPTION: List widget implementation.
 *     AUTHORS: Darren Hart, Marc Straemke
 *  START DATE: 03/Mar/2003  LAST UPDATE: 22/May/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available
 *              with the source as license.txt or at 
 *              http://www.libstk.org/index.php?page=docs/license
 *****************************************************************************/

#include "libstk/list.h"
#include "libstk/event.h"
#include "libstk/key_event.h"
#include "libstk/mouse_event.h"
#include "libstk/keycode.h"

namespace stk
{
	list::ptr list::create(container::ptr parent, const rectangle& rect, 
			scroll_model::ptr v_scroll)
	{
		list::ptr new_list(new list(parent, rect, v_scroll));
		parent->add(new_list);
		return new_list;
	}

	list::list(container::ptr parent, const rectangle& rect, 
			scroll_model::ptr v_scroll) : widget(parent, rect), selected_(0), 
			prev_selected_(0), v_scroll_(v_scroll)
	{
		cout << "list::list()" << endl;
		focusable(true);
		v_scroll_->vis_size(rect_.height());
	}

	list::~list()
	{
	}

	void list::handle_event(event::ptr e)
	{
		//cout << "list::handle_event()" << endl;
		// handle list_item selection and clicking here...	
		switch (e->type())
		{
			case event::mouse_down:
			{
				mouse_event::ptr me = boost::shared_static_cast<mouse_event>(e);
				selected_ = (v_scroll_->begin() / 25) + ((me->y() - rect_.y1()) / 25);
				redraw(rect_);
				return;
				break;
			}
			case event::key_up:
			{
				key_event::ptr ke = boost::shared_static_cast<key_event>(e);
				switch ( ke->key() )
				{
					case up_arrow:
						if (selected_ > 0)
						{
							selected_--;
							// FIXME: magic 25's
							if (selected_ < v_scroll_->begin() / 25)
								v_scroll_->begin(v_scroll_->begin() - 25);
							redraw(rect_);
						}
						return;
						break;
					case down_arrow:
						if (selected_ < items_.size() - 1) 
						{
							selected_++;
							// FIXME: magic 25's
							if (selected_ >= v_scroll_->end() / 25)
								v_scroll_->begin(v_scroll_->begin() + 25);
							redraw(rect_);
						}
						return;
						break;
				}
			}
		}
		widget::handle_event(e); 
	}
		
	void list::add_item(list_item::ptr item)
	{
		items_.push_back(item);
		cout << "list::add_item: items_ count=" << items_.size() << endl;

		// adjust scroll properties
		// FIXME: add width to the list_item API
		//if (h_scroll()->size() < item->width()) h_scroll()->size(item->width());
		// FIXME: store the item height (25) somewhere, it is also magic in tribal_theme.cpp
		v_scroll_->size(v_scroll_->size() + 25);
	}
	
}
