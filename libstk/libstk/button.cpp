#include <iostream>
#include "button.h"
#include "widget.h"

// FIXME: should we include these like this, or maybe in one events.h file for convenience ?
#include "event.h"
#include "key_event.h"
#include "mouse_event.h"
#include "keycode.h"


using std::cout;
using std::endl;

namespace stk
{
	button::button(boost::shared_ptr<container> parent, std::string label,
					int x, int y, int width, int height) : widget(parent)
	{
		cout << "button::button()" << endl;
	}

	button::~button()
	{
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
						parent_.get()->handle_event(e);
				}
				break;
			}
			default:
				//FIXME: make_shared first ?... the parent should never be null...
				parent_.get()->handle_event(e);
		}
	}
}
