/***************************************************************************
                          stk_widget.h  -  description
                             -------------------
    begin                : Sat Apr 27 2002
    copyright            : (C) 2002 by Darren Hart
    email                : dvhart@byu.edu
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef STK_WIDGET_H
#define STK_WIDGET_H

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <SDL/SDL.h>

namespace stk
{
class widget;
class container;
class state;
typedef boost::shared_ptr<widget> Twidget_ptr;
typedef boost::shared_ptr<container> Tcontainer_ptr;
	
class widget 
{
public:
	virtual bool is_container() { return false;} 
	widget(boost::weak_ptr<state> state,boost::weak_ptr<container> parent);
	~widget();

    /* DEMO , not really implemented (CHANGE TO BOOST::SIGNALS!)
    SigC::Signal1<void, widget&>         on_focus;     // void sig(widget&)
    SigC::Signal1<void, widget&>         on_unfocus;   // void sig(widget&)
	SigC::Signal2<bool, widget&, SDLKey> on_keydown;  // bool sig(widget&, SDLKey)
	SigC::Signal2<bool, widget&, SDLKey> on_keyup;  // bool sig(widget&, SDLKey)
    SigC::Signal2<bool, widget&, SDLKey> on_keypress;  // bool sig(widget&, SDLKey)
	*/
protected:
    boost::weak_ptr<state> parent_state_;
    boost::weak_ptr<container> parent_;
};

} // namespace stk

#endif
