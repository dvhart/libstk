#ifndef STK_DRAWABLE_H
#define STK_DRAWABLE_H

#include "surface.h"

namespace stk
{
	class drawable
	{
		// FIXME have typedefs here?
		private:
			
		protected:
			
		public:
			drawable() { };
			virtual ~drawable() { };
			virtual boost::shared_ptr<stk::surface> surface() = 0;
			
	}; // class drawable
} // namespace stk

#endif

