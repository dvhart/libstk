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
			virtual void draw_button(const rectangle& rect, const std::wstring& label, 
				bool active, bool focused, bool hover);
			virtual void draw_progress(const rectangle& rect, const std::wstring& label, float range); 
			virtual void draw_label(const rectangle& rect, const std::wstring& text);
	};
}

#endif
