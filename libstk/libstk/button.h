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
			
		protected:
			button(container::ptr parent, std::wstring label, const rectangle& rect);
			std::wstring label_;
			
		public:
			static button::ptr create(container::ptr parent, const std::wstring label, 
					const rectangle& _rect);
			virtual ~button();

			// button signals
			boost::signal<bool ()> on_click; // FIXME: on_activate in widget maybe ?

			// event_handler interface
			virtual void handle_event(event::ptr e);

			// drawable interface
			virtual void draw(surface::ptr surface);
			// using defaults (widget.h)

			// parent interface
			// using defaults (widget.h)
	};
	
} // namespace stk

#endif
