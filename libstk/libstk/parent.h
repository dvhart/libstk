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
			virtual boost::smart_ptr<widget> focus_next();
			virtual boost::smart_ptr<widget> focus_prev();

	}; // class parent
} // namespace stk

#endif
