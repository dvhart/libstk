#ifndef PROGRESS_H
#define PROGRESS_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/signal.hpp>

#include "rectangle.h"
#include "widget.h"
#include "container.h"

namespace stk
{
	class progress : public widget
	{
		public:
			typedef boost::shared_ptr<progress> ptr;
			typedef boost::weak_ptr<progress> weak_ptr;
		
		private:
			
		protected:
			progress(container::ptr parent, std::wstring label, const rectangle& rect, 
					int range);
			std::wstring label_;
			int range_;
			int position_;
			
		public:
			static progress::ptr create(container::ptr parent, const std::wstring label, 
				const rectangle& _rect, int range);
			virtual ~progress();

			// accessor methods
			int range() { return range_; }
			void range(int val) { range_ = val; redraw(rect_); } // FIXME: update position too
			int position() { return position_; }
			void position(int val) { position_ = MIN(range_, val); redraw(rect_); } // FIXME: make more robust, neg, exceptions, etc
			float percent() { return (float)position_/(float)range_; }
			void percent(float val) { position_ = (int)(range_*val); redraw(rect_); }

			// event_handler interface
			// using defaults (widget.h)

			// drawable interface
			virtual void draw(surface::ptr surface);
			// using defaults (widget.h)

			// parent interface
			// using defaults (widget.h)


	};
}

#endif
