#ifndef STK_WIDGET_H
#define STK_WIDGET_H

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/signal.hpp>

#include "parent.h"
#include "keycode.h"
#include "combiners.h"

namespace stk
{
	class application;
	class container;
	class state;
	
	class widget : public parent
	{
		public:
			typedef boost::shared_ptr<widget> ptr; 
			typedef boost::weak_ptr<widget> weak_ptr;
			
		private:

		protected:
			widget(boost::shared_ptr<container> parent);
			widget(boost::shared_ptr<parent> parent);
			boost::weak_ptr<parent> parent_;
			int x_;
			int y_;
			int width_;
			int height_;
			
		public:
			virtual ~widget();

			void draw(boost::shared_ptr<stk::surface> surface);
			
			// event_handler interface
			virtual void handle_event(boost::shared_ptr<stk::event> e);

			// drawable interface
			virtual boost::shared_ptr<stk::surface> surface(); 

			// parent interface
			virtual boost::shared_ptr<widget> focus_next();
			virtual boost::shared_ptr<widget> focus_prev();
			
			boost::signal<bool (), combiner::logical_and<bool> > on_focus;
			boost::signal<bool (), combiner::logical_and<bool> > on_unfocus;
			boost::signal<bool (), combiner::logical_and<bool> > on_mouse_enter;
			boost::signal<bool (), combiner::logical_and<bool> > on_mouse_leave;
			boost::signal<bool (stk::keycode), combiner::logical_and<bool> > on_keydown;
			boost::signal<bool (stk::keycode), combiner::logical_and<bool> > on_keyup;

			/*
				 The signals currently all return void, however, it may be useful
				 to define an AND combiner functor and have all slots return bool.  
				 Then if any of the slots fail, the signal returns false, otherwise
				 it returns true.  We might do something like:
				 
				 see combiners.h
				 
				 template <typename T>
				 struct logical_and 
				 { 
					 typedef T return_type;

					 template <typename InputIterator>
				   T operator ()(InputIterator first, InputIterator last) const
				   {
				     if (first == last) return true; // no connections, so we didn't fail
				     while (first != last)
						 {
				       if (! *first) return false; // one failed, so return false
				       ++first;
				     }
				     return true; // no connections failed
				   }
				 };

				 boost::signal<bool (), sig_and> sig;

				 we can of course let the "user" specify all this... but as a widget set,
				 I think it makes since to define a standard interface, and I doubt
				 users are going to want to receive a vector of return values...

				 sorry for the spewage in Marc's beautiful code... :-)

				 
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
			
	}; // class widget
} // namespace stk

#endif
