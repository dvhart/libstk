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

#include <string.h>
#include <malloc.h>

#include "widget.h"
#include "container.h"

using std::cout;
using std::endl;

namespace stk
{

	//widget::widget(boost::weak_ptr<container> parent) : parent_(parent)
	widget::widget(boost::shared_ptr<container> parent) : parent_(parent)
	{
		cout << "widget::widget()" << endl;
	}


	widget::~widget()
	{
	}

	// event_handler interface
	void widget::handle_event(stk::event& e) 
	{
	}

	// drawable interface
	boost::shared_ptr<stk::surface> widget::surface()
	{
		return parent_.get()->surface(); 
	}

	// parent interface
	boost::shared_ptr<widget> widget::focus_next()
	{ 
		return parent_.get()->focus_next(); 
	}

	boost::shared_ptr<widget> widget::focus_prev()
	{ 
		return parent_.get()->focus_prev(); 
	}

	void widget::add_child(boost::shared_ptr<widget>) 
	{
	}

}
