#ifndef STK_DECORATOR_H
#define STK_DECORATOR_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "libstk/widget.h"
#include "libstk/container.h"
#include "libstk/rectangle.h"

namespace stk
{
	/// Abstract base class for concrete decorators.
	/// A decorator forwards all widget requests to its member widget component_.
	/// Concrete decorators add responsibilty to individual widget instances, 
	/// such as a the scroll panel decorator.
	class decorator : public widget
	{
		public:
			typedef boost::shared_ptr<decorator> ptr; 
			typedef boost::weak_ptr<decorator> weak_ptr;
			
		private:

		protected:
			/// the widget decorated by this decorator
			widget::ptr component_;
			decorator(container::ptr parent, const rectangle& rect, 
					widget::ptr component) : widget(parent, rect), component_(component)
			{
			}

		public:
			virtual ~decorator() { }
			
			/********** EVENT HANDLER INTERFACE **********/
			virtual void handle_event(event::ptr e) { component_->handle_event(e); }
			/********** END EVENT HANDLER INTERFACE **********/

			/********** DRAWABLE INTERFACE **********/
			virtual surface::ptr surface() { return component_->surface(); }
			virtual void draw(surface::ptr surface) { component_->draw(surface); }
			virtual void redraw(const rectangle& rect) { component_->redraw(rect); }
			/********** END DRAWABLE INTERFACE **********/
			
			/********** PARENT INTERFACE **********/
			virtual widget::ptr focus_next() { return component_->focus_next(); }
			virtual widget::ptr focus_prev() { return component_->focus_prev(); }
			/********** END PARENT INTERFACE **********/
			
			/********** WIDGET INTERFACE **********/
			rectangle rect() { return component_->rect(); }
			bool contains(int x, int y) { return component_->contains(x, y); }
			bool intersects(const rectangle& rect) { return component_->intersects(rect); }
			virtual bool is_container() { return component_->is_container(); }

			// widget attribute accessor methods
			// FIXME: have the setters return bool ? 
			// (ie label would return false for a focus(true) call) ?
			/// Return a bool indicating if the widget is focusable
			bool focusable() { return component_->focusable(); }
			/// Set the focusable property of the widget
			void focusable(bool val) { component_->focusable(val); }
			/// Return the tab index of the widget
			/// \todo this is currently not implemented
			int tab() { return component_->tab(); }
			/// Set the tab index of the widget
			/// \todo this is currently not implemented
			void tab(int val) { component_->tab(val); }
			/// Return the pressed property of the widget
			bool pressed() { return component_->pressed(); }
			/// Return the focused property of the widget
			bool focused() { return component_->focused(); }
			/// Return the hover property of the widget
			bool hover() { return component_->hover(); } 

			// FIXME: hmmm... how to map these to the component signals... ???
			// is it even necessary ?
			/*
			boost::signal<bool (), combiner::logical_and<bool> > on_focus;
			boost::signal<bool (), combiner::logical_and<bool> > on_unfocus;
			boost::signal<bool (), combiner::logical_and<bool> > on_mouse_enter;
			boost::signal<bool (), combiner::logical_and<bool> > on_mouse_leave;
			boost::signal<bool (stk::keycode), combiner::logical_and<bool> > on_keydown;
			boost::signal<bool (stk::keycode), combiner::logical_and<bool> > on_keyup;
			*/
			/********** END WIDGET INTERFACE **********/

	};
}

#endif
