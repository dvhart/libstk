#ifndef STK_SCROLL_DECORATOR_H
#define STK_SCROLL_DECORATOR_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "libstk/decorator.h"

namespace stk
{
	class scroll_decorator : public decorator
	{
		public:
			typedef boost::shared_ptr<scroll_decorator> ptr; 
			typedef boost::weak_ptr<scroll_decorator> weak_ptr;

		private:

		protected:
			scroll_decorator(container::ptr parent, const rectangle& rect, 
					widget::ptr component);
			/// the height of the scroll area
			int vrange_;
			/// the width of the scroll area
			int hrange_;
			/// the verticle position of the scroll area (first y value in the window)
			int vposition_;
			/// the horizontal position of the scroll area (first x value in the window)
			int hposition_;
			
		public:
			static scroll_decorator::ptr create(container::ptr parent, 
					const rectangle& rect, widget::ptr component);
			~scroll_decorator();

			/********** SCROLL DECORATOR INTERFACE **********/
			/// Whether or not to draw the verticle scrollbar
			/// \todo: perhaps this should take an enum: show, hide, auto ?
			void show_vscroll(bool val);
			/// Whether or not to draw the horizontal scrollbar
			/// \todo: perhaps this should take an enum: show, hide, auto ?
			void show_hscroll(bool val);
			/// Whether or not to show the arrows on the scrollbars
			/// You want these if you have a mouse, but they are superfluous and maybe 
			/// confusing if you don't.
			void show_arrows(bool val);
			/// Set the verticle scroll position as a percentage [0-1.0]
			void vpos(float percent);
			/// Set the horizontal scroll position as a percentage [0-1.0]
			void hpos(float percent);
			/********** END SCROLL DECORATOR INTERFACE **********/
	};
}

#endif
