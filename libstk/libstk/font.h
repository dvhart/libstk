#ifndef STK_FONT_H
#define STK_FONT_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include <string>
#include <boost/shared_ptr.hpp>
#include "fbitmap.h"

/*
  // we will cache these
   
  typedef struct  FT_Bitmap_
  {
    int             rows;
    int             width;
    int             pitch;
    unsigned char*  buffer;
    short           num_grays;
    char            pixel_mode;
    char            palette_mode;
    void*           palette;

  } FT_Bitmap;
*/

namespace stk
{
	class font
	{
		public:
			const static int NUM_GLYPHS=256;
		private:

		protected:
			static FT_Library lib_;
			FT_Face face_;
			int height_, width_;
			std::vector<FT_Bitmap *> bmaps_;

		public:
			font(std::string fontname, int height, int width=0);
			virtual ~font();
			const stk::bitmap &char_bitmap(char c); // FIXME:: UCS4 not char
			int draw_len(std::string text) const;
	}; //class font

	typedef boost::shared_ptr<stk::font> Font;

} // namespace stk

#endif
