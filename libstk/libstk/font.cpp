#include <iostream>
#include <string>
#include "font.h"

using std::wstring;
using std::string;
using std::cout;
using std::endl;

namespace stk
{
	FT_Library font::lib_ = NULL;
	int font::font_count_ = 0;

	font::ptr font::create(const string& fontname, int height, int width)
	{
		font::ptr new_font(new font(fontname, height, width));
		return new_font;
	}
	
	font::font(const string& fontname, int height, int width)
		: height_(height), width_(width)
	{
		int error;
		if (font_count_ == 0)
		{
			//cout << "font::font: initializing FreeType library" << endl;
			error = FT_Init_FreeType( &lib_ );
			if ( error ) throw std::string("font::font: could not initialize Freetype library");
		}
		string filename = string("/usr/share/libstk/fonts/")+fontname;
		//cout << "font::font: opening font file " << filename << endl;
		error = FT_New_Face( lib_, filename.c_str(), 0, &face_);
		if (error == FT_Err_Unknown_File_Format)
		{
			throw std::string("font::font: unknown file format");
		}
		else if (error)
		{
			cout << "stk::font::font: error: " << error << endl;
			throw std::string("font::font: unknown error loading font");
		}

		error = FT_Set_Pixel_Sizes(
				face_,		// handle to face object
				width_,		// char_width in pixels
				height_);	// char_height in pixels
		if (error) throw std::string("font::font: could not set font size");
		
		// increment library usage counter
		font_count_++;
	}

	font::~font()
	{
		// free the freetype font data
		FT_Done_Face(face_);
		
		// need to do reference counting for the lib handle
		if (--font_count_ == 0)
		{
			//cout << "font::~font: Done with FreeType library" << endl;
			FT_Done_FreeType(lib_);
		}

		// free the glyphs
	}

	const glyph::ptr font::glyph(wchar_t c)
	{
		if (glyph_cache_[c])
		{
			return glyph_cache_[c];
		}
		// create a new one - not found
		// retrieve glyph index from character code
		int index = FT_Get_Char_Index(face_, c);

		// load glyph image into the slot (erase previous one)
		int error = FT_Load_Glyph(face_, index, FT_LOAD_DEFAULT);
		if (error) throw std::string("font::glyph: could not load glyph");

		// convert to an anti-aliased bitmap
		error = FT_Render_Glyph(face_->glyph, ft_render_mode_normal);
		if (error) throw std::string("font::glyph: could not render glyph");

		glyph::ptr g = glyph::create(face_->glyph, index);
		glyph_cache_[c] = g;
		return glyph_cache_[c];
	}

	int font::draw_len(const wstring& text, int kerning_mode)
	{
		int len = 0;
		for (int i=0; i<text.length(); i++)
		{
			len += glyph(text[i])->width();
		}
		return len;
	}

	int font::kerning(wchar_t left, wchar_t right, int kerning_mode)
	{
		FT_Vector kerning;
		int error = FT_Get_Kerning(face_,	// handle to face object
								   left,	// left glyph index
								   right,	// right glyph index
								   kerning_mode,	// kerning mode
								   &kerning);	// target vector
		if (error) return 0;
		return kerning.x;
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


