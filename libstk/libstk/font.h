#ifndef STK_FONT_H
#define STK_FONT_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>
#include <string>
#include <boost/shared_ptr.hpp>
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

		private:

		protected:
			font(const std::string& fontname, int height, int width);

			static FT_Library lib_;
			FT_Face face_;
			int height_, width_;
			std::map<unsigned int, stk::glyph::ptr> glyph_cache_;

		public:
			static font::ptr create(const std::string& fontname, int height, int width=0);
			virtual ~font();
			const stk::glyph::ptr glyph(char c); // FIXME:: UCS4 not char
			int draw_len(std::string text);
	}; //class font


} // namespace stk

#endif
