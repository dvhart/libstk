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
			button(boost::shared_ptr<container> parent, std::string label, 
					int x, int y, int width, int height);
			~button();

			// button signals
			boost::signal<bool ()> on_click;

			// event_handler interface
			virtual void handle_event(boost::shared_ptr<stk::event> e);
	};

	typedef boost::shared_ptr<stk::button> Button;
	
} // namespace stk

#endif
