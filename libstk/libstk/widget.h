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
#include <boost/signal.hpp>
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
			widget(boost::weak_ptr<container> parent);
			~widget();

			/*
				 The signals currently all return void, however, it may be useful
				 to define an AND combiner functor and have all slots return bool.  
				 Then if any of the slots fail, the signal returns false, otherwise
				 it returns true.  We might do something like:

				 struct sig_and 
				 { 
				   bool operator ()(InputIterator first, InputIterator last) const
				   {
				     if (first == last) return true; // no connections, so we didn't fail
				     while (first != last)
						 {
				       if (! *first) return false; // one failed, so return false
				       ++first;
				     }
				     return true; // no connections failed
				   }
				 }

				 boost::signal<bool (), sig_and> sig;

				 we can of course let the "user" specify all this... but as a widget set,
				 I think it makes since to define a standard interface, and I doubt
				 users are going to want to receive a vector of return values...

				 sorry for the spewage in Marc's beautiful code... :-)

*/
			boost::signal<void ()> on_focus;
			boost::signal<void ()> on_unfocus;
			boost::signal<void (SDLKey)> on_keydown;
			boost::signal<void (SDLKey)> on_keyup;

			/*
			//some cool things we can do with boost::signals

			VideoPanel my_vp(new stk::video_panel);
			Button my_button(new stk::button);

			// we don't need any arguments in general for slots since we can connect
			// functors, and specific object member variables
			my_button.on_click.connect(boost::bind(&stk::video_panel::play, my_vp));

			// perhaps we want to rewind and start over on_click (in that order)
			my_button.on_click.connect(0, boost::bind(&stk::video_panel::rewind, my_vp));
			my_button.on_click.connect(1, boost::bind(&stk::video_panel::play, my_vp));

*/


		protected:
			boost::weak_ptr<container> parent_;
	};

} // namespace stk

#endif
