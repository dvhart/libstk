#include <iostream>
#include <string>
#include "libstk/font.h"
#include "libstk/exceptions.h"

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
			if ( error ) throw error_message_exception("font::font: could not initialize Freetype library");
		}
		string filename = string("/usr/share/libstk/fonts/")+fontname;
		//cout << "font::font: opening font file " << filename << endl;
		error = FT_New_Face( lib_, filename.c_str(), 0, &face_);
		if (error == FT_Err_Unknown_File_Format)
		{
			throw error_message_exception("font::font: unknown file format");
		}
		else if (error)
		{
			throw error_message_exception("font::font: unknown error loading font");
		}

		error = FT_Set_Pixel_Sizes(
				face_,		// handle to face object
				width_,		// char_width in pixels
				height_);	// char_height in pixels
		if (error) throw error_message_exception("font::font: could not set font size");
		
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
		if (error) throw error_message_exception("font::glyph: could not load glyph");

		// convert to an anti-aliased bitmap
		error = FT_Render_Glyph(face_->glyph, ft_render_mode_normal);
		if (error) throw error_message_exception("font::glyph: could not render glyph");

		glyph::ptr g = glyph::create(face_->glyph, index);
		glyph_cache_[c] = g;
		return glyph_cache_[c];
	}

	int font::draw_len(const wstring& text, int kerning_mode)
	{
		int len = 0;
		for (int i=0; i<text.length(); i++)
		{
			len += glyph(text[i])->advance_x();
			if (i < text.length()-1)
				len += kerning(text[i], text[i+1], kerning_mode);
		}
		return len >> 6;
	}

	int font::chars_in_rect(const rectangle& rect, const wstring& text, int kerning_mode)
	{
		int maxcount = text.length();
		if (height_ > rect.height()) return 0;
		int len = 0, count = 0;
		while (1)
		{
			len += glyph(text[count])->advance_x();
			if ((len >> 6) > rect.width()) break;
			if (count < maxcount-1)
				len += kerning(text[count], text[count+1], kerning_mode);
			count++;
		}
		if (count > maxcount) count = maxcount;
		return count;
	}

	int font::kerning(wchar_t left, wchar_t right, int kerning_mode)
	{
		FT_Vector kerning;
		int error = FT_Get_Kerning(face_,	// handle to face object
								   glyph(left)->index(),	// left glyph index
								   glyph(right)->index(),	// right glyph index
								   kerning_mode,	// kerning mode
								   &kerning);	// target vector
		if (error) return 0;
		return kerning.x;
	}
	
	
} // namespace stk
