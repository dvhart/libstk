#ifndef STK_FONT_H
#define STK_FONT_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>
#include <string>
#include <boost/shared_ptr.hpp>
#include "rectangle.h"
#include "glyph.h"

/*
   we will be caching stk::glyph's 
*/

namespace stk
{	
	class font
	{
		public:
			typedef boost::shared_ptr<stk::font> ptr;
			typedef boost::weak_ptr<stk::font> weak_ptr;
		private:

		protected:
			

			static FT_Library lib_;
			static int font_count_;
			FT_Face face_;
			int height_, width_;
			std::map<wchar_t, stk::glyph::ptr> glyph_cache_;

		public:
			static font::ptr create(const std::string& fontname, int height, int width=0);
			font(const std::string& fontname, int height, int width);
			virtual ~font();
			const stk::glyph::ptr glyph(wchar_t c); 
			int draw_len(const std::wstring& text, int kerning_mode=0);
			int chars_in_rect(const rectangle& rect, const std::wstring& text, int kerning_mode=0);
			int kerning(wchar_t left, wchar_t right, int kerning_mode=0);
			int height() const { return height_; }
			int width() const { return width_; }
	}; //class font


} // namespace stk

#endif
