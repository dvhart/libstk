#ifndef STATE_H
#define STATE_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "widget.h"
#include "container.h"
#include "application.h"

namespace stk
{
	class state : public stk::container
	{
		public:
			typedef boost::shared_ptr<state> ptr;
			typedef boost::weak_ptr<state> weak_ptr;
			
		private:

		protected:
			state(application::ptr parent);
			widget::weak_ptr focused_widget_; // FIXME: are we using this ?
			
		public:
			virtual ~state();
			static state::ptr create(application::ptr parent);
			widget::weak_ptr focused_widget();
			void focused_widget(widget::weak_ptr value);

			// event_handler interface
			virtual void handle_event(event::ptr e);

			// drawable interface
			//virtual surface::ptr surface(); 
			virtual void draw(boost::shared_ptr<stk::surface> surface);
			//virtual void redraw(bool val, const rectangle& rect=rectangle(0,0,0,0));
	};
}

#endif
