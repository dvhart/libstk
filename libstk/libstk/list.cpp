#include "libstk/list.h"
#include "libstk/event.h"
#include "libstk/key_event.h"
#include "libstk/mouse_event.h"
#include "libstk/keycode.h"

namespace stk
{
	list::ptr list::create(container::ptr parent, const rectangle& rect)
	{
		list::ptr new_list(new list(parent, rect));
		parent->add(new_list);
		return new_list;
	}

	list::list(container::ptr parent, const rectangle& rect) : widget(parent, rect), 
		selected_(0), prev_selected_(0)
	{
		cout << "list::list()" << endl;
		focusable(true);
	}

	list::~list()
	{
	}

	void list::handle_event(event::ptr e)
	{
		// handle list_item selection and clicking here...	
		switch (e->type())
		{
			case event::key_up:
			{
				key_event::ptr ke = boost::shared_static_cast<key_event>(e);
				switch ( ke->key() )
				{
					case up_arrow:
						cout << "list::handle_event() - up" << endl;
						if (selected_ > 0)
						{
							selected_--;
							redraw(rect_);
						}
						return;
						break;
					case down_arrow:
						cout << "list::handle_event() - down" << endl;
						if (selected_ < items_.size()-1) 
						{
							selected_++;
							redraw(rect_);
						}
						return;
						break;
				}
			}
		}
		// fixme: do we want to call widget's handle_event ?
		// fixme: maybe handle_event should return a bool to aid in determining if the event
		// has been handled
		widget::handle_event(e); 
		parent_.lock()->handle_event(e);
	}
		
	void list::add_item(list_item::ptr item)
	{
		items_.push_back(item);
		cout << "list::add_item: items_ count=" << items_.size() << endl;
	}
}
