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
			// FIXME: what can we do to use container::ptr, etc here ?
			widget(boost::shared_ptr<container> parent, const rectangle& rect=rectangle(0,0,0,0));
			widget(parent::ptr parent, const rectangle& rect=rectangle(0,0,0,0));
			parent::weak_ptr parent_;
			
			rectangle rect_;
			
			// widget attributes
			bool redraw_;
			bool focusable_;
			int tab_;
			
			// drawing related attributes
			bool active_;
			bool focused_;
			bool hover_;
			
		public:
			virtual ~widget();

			virtual void draw(boost::shared_ptr<stk::surface> surface);
			virtual bool contains(int x, int y) { return rect_.contains(x, y); }

			// widget attribute accessor methods
			bool redraw() { return redraw_; }
			virtual void redraw(bool val) { redraw_ = val; }
			bool focusable() { return focusable_; }
			void focusable(bool val) { focusable_ = val; }
			int tab() { return tab_; }
			void tab(int val) { tab_ = val; }
			bool active() { return active_; }
			void active(bool val) { active_ = val; }
			bool focused() { return focused_; }
			void focused(bool val) { focused_ = val; }
			bool hover() { return hover_; }
			void hover(bool val) { hover_ = val; }
			
			// event_handler interface
			virtual void handle_event(event::ptr e);

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
			//some cool things we can do with boost::signals

			VideoPanel my_vp(new stk::video_panel);
			Button my_button(new stk::button);

			// we don't need any arguments in general for slots since we can connect
			// functors, and specific object member methods
			my_button.on_click.connect(boost::bind(&stk::video_panel::play, my_vp));

			// perhaps we want to rewind and start over on_click (in that order)
			my_button.on_click.connect(0, boost::bind(&stk::video_panel::rewind, my_vp));
			my_button.on_click.connect(1, boost::bind(&stk::video_panel::play, my_vp));
			*/
			
	}; // class widget
} // namespace stk

#endif
