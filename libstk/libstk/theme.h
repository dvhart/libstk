#ifndef THEME_H
#define THEME_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "surface.h"
#include "rectangle.h"

namespace stk
{
	class theme
	{
		public:
			typedef boost::shared_ptr<theme> ptr; 
			typedef boost::weak_ptr<theme> weak_ptr;
			
		private:
			stk::surface::ptr surface_;
			static stk::theme::ptr instance_;
			
		protected:
			theme(stk::surface::ptr surface);
			
		public:
			static theme::ptr create(stk::surface::ptr surface);
			virtual ~theme();
			static stk::theme::ptr instance();
			virtual void draw_state(const rectangle& rect);
			virtual void draw_button(rectangle& rect, bool active, bool focused, bool hover);
			virtual void draw_label(rectangle& rect, std::wstring text);
	};
}

#endif
