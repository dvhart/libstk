/******************************************************************************
 *    FILENAME: widget.cpp
 * DESCRIPTION: Widget abstract base class implementation.
 *     AUTHORS: Darren Hart, Marc Straemke, Dirk Hoerner
 *  START DATE: 08/Sep/2003  LAST UPDATE: 14/May/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available
 *              with the source as license.txt or at 
 *              http://www.libstk.org/index.php?page=docs/license
 *****************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <string>

#include "libstk/application.h"
#include "libstk/component.h"
#include "libstk/container.h"
#include "libstk/key_event.h"
#include "libstk/mouse_event.h"
#include "libstk/widget.h"

using std::cout;
using std::endl;

namespace stk
{

	widget::widget(component::ptr parent, const rectangle& rect)
		: parent_(parent), rect_(rect), focusable_(false), 
	    pressed_(false), focused_(false), hover_(false), tab_(0), frame_(0)
	{
		cout << "widget::widget(parent)" << endl;
	}

	widget::~widget()
	{
		cout << "widget::~widget()" << endl;
	}
	
	// event_handler interface - default back to parent
	void widget::handle_event(event::ptr e)
	{
		//cout << "widget::handle_event()" << endl;

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
			case event::key_up:
				key_event::ptr ke = boost::shared_static_cast<key_event>(e);
				bool ret = on_keyup(ke->key());
				cout << "widget::handle_event() - on_keyup returned: " << ((ret) ? "true" : "false") << endl;
				break;
		}

		parent_.lock()->handle_event(e);
	}

	// drawable interface
	surface::ptr widget::surface()
	{
		// MSTR: broken!, we shouldnt use Smart pointers like this,VERY unsafe!
		return parent_.lock()->surface(); 
	}
	
	void widget::redraw(const rectangle& rect)
	{
		//cout << "widget::redraw() - rectangle: " << rect << endl;
		parent_.lock()->redraw(rect);
	}
	
}
