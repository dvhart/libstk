#ifndef STK_GRAPHICS_CONTEXT_H
#define STK_GRAPHICS_CONTEXT_H

#include "stk.h"
#include "font.h"

namespace stk
{
	class graphics_context
	{
		private:
			int line_width_;
			int line_style_;
			color line_color_;
			color fill_color_;
			// font stuff
			stk::font::ptr font_;
			color font_fill_color_;
			color font_outline_color_;
			// we may need to subclass the gc (gc_sdl, gc_dfb, etc)
		public:
			int line_width() const { return line_width_; }
			void line_width(int val) { line_width_ = val; }
			int line_style() const { return line_style_; }
			void line_style(int val) { line_style_ = val; }
			color line_color() const { return line_color_; }
			void line_color(color clr) { line_color_ = clr; }
			color fill_color() const { return fill_color_; }
			void fill_color(color clr) { fill_color_ = clr; }
			// font stuff
			stk::font::ptr font() const { return font_; }
			void font(stk::font::ptr f) { font_ = f; }
			color font_fill_color() const { return font_fill_color_; }
			void font_fill_color(color clr) { font_fill_color_ = clr; }
			color font_outline_color() const { return font_outline_color_; }
			void font_outline_color(color clr) { font_outline_color_ = clr; }
	};
}

#endif
