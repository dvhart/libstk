#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <string>

#include "libstk/parent.h"
#include "libstk/application.h"
#include "libstk/widget.h"
#include "libstk/container.h"

using std::cout;
using std::endl;

namespace stk
{

	widget::widget(parent::ptr parent, const rectangle& rect)
		: parent_(parent), rect_(rect), focusable_(false), 
	    pressed_(false), focused_(false), hover_(false), tab_(0), frame_(0)
	{
		cout << "widget::widget(parent)" << endl;
	}

	widget::~widget()
	{
	}
	
	// event_handler interface - default back to parent
	void widget::handle_event(event::ptr e)
	{
		cout << "widget::handle_event()" << endl;

		// default handlers for focus/un_focus mouse_enter/leave events
		// a derived widget should call widget::handle_event() for these basic events
		switch (e->type())
		{
			case event::focus:
				if (focusable_) 
				{
					focused_ = true;
					redraw(rect_);
				}
				break;
			case event::un_focus:
				if (focusable_)
				{
					focused_ = false;
					pressed_ = false;
					redraw(rect_);
				}
				break;
			case event::mouse_enter:
				hover_ = true;
				redraw(rect_);
				break;
			case event::mouse_leave:
				hover_ = false;
				pressed_ = false;
				redraw(rect_);
				break;
		}

		// do not call parent->handle_event() here, the derived widget is responsible
		// for that if they so desire (ie they don't handle an event)

		cout << "widget::handle_event() done" << endl;
	}

	// drawable interface
	surface::ptr widget::surface()
	{
		// MSTR broken!
		return parent_.lock()->surface(); 
	}
	
	void widget::redraw(const rectangle& rect)
	{
		//cout << "widget::redraw() - rectangle: " << rect << endl;
		parent_.lock()->redraw(rect);
	}
	
	// parent interface
	widget::ptr widget::focus_next()
	{ 
		// MSTR broken!
		return parent_.lock()->focus_next(); 
	}

	widget::ptr widget::focus_prev()
	{ 
		// MSTR broken!
		return parent_.lock()->focus_prev(); 
	}

}
