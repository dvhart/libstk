#include <iostream>
#include "button.h"
#include "widget.h"
#include "theme.h"

// FIXME: should we include these like this, or maybe in one events.h file for convenience ?
#include "event.h"
#include "key_event.h"
#include "mouse_event.h"
#include "keycode.h"

using std::cout;
using std::endl;

namespace stk
{

	button::ptr button::create(container::ptr parent, const std::wstring label, 
		const rectangle& rect)
	{
		button::ptr new_button(new button(parent, label, rect));
		parent->add_child(new_button);
		return new_button;
	}

	button::button(boost::shared_ptr<container> parent, const std::wstring label, 
		const rectangle& rect) : widget(parent, rect), label_(label)
	{
		cout << "button::button()" << endl;
	}

	button::~button()
	{
	}

	void button::draw(boost::shared_ptr<stk::surface> surface)
	{
		theme::instance()->draw_button(rect_, label_, active_, focused_, hover_);
	}
			
	// event_handler interface
	void button::handle_event(event::ptr e)
	{
		//cout << "button::handle_event()" << endl;
		
		switch (e->type())
		{
			case key_up:
			{
				key_event::ptr ke = boost::shared_static_cast<key_event>(e);
				switch ( ke->key() )
				{
					case key_enter:
						cout << "button::handle_event() - emitting signal on_click()" << endl;
						on_click();
						break;
					default:
						//mstr: broken in boost_1_30_0 FIXME
						boost::make_shared(parent_)->handle_event(e);
				}
				break; // key_up
			}
			// FIXME: this stuff should be moved to application or state I think
			case mouse_motion:
			{
				//boost::make_shared(parent_)->handle_event(e);
				break; // mouse_motion
			}
			case mouse_down:
			{
				mouse_event::ptr me = boost::shared_static_cast<mouse_event>(e);
				if (!active())
				{
					active(true);
					redraw(rect_);
				}
				break; // mouse_down
			}
			case mouse_up:
			{
				mouse_event::ptr me = boost::shared_static_cast<mouse_event>(e);
				if (active())
				{
					active(false);
					redraw(rect_);
					on_click();
				}
				break; // mouse_up
			}
			default:
				//mstr: broken in boost_1_30_0 FIXME
				boost::make_shared(parent_)->handle_event(e);
		}
	}
}
