#ifndef STK_WIDGET_H
#define STK_WIDGET_H

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/signal.hpp>

#include "libstk/parent.h"
#include "libstk/keycode.h"
#include "libstk/combiners.h"

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
			widget(boost::shared_ptr<container> parent, 
				const rectangle& rect=rectangle(0,0,0,0));
			widget(parent::ptr parent, const rectangle& rect=rectangle(0,0,0,0));
			parent::weak_ptr parent_;
			
			rectangle rect_;
			
			// widget attributes
			bool focusable_;
			int tab_;
			
			// drawing related attributes
			bool pressed_; // does this belong in button only ?
			               // it is nice to have it here so that handle_event can handle
										 // the common cases, see handle_event()
			bool focused_;
			bool hover_;
			
		public:
			virtual ~widget();

			/********** EVENT HANDLER INTERFACE **********/
			virtual void handle_event(event::ptr e);
			/********** END EVENT HANDLER INTERFACE **********/

			/********** DRAWABLE INTERFACE **********/
			virtual surface::ptr surface(); 
			virtual void draw(surface::ptr surface);
			virtual void redraw(const rectangle& rect);
			/********** END DRAWABLE INTERFACE **********/
			
			/********** PARENT INTERFACE **********/
			/// Pass call along to parent
			virtual widget::ptr focus_next();
			/// Pass call along to parent
			virtual widget::ptr focus_prev();
			/********** END PARENT INTERFACE **********/
			
			/********** WIDGET INTERFACE **********/
			rectangle rect() { return rect_; }
			bool contains(int x, int y) { return rect_.contains(x, y); }
			bool intersects(const rectangle& rect) { return rect_.intersects(rect); }
			virtual bool is_container() { return false; }

			// widget attribute accessor methods
			// FIXME: have the setters return bool ? 
			// (ie label would return false for a focus(true) call) ?
			/// Return a bool indicating if the widget is focusable
			bool focusable() { return focusable_; }
			/// Set the focusable property of the widget
			void focusable(bool val) { focusable_ = val; }
			/// Return the tab index of the widget
			/// \todo this is currently not implemented
			int tab() { return tab_; }
			/// Set the tab index of the widget
			/// \todo this is currently not implemented
			void tab(int val) { tab_ = val; }
			/// Return the pressed property of the widget
			bool pressed() { return pressed_; }
			/// Return the focused property of the widget
			bool focused() { return focused_; }
			/// Return the hover property of the widget
			bool hover() { return hover_; } 

			boost::signal<bool (), combiner::logical_and<bool> > on_focus;
			boost::signal<bool (), combiner::logical_and<bool> > on_unfocus;
			boost::signal<bool (), combiner::logical_and<bool> > on_mouse_enter;
			boost::signal<bool (), combiner::logical_and<bool> > on_mouse_leave;
			boost::signal<bool (stk::keycode), combiner::logical_and<bool> > on_keydown;
			boost::signal<bool (stk::keycode), combiner::logical_and<bool> > on_keyup;
			/********** END WIDGET INTERFACE **********/

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
