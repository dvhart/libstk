#ifndef STK_BUTTON_H
#define STK_BUTTON_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/signal.hpp>

#include "widget.h"
#include "container.h"

namespace stk
{
	class button : public widget
	{
		public:
			typedef boost::shared_ptr<button> ptr;
			typedef boost::weak_ptr<button> weak_ptr;

		private:
			rectangle rect_;
			
		protected:
			button(container::ptr parent, std::string label, const rectangle& rect);
			
		public:
			static button::ptr create(container::ptr parent, std::string label, 
					const rectangle& _rect);
			virtual ~button();

			virtual void draw(boost::shared_ptr<stk::surface> surface);

			// button signals
			boost::signal<bool ()> on_click;

			// event_handler interface
			virtual void handle_event(event::ptr e);
	};
	
} // namespace stk

#endif
