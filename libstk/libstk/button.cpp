#include <iostream>
#include "button.h"
#include "widget.h"

// FIXME: should we include these like this, or maybe in one events.h file for convenience ?
#include "event.h"
#include "key_event.h"
#include "mouse_event.h"
#include "keycode.h"
#include "theme.h"

using std::cout;
using std::endl;

namespace stk
{

	button::ptr button::create(container::ptr parent, std::string label, const rectangle& rect)
	{
		button::ptr new_button(new button(parent, label, rect));
		parent->add_child(new_button);
		return new_button;
	}

	button::button(boost::shared_ptr<container> parent, std::string label,
					const rectangle& rect) : widget(parent), rect_(rect)
	{
		cout << "button::button()" << endl;
	}

	button::~button()
	{
	}

	void button::draw(boost::shared_ptr<stk::surface> surface)
	{
		cout << "button::draw()" << endl;
		theme::instance()->draw_button(rect_);
	}
			
	// event_handler interface
	void button::handle_event(boost::shared_ptr<stk::event> e)
	{
		cout << "button::handle_event()" << endl;
		
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
						//FIXME: make_shared first ?... the parent should never be null...
						//mstr: broken in boost_1_30_0 FIXME
						//parent_.get()->handle_event(e);
					;
				}
				break;
			}
			default:
				//FIXME: make_shared first ?... the parent should never be null...
				//mstr: broken in boost_1_30_0 FIXME
				//parent_.get()->handle_event(e);
			;
		}
	}
}
