#include "font.h"

using std::string;

namespace stk
{
	font::font(string fontname, int height, int width)
		: height_(height), width_(width)
	{
		int error;
		if (!lib_)
		{
			error = FT_Init_FreeType( &library );
			if ( error ) throw string("font::font: could not initialize Freetype library");
		}
		error = FT_New_Face( lib_, (string("/usr/share/libstk/fonts/")+
									fontname).c_str(), 0, &face_);
		if (error == FT_Err_Unknown_File_Format)
		{
			throw string("font::font: unknown file format");
		}
		else if (error)
		{
			throw string("font::font: unknown error loading font");
		}

		error = FT_Set_Pixel_Sizes(
				face_,		// handle to face object
				width_,		// char_width in pixels
				height_);	// char_height in pixels
		if (error) throw string("font::font: could not set font size");

		bmaps_.resize(NUM_GLYPHS);
		for (int i=0; i<NUM_GLYPHS; i++)
			bmaps_[i] = NULL;
	}

	font::~font()
	{
		// free the freetype font data
		FT_Done_Face(face_);
		// need to do reference counting for the lib handle
		//FT_Done_FreeType();

		// free the glyphs
		for (int i=0; i<NUM_GLYPHS; i++)
			if (bmaps_[i]) 
			{
				delete bmaps_[i]->buffer;
				delete bmaps_[i];
			}
		bmaps_.clear();
	}

	const FT_bitmap &char_bitmap(char c)
	{
		if (bmaps_[c]) return bmaps_[c];
		// create a new one - not found
		error = FT_Load_Char( face, text[n], FT_LOAD_RENDER );
		FT_Bitmap *tb = new FT_Bitmap;
		memcpy(tb, face->glyph->bitmap, sizeof(FT_Bitmap));
		tb->buffer = new unsigned char[tb->rows*tb->width];
		memcpy(tb->buffer, face->glyph->bitmap->buffer, 
			   tb->rows*tb->width);
		bmaps_[c] = tb;
		return bmaps_[c];
	}

	int draw_len(string text)
	{
		int len = 0;
		for (int i=0; i<text.length(); i++)
		{
			len += char_bitmap[text[i]].x();
		}
		return len;
	}
	
	
} // namespace stk


/*
 * demo code from freetype tutorial
 *
 *
 *
*/
/*
int main(void)
{
	FT_Library   library;   // handle to library
	FT_Face      face;      // handle to face object
	FT_GlyphSlot  slot = face->glyph;  // a small shortcut
	FT_UInt       glyph_index;
	int           pen_x, pen_y, n;

	error = FT_Init_FreeType( &library );
	if ( error ) { ... }

	error = FT_New_Face( library,
			"/usr/share/fonts/truetype/arial.ttf",
			0,
			&face );
	if ( error == FT_Err_Unknown_File_Format )
	{
		//... the font file could be opened and read, but it appears
		//... that its font format is unsupported
	}
	else if ( error )
	{
		//... another error code means that the font file could not
		//... be opened or read, or simply that it is broken...
	}



	error = FT_Set_Char_Size(
			face,    // handle to face object           
			0,       // char_width in 1/64th of points  
			16*64,   // char_height in 1/64th of points 
			0,     // 72 dpi horizontal device resolution    
			0 );   // 72 dpi vertical device resolution      


	pen_x = 300;
	pen_y = 200;

	for ( n = 0; n < num_chars; n++ )
	{
		// load glyph image into the slot (erase previous one)
		error = FT_Load_Char( face, text[n], FT_LOAD_RENDER );
		if (error) continue;  // ignore errors

		// now, draw to our target surface
		my_draw_bitmap( &slot->bitmap,
				pen_x + slot->bitmap_left,
				pen_y - slot->bitmap_top );

		// increment pen position 
		pen_x += slot->advance.x >> 6;
	}

	return 0;
}

*/


