#ifndef STK_DRAWABLE_H
#define STK_DRAWABLE_H

#include "surface.h"
#include "rectangle.h"

namespace stk
{
	class drawable
	{
		private:
			
		protected:
			rectangle redraw_rect_; // only containers really need it...
			
		public:
			drawable() { }
			virtual ~drawable() { };\
			virtual boost::shared_ptr<stk::surface> surface() = 0;
			virtual void redraw(const rectangle& rect=rectangle(0,0,0,0)) { }
			
	}; // class drawable
} // namespace stk

#endif

