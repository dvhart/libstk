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
		focusable(true);
	}

	button::~button()
	{
	}

	void button::draw(surface::ptr surface)
	{
		theme::instance()->draw_button(rect_, label_, pressed_, focused_, hover_);
	}
			
	// event_handler interface
	void button::handle_event(event::ptr e)
	{
		//cout << "button::handle_event()" << endl;	
		switch (e->type())
		{
			case event::key_up:
			{
				// FIXME: where should default actions be taken care of ?
				// next and prev are currently in App, so activate (enter) should
				// probably be with them...
				key_event::ptr ke = boost::shared_static_cast<key_event>(e);
				switch ( ke->key() )
				{
					case key_enter:
						if (pressed_)
						{
							cout << "button::handle_event() - emitting signal on_click()" << endl;
							pressed_ = false;
							redraw(rect_);
							on_release();
						}
						return;
						break;
				}
				break; // key_up
			}
			case event::key_down:
			{
				key_event::ptr ke = boost::shared_static_cast<key_event>(e);
				switch ( ke->key() )
				{
					case key_enter:
						cout << "button::handle_event() - emitting signal on_click()" << endl;
						pressed_ = true;
						redraw(rect_);
						on_press();
						return;
						break;
				}
				break; // key_down
			}
			// FIXME: this stuff should be moved to application or state I think
			case event::mouse_motion:
			{
				break; // mouse_motion
			}
			case event::mouse_down:
			{
				mouse_event::ptr me = boost::shared_static_cast<mouse_event>(e);
				if (!pressed())
				{
					pressed_ = true;
					redraw(rect_);
					on_press();
				}
				return;
				break; // mouse_down
			}
			case event::mouse_up:
			{
				mouse_event::ptr me = boost::shared_static_cast<mouse_event>(e);
				if (pressed())
				{
					pressed_ = false;
					redraw(rect_);
					on_release();
				}
				return;
				break; // mouse_up
			}
		}
		// fixme: do we want to be calling widget::handle_event ?
		widget::handle_event(e); 
		parent_.lock()->handle_event(e);
	}

}
