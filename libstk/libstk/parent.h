#ifndef STK_PARENT_H
#define STK_PARENT_H

#include <boost/shared_ptr.hpp>
#include "libstk/event_handler.h"
#include "libstk/drawable.h"

namespace stk
{
	class widget;

	/// A class declaring the parent interface.
	/// I think this class is superfluous and its interface should be moved
	/// down to widget. --dvhart
	class parent : public event_handler, public drawable
	{
		public:
			typedef boost::shared_ptr<parent> ptr;
			typedef boost::weak_ptr<parent> weak_ptr;

		private:
			
		protected:
			parent() { };
			
		public:
			virtual ~parent() { };
			
			/********** PARENT INTERFACE **********/
			/// Return the parent, widgets all return a valid pointer.
			/// Return an empty pointer here as not all derived classes have parents. 
			/// FIXME: the parent class should really be called component, then rename this
			virtual parent::ptr get_parent() { return parent::ptr(); }
			/// Retrieve the next focusable widget.
			/// Implemented by container, here we just return an empty pointer.
			virtual boost::shared_ptr<widget> focus_next() { return boost::shared_ptr<widget>(); }
			/// Retrieve the next focusable widget.
			/// Implemented by container, here we just return an empty pointer.
			virtual boost::shared_ptr<widget> focus_prev() { return boost::shared_ptr<widget>(); }
			/// Add a child.
			/// This does nothing by default and is overridden in container.
			virtual void add(boost::shared_ptr<widget> item) { }
			/// Remove a child 
			/// This does nothing by default and is overridden in container.
			virtual void remove(boost::shared_ptr<widget> item) { }
			/********** END PARENT INTERFACE **********/
			
	}; // class parent
} // namespace stk

#endif
