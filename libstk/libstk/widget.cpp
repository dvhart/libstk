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

#include "application.h"
#include "widget.h"
#include "container.h"

using std::cout;
using std::endl;

namespace stk
{

	widget::widget(boost::shared_ptr<container> parent) : parent_(parent)
	{
		cout << "widget::widget(container)" << endl;
		cout << "widget::widget(container) - parent pointer is " << std::hex << parent.get() << endl;
		//parent->add_child(boost::shared_ptr<widget>(this));
	}

	widget::widget(boost::shared_ptr<parent> parent) : parent_(parent)
	{
		cout << "widget::widget(parent)" << endl;
		cout << "widget::widget(parent) - parent pointer is " << std::hex << parent.get() << endl;
	}

	widget::~widget()
	{
	}

	// event_handler interface - default back to parent
	void widget::handle_event(boost::shared_ptr<stk::event> e)
	{
		cout << "widget::handle_event()" << endl;
		// FIXME: what is the best way to access the weak_ptr parent_
		// should we make it shared first ? (see all other uses torc/te)
		// MSTR:broken! (NO weak_ptr::GET in boost 1_30_0) FIXME
		//if (parent_.get() == 0)
		//{
			// throw something
//			cout << "widget::handle_event() - null parent_ pointer" << endl;
	//	}
		//parent_.get()->handle_event(e);
	}

	// drawable interface
	boost::shared_ptr<stk::surface> widget::surface()
	{
		// MSTR broken!
		//return parent_.get()->surface(); 
	}

	// parent interface
	boost::shared_ptr<widget> widget::focus_next()
	{ 
		// MSTR broken!
		//return parent_.get()->focus_next(); 
	}

	boost::shared_ptr<widget> widget::focus_prev()
	{ 
		// MSTR broken!
		//return parent_.get()->focus_prev(); 
	}

}
