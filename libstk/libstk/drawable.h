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
			bool redraw_; // should this be in drawable ?
			rectangle redraw_rect_;
			
		public:
			drawable() : redraw_(true) { };
			virtual ~drawable() { };
			virtual boost::shared_ptr<stk::surface> surface() = 0;
			virtual bool redraw() { return redraw_; }
			virtual void redraw(bool val, const rectangle& rect=rectangle(0,0,0,0)) { }
			
	}; // class drawable
} // namespace stk

#endif

