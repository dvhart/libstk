#ifndef THEME_H
#define THEME_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "libstk/surface.h"
#include "libstk/rectangle.h"
#include "libstk/image_panel.h"
#include "libstk/image.h"
#include "libstk/list.h"
#include "libstk/list_item.h"

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
			
			color outline_color_normal_;
			color outline_color_focused_;
			color outline_color_hover_;
			color outline_color_pressed_;
			
			color fill_state_color_;
			color fill_color_normal_;
			color fill_color_focused_;
			color fill_color_hover_;
			color fill_color_pressed_;

			color font_color_normal_;
			color font_color_focused_;
			color font_color_hover_;
			color font_color_pressed_;
			
		public:
			static theme::ptr create(stk::surface::ptr surface);
			virtual ~theme();
			static stk::theme::ptr instance(); // FIXME: use get instead ?
			
			// FIXME: the draw routines need some kind of property map passed in
			virtual void draw_state(const rectangle& rect, bool focused);
			virtual void draw_button(const rectangle& rect, const std::wstring& label, 
				bool pressed, bool focused, bool hover);
			virtual void draw_progress(const rectangle& rect, const std::wstring& label, float range, bool focused); 
			virtual void draw_label(const rectangle& rect, const std::wstring& text, bool focused);
			virtual void draw_image_panel(const rectangle& rect, image::ptr img);
			virtual void draw_list(const rectangle& rect, std::vector<list_item::ptr> items, float selected);
			virtual void draw_list_item(const rectangle& rect, std::wstring label);
	};
}

#endif
