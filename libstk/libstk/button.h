#ifndef STK_BUTTON_H
#define STK_BUTTON_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/signal.hpp>

#include "widget.h"

namespace stk
{
	class button : public widget
	{
		private:
		protected:
			button() { }; // empty constructor, needed for creating derived classes ?
		public:
			button(boost::weak_ptr<container> parent);
			~button();

			// button signals
			boost::signal<bool ()> on_click;
	};

	typedef boost::shared_ptr<stk::button> Button;
	
} // namespace stk

#endif
