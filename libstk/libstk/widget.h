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
#include <map>
#include <list>
#include <string>
#include <sigc++/signal_system.h>
#include <boost/shared_ptr.hpp>
#include <SDL/SDL.h>

namespace stk
{
class widget;
class app;
typedef boost::shared_ptr<widget> widget_ptr;
typedef std::map<std::string, widget_ptr> widget_map;

class widget : public SigC::Object
{
public:
    widget(app* app,widget* parent);
    virtual ~widget();

    void parent_app(app *val)
    {
        parent_app_ = val;
    }
    app *parent_app(void)
    {
        return parent_app_;
    }

    // the new signal system
    SigC::Signal1<void, widget&>         on_focus;     // void sig(widget&)
    SigC::Signal1<void, widget&>         on_unfocus;   // void sig(widget&)
    SigC::Signal1<void, widget&>         on_activate;  // void sig(widget&)
    SigC::Signal2<bool, widget&, SDLKey> on_keypress;  // bool sig(widget&, SDLKey)

protected:
    app *parent_app_;
    widget *parent;
};

} // namespace stk

#endif
