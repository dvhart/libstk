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
			decorator(container::ptr parent, const rectangle& rect) 
				: widget(parent, rect)
			{
			}

		public:
			virtual ~decorator() { }
			
			/********** EVENT HANDLER INTERFACE **********/
			/// handle_event is an upward method, do not use it to pass events down
			/// or infinite recursion will result.
			virtual void handle_event(event::ptr e) 
			{ 
				cout << "decorator::handle_event()" << endl;
				parent_.lock()->handle_event(e);
			}
			/********** END EVENT HANDLER INTERFACE **********/

			/********** DRAWABLE INTERFACE **********/
			/// surface is an upward method, do not call down from within surface
			/// or infinite recursion will result.
			//virtual surface::ptr surface() { return component_->surface(); }
			virtual void draw(surface::ptr surface, const rectangle& clip_rect = rectangle())
			{ 
				if (clip_rect.empty())
					component_->draw(surface, rect_);
				else
					component_->draw(surface, clip_rect);
			}
			
			/// redraw is an upward method, do not call down from within redraw
			/// or infinite recursion will result.
			//virtual void redraw(const rectangle& rect) { parent_.lock()->redraw(rect); }
			/********** END DRAWABLE INTERFACE **********/
			
			/********** PARENT INTERFACE **********/
			virtual widget::ptr focus_next() { return component_->focus_next(); }
			virtual widget::ptr focus_prev() { return component_->focus_prev(); }
			/// Assign the first item added to component_.
			/// Subsequent adds get passed along to component_.
			virtual void add(widget::ptr item) 
			{ 
				if (!component_)
					component_ = item;
				else
					component_->add(item); 
			}
			virtual void remove(widget::ptr item) { component_->remove(item); }
			/********** END PARENT INTERFACE **********/
			
			/********** WIDGET INTERFACE **********/
			/// FIXME: I think we may want this to just return true...
			virtual bool is_container() { return component_->is_container(); }
			/// FIXME: these should all be virtual too!!!
			/// Return a bool indicating if component_ is focusable
			bool focusable() { return component_->focusable(); }
			/// Set the focusable property of component_
			void focusable(bool val) { component_->focusable(val); }
			/// Return the tab index of component_
			/// \todo this is currently not implemented
			int tab() { return component_->tab(); }
			/// Set the tab index of component_ 
			/// \todo this is currently not implemented
			void tab(int val) { component_->tab(val); }
			/// Return the pressed property of component_
			bool pressed() { return component_->pressed(); }
			/// Return the focused property of component_
			bool focused() { return component_->focused(); }
			/// Return the hover property of component_
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
			
			/********** COMPOSITE INTERFACE **********/
			// FIXME: make these throw an exception or something
			virtual widget::ptr widget_at(int x, int y) { return component_->widget_at(x, y); }
			virtual void delegate_mouse_event(mouse_event::ptr me) { component_->delegate_mouse_event(me); }
			virtual widget::ptr get_active_child() { return component_->get_active_child(); }
			virtual rectangle redraw_rect() { return component_->redraw_rect(); } 
			/********** END COMPOSITE INTERFACE **********/

	};
}

#endif
