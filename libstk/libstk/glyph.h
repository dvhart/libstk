#ifndef STK_GLYPH_H
#define STK_GLYPH_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>

namespace stk
{
	class glyph
	{
		public:
			typedef boost::shared_ptr<stk::glyph> ptr;

		private:

		protected:
			glyph(const FT_GlyphSlot g);

			int width_, height_;
			int advance_x_, advance_y_;

			boost::shared_array<unsigned char> rdata_;
			
		public:
			static glyph::ptr create(const FT_GlyphSlot g);
			virtual ~glyph();
			const boost::shared_array<unsigned char> bitmap() const { return rdata_; } 
			int width() const { return width_; }
			int height() const { return height_; }
			int advance_x() const { return advance_x_; }
			int advance_y() const { return advance_y_; }
	}; //class glyph

} // namespace stk

#endif

