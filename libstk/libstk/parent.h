#ifndef STK_PARENT_H
#define STK_PARENT_H

#include <boost/shared_ptr.hpp>
#include "event_handler.h"
#include "drawable.h"

namespace stk
{
	class widget;

	class parent : public event_handler, public drawable
	{
		private:
			
		protected:
			
		public:
			parent() { };
			virtual ~parent() { };
			virtual void add_child(boost::shared_ptr<widget>) = 0;
			virtual boost::shared_ptr<widget> focus_next() = 0;
			virtual boost::shared_ptr<widget> focus_prev() = 0;

	}; // class parent
} // namespace stk

#endif
