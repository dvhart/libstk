/***************************************************************************
	stk_widget.cpp  -  description
	-------------------
begin                : Sat Apr 27 2002
copyright            : (C) 2002 by Darren Hart
email                : dvhart@byu.edu
 ***************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <string>

#include "parent.h"
#include "application.h"
#include "widget.h"
#include "container.h"

using std::cout;
using std::endl;

namespace stk
{

	widget::widget(container::ptr parent, const rectangle& rect)
		: parent_(parent), rect_(rect), focusable_(false), 
	    active_(false), focused_(false), hover_(false), tab_(0)
	{
		cout << "widget::widget(container)" << endl;
		//parent->add_child(widget::ptr(this)); // why is this not being called ?
	}

	widget::widget(parent::ptr parent, const rectangle& rect)
		: parent_(parent), rect_(rect), focusable_(false), 
	    active_(false), focused_(false), hover_(false), tab_(0)
	{
		cout << "widget::widget(parent)" << endl;
	}

	widget::~widget()
	{
	}

	void widget::draw(surface::ptr surface)
	{
		redraw_ = false;
	}
		
	// event_handler interface - default back to parent
	void widget::handle_event(event::ptr e)
	{
		cout << "widget::handle_event()" << endl;
		// MSTR:broken! (NO weak_ptr::GET in boost 1_30_0) FIXME
		if (!boost::make_shared(parent_))
		{
			// throw something
			cout << "widget::handle_event() - null parent_ pointer" << endl;
		}
		boost::make_shared(parent_)->handle_event(e);
	}

	// drawable interface
	surface::ptr widget::surface()
	{
		// MSTR broken!
		return boost::make_shared(parent_)->surface(); 
	}
	
	void widget::redraw(bool val, const rectangle& rect)
	{
		cout << "widget::redraw() - rectangle " << rect.x1() << "," << rect.y1() << " " << rect.x2() << "," << rect.y2() << endl;
		redraw_ = val;
		redraw_rect_ += rect;
		make_shared(parent_)->redraw(true, rect);
	}
	
	// parent interface
	widget::ptr widget::focus_next()
	{ 
		// MSTR broken!
		return boost::make_shared(parent_)->focus_next(); 
	}

	widget::ptr widget::focus_prev()
	{ 
		// MSTR broken!
		return boost::make_shared(parent_)->focus_prev(); 
	}

}
