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
			// FIXME: only container needs this
			rectangle redraw_rect_; 
			
		public:
			drawable() { }
			virtual ~drawable() { };
			virtual boost::shared_ptr<stk::surface> surface() = 0;
			virtual void redraw(const rectangle& rect) { }

			// FIXME these are only needed in container, should we provide them both ???
			virtual rectangle redraw_rect() { return redraw_rect_; } 
			
	}; // class drawable
} // namespace stk

#endif

