#ifndef STK_CONTAINER_H
#define STK_CONTAINER_H

#include <map>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "application.h"
#include "widget.h"

namespace stk
{
	class container : public stk::widget
	{
		public:
			typedef boost::shared_ptr<container> ptr;
			typedef boost::weak_ptr<container> weak_ptr;

		private:
			std::vector<widget::ptr> children_;
			
		protected:
			container(application::ptr parent);
			
		public:
			container(container::ptr parent);
			~container();
			virtual bool is_container() { return true; }
			// FIXME :carter: implement all this
			virtual widget::weak_ptr get_active_child()  // called when the tree parser for widget cycling switches focus to a container
			{ return  *children_.begin(); } // default behaviour for a container is to switch into the FIRST child first 
			
			// event_handler interface
			//virtual void handle_event(boost::shared_ptr<stk::event> e);

			// drawable interface
			//virtual boost::shared_ptr<stk::surface> surface() 
			//{ return parent_.get()->surface(); }

			// parent interface
			// FIXME
			virtual widget::ptr focus_next()
			{ return *children_.begin(); }
			
			virtual widget::ptr focus_prev()
			{ return *children_.begin(); }
			
			virtual void add_child(widget::ptr w)
			{ children_.push_back(w); }
			
			/// container specific methods
			/// \todo arent these redundant
			void add(widget::ptr item);
			void remove(widget::ptr item);
	};

} // namespace stk

#endif
