#ifndef STK_CONTAINER_H
#define STK_CONTAINER_H

#include <map>
#include <vector>
#include <boost/shared_ptr.hpp>
#include "widget.h"

namespace stk
{
	class container : public stk::widget
	{
		private:
			std::vector<boost::shared_ptr<stk::widget> > children_;
			
		protected:
			container() { }; // empty constructor, needed for creating derived classes ?
			
		public:
			//container(boost::weak_ptr<stk::container> parent);
			container(boost::shared_ptr<stk::container> parent);
			~container();
			virtual bool is_container() { return true;} 
			virtual boost::weak_ptr<stk::widget> get_active_child()  // called when the tree parser for widget cycling switches focus to a container
			{ return  *children_.begin(); } // default behaviour for a container is to switch into the FIRST child first 
			
			// event_handler interface
			//virtual void handle_event(stk::event& e);

			// drawable interface
			//virtual boost::shared_ptr<stk::surface> surface() 
			//{ return parent_.get()->surface(); }

			// parent interface
			// FIXME
			virtual boost::shared_ptr<widget> focus_next()
			{ return *children_.begin(); }
			
			virtual boost::shared_ptr<widget> focus_prev()
			{ return *children_.begin(); }
			
			virtual void add_child(boost::shared_ptr<widget> w)
			{ children_.push_back(w); }
			
			// container specific methods
			void add(boost::shared_ptr<stk::widget> item);
			void remove(boost::shared_ptr<stk::widget> item);
	};

} // namespace stk

#endif
