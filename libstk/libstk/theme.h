#ifndef THEME_H
#define THEME_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "surface.h"
#include "rectangle.h"

namespace stk
{
	// some tribal colors for this theme
	const std::string outline_color_normal_str_  = "0xFFFFFFFF";
	const std::string outline_color_focused_str_ = "0xFFFF00FF";
	const std::string outline_color_hover_str_   = "0xFFFFFFFF";
	const std::string outline_color_active_str_  = "0xFFFF00FF";

	const std::string fill_color_normal_str_     = "0x0088B2FF";
	const std::string fill_color_focused_str_    = "0x0088B2FF";
	const std::string fill_color_hover_str_      = "0x00C3FFFF";
	const std::string fill_color_active_str_     = "0x00C3FFFF";
	
	const std::string font_color_normal_str_     = "0xFFFFFFFF";
	const std::string font_color_focused_str_    = "0xFFFFFFFF";
	const std::string font_color_hover_str_      = "0xFFFFFFFF";
	const std::string font_color_active_str_     = "0xFFFFFFFF";
	
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
			
			color outline_color_normal_;
			color outline_color_focused_;
			color outline_color_hover_;
			color outline_color_active_;
			
			color fill_color_normal_;
			color fill_color_focused_;
			color fill_color_hover_;
			color fill_color_active_;

			color font_color_normal_;
			color font_color_focused_;
			color font_color_hover_;
			color font_color_active_;
			
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
