#ifndef STK_DRAWABLE_H
#define STK_DRAWABLE_H

#include "libstk/surface.h"
#include "libstk/rectangle.h"

namespace stk
{
	/// An abstract class defining the drawable interface.
	class drawable
	{
		private:
			
		protected:
			
		public:
			drawable() { }
			virtual ~drawable() { };

			/// Retrieve the surface used in the application
			virtual boost::shared_ptr<stk::surface> surface() = 0;

			/// Redraw the area in rect the next time draw is called.  
			/// \param rect The area to be redrawn 
			/// See container for specifics regarding behavior.  The default 
			/// implementation does nothing as leaf widgets redraw themselves 
			/// entirely.
			virtual void redraw(const rectangle& rect) { }
			
	}; // class drawable
} // namespace stk

#endif
