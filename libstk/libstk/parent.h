#ifndef STK_PARENT_H
#define STK_PARENT_H

#include "event_handler.h"
#include "drawable.h"

namespace stk
{
	class parent : public event_handler, public drawable
	{
		private:
			
		protected:
			
		public:
			parent();
			~parent();
			virtual boost::shared_ptr<widget> focus_next();
			virtual boost::shared_ptr<widget> focus_prev();
			virtual void add_child(boost::shared_ptr<widget>);

	}; // class parent
} // namespace stk

#endif
