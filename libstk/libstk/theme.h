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
			//typedef boost::shared_ptr<theme> ptr; 
			//typedef boost::weak_ptr<theme> weak_ptr;
			
		private:
			stk::surface::ptr surface_;
			static stk::theme* instance_;
			
		protected:
			theme(stk::surface::ptr surface);
			
		public:
			static void create(stk::surface::ptr surface);
			static stk::theme* instance();
			virtual ~theme();
			virtual void draw_state();
			virtual void draw_button(rectangle& rect);
			virtual void draw_label();
	};
}

#endif
