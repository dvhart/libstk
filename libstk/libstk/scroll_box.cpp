#include "libstk/scroll_box.h"
#include "libstk/event.h"
#include "libstk/key_event.h"

namespace stk
{
	scroll_box::ptr scroll_box::create(container::ptr parent, 
			const rectangle& rect)
	{
		scroll_box::ptr new_scroll_box(
				new scroll_box(parent, rect));
		parent->add(new_scroll_box);
		return new_scroll_box;
	}
	
	scroll_box::scroll_box(container::ptr parent, 
			const rectangle& rect) : container(parent, rect), vrange_(0), 
			hrange_(0), vposition_(0), hposition_(0)
	{
		focusable_ = true;
	}
			
	scroll_box::~scroll_box()
	{
	}

	void scroll_box::handle_event(event::ptr e)
	{
		//cout << "scroll_box::handle_event()" << endl;	
		switch (e->type())
		{
			case event::key_up:
			{
				// FIXME: do some error checking on children_[0]
				key_event::ptr ke = boost::shared_static_cast<key_event>(e);
				switch ( ke->key() )
				{
					case up_arrow:
						//cout << "scroll_box::handle_event() - scroll up" << endl;
						if (vposition_ > 0)
						{
							vposition_ -= 1;
							children_[0]->position(children_[0]->x1(), children_[0]->y1()+1);
							redraw(rect_);
						}
						return;
						break;
					case down_arrow:
						//cout << "scroll_box::handle_event() - scroll down" << endl;
						if (vposition_ < (vrange_ - height()))
						{
							vposition_ += 1;
							children_[0]->position(children_[0]->x1(), children_[0]->y1()-1);
							redraw(rect_);
						}
						return;
						break;
					case left_arrow:
						//cout << "scroll_box::handle_event() - scroll left" << endl;
						if (hposition_ > 0)
						{
							hposition_ -= 1;
							children_[0]->position(children_[0]->x1()+1, children_[0]->y1());
							redraw(rect_);
						}
						return;
						break;
					case right_arrow:
						//cout << "scroll_box::handle_event() - scroll right" << endl;
						if (hposition_ < (hrange_ - width()))
						{
							hposition_ += 1;
							children_[0]->position(children_[0]->x1()-1, children_[0]->y1());
							redraw(rect_);
						}
						return;
						break;
				}
				break; // key_up
			}
		}
		widget::handle_event(e); 
		parent_.lock()->handle_event(e);
	}

}

