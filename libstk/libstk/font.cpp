/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Font class implementation.
 *     AUTHORS: Vernon Mauery, Darren Hart, Marc Strämke
 *  START DATE: 2003/Mar/15
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include <iostream>
#include <string>
#include <math>
#include <libstk/font.h>
#include <libstk/read_dir.h>
#include <libstk/exceptions.h>
#include <libstk/logging.h>
#include <libstk/glyph.h>

using std::wstring;
using std::string;
using std::vector;

namespace stk
{
    FT_Library font::lib_ = NULL;
    int font::font_count_ = 0;

    font::ptr font::create(const font_properties& prop)
    {
        font::ptr new_font(new font(prop));
        return new_font;
    }

    font::font()
    {
        throw error_message_exception("font::font: bad call to empty constructor");
    }

    font::font(const font_properties& prop)
        : height_(*prop.height), rotation_(*prop.rotation) 
    {
        INFO("constructor: " << *prop.fontname << ": " << *prop.height << " pt, style " << (int)*prop.style);
        int error;
        // FIXME: consider using a freetype_backend (like dfb_backend and sdl_data)
        if (font_count_ == 0)
        {
            INFO("initializing FreeType library");
            error = FT_Init_FreeType( &lib_ );
            if ( error )
            {
                ERROR("could not initialized freetype library");
                throw error_message_exception("font::font: could not initialize Freetype library");
            }
        }

        // look for the right family, right style
        face_ = find_font(prop);
        if (face_ == NULL)
        {
            // look for the right family, no style
            font_properties replacement_font = prop;
            if (*prop.style != font_properties::plain)
            {
                INFO("falling back to " << prop.fontname << " plain");
                *replacement_font.style = font_properties::plain;
                face_ = find_font(replacement_font);
                if (face_ == NULL)
                {
                    // or look for vera, right style
                    INFO("falling back to vera style " << prop.style);
                    *replacement_font.fontname = "vera";
                    replacement_font.style = prop.style;
                    face_ = find_font(replacement_font);
                }
            }
            else
            {
                // or look for vera plain
                INFO("falling back to vera plain");
                *replacement_font.fontname = "vera";
                face_ = find_font(replacement_font);
            }
        }
        
        // could not find after second try, throw something
        if (face_ == NULL)
        {
            ERROR("could not find appropriate face/style");
            throw error_message_exception("font::font: could not find appropriate face");
        }

        error = FT_Set_Pixel_Sizes(
                    face_,    // handle to face object
                    0,        // char_width in pixels (height if 0)
                    height_); // char_height in pixels
        if (error)
            throw error_message_exception("font::font: could not set font size");

        transformation_matrix.xx = cos(rotation_)*0x10000;
        transformation_matrix.xy = -sin(rotation_)*0x10000;
        transformation_matrix.yx = sin(rotation_)*0x10000;
        transformation_matrix.yy = cos(rotation_)*0x10000;

        // increment library usage counter
        font_count_++;
    }

    font::~font()
    {
        INFO("destructor");
        // free the freetype font data
        FT_Done_Face(face_);

        // need to do reference counting for the lib handle
        if (--font_count_ == 0)
        {
            INFO("last font destroyed: freeing FreeType library");
            FT_Done_FreeType(lib_);
        }

        // free the glyphs
        // WRITEME... (when we actually cache them of course :-) )
        INFO("destructor complete");
    }

    FT_Face font::find_font(const font_properties& prop)
    {
        string fontdir = PACKAGE_FONTS_DIR"/";
        vector<dir_entry::ptr> selected_files = read_dir(fontdir, *prop.fontname);

        int error;
        FT_Face face = NULL;
        bool found_font = false;
        vector<dir_entry::ptr>::iterator dir_iter = selected_files.begin();
        for (; dir_iter != selected_files.end(); dir_iter++)
        {
            string filename = fontdir + (*dir_iter)->filename();
            int face_style;

            error = FT_New_Face( lib_, filename.c_str(), 0, &face);
            if (error == FT_Err_Unknown_File_Format)
            {
                ERROR("unknown file format: " << filename); 
                throw error_message_exception("font::font: unknown file format: " + filename);
            }
            else if (error)
            {
                ERROR("Unable to load font, does " << filename << " exist?");
                throw error_message_exception("font::font: unknown error loading font: " + filename);
            }

            // from font_properties, 
            // style = ((bold)?(1<<1):0) | ((italic)?(1<<0):0);
            face_style = 
                ((face->style_flags & FT_STYLE_FLAG_BOLD) ? 
                 font_properties::bold : font_properties::plain) |
                ((face->style_flags & FT_STYLE_FLAG_ITALIC) ?
                 font_properties::italic : font_properties::plain);

            INFO(filename);
            INFO(face->family_name << ": " << prop.height << " pt, style " << face_style);

            if (face_style == *prop.style)
            {
                found_font = true;
                break;
            }
            else
            {
                FT_Done_Face(face);
                face = NULL;
            }

            if (found_font) break;
        }
        return face;
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

        // Set transformation matrix (Only rotational atm)
/*        FT_Vector     pen;                 // untransformed origin
        pen.x=0;
        pen.y=0;*/
        FT_Set_Transform(face_, &transformation_matrix, NULL);

        // load glyph image into the slot (erase previous one)
        int error = FT_Load_Glyph(face_, index, FT_LOAD_DEFAULT);
        if (error)
            throw error_message_exception("font::glyph: could not load glyph");

        // convert to an anti-aliased bitmap
        error = FT_Render_Glyph(face_->glyph, ft_render_mode_normal);
        if (error)
            throw error_message_exception("font::glyph: could not render glyph");

        // Have to fix up Metrics, they dont get transformed!
        face_->glyph->metrics.horiBearingX=face_->glyph->bitmap_left << 6;
        face_->glyph->metrics.horiBearingY=face_->glyph->bitmap_top << 6;
        
        glyph::ptr g = glyph::create(face_->glyph, index);
        glyph_cache_[c] = g;
        return glyph_cache_[c];
    }
    //  Should return a vector/point
    int font::draw_len(const wstring& text, int kerning_mode)
    {
        unsigned int len = 0;
        for (unsigned int i=0; i<text.length(); i++)
        {
            len += glyph(text[i])->advance_x();
            if (i < text.length()-1)
                len += kerning(text[i], text[i+1], kerning_mode).x();
        }
        return len >> 6;
    }

    /// Returns
    rectangle font::draw_extends(const wstring& text, int kerning_mode)
    {
        rectangle extends(0,0,0,0);

        int len=0;
        for (unsigned int i=0; i<text.length(); i++)
        {
            const glyph::ptr current_glyph=glyph(text[i]);

            if(-current_glyph->bearing_y() < extends.y1())
                extends.y1(-current_glyph->bearing_y());
            if((-current_glyph->bearing_y() + current_glyph->height()) > extends.y2())
                extends.y2(-current_glyph->bearing_y() + current_glyph->height());
            
            len += current_glyph->advance_x();
            if (i < text.length()-1)
                len += kerning(text[i], text[i+1], kerning_mode).x();
            
            if(len > extends.x2())
                extends.x2(len);
        }
        extends.x1(extends.x1()>>6);
        extends.x2(extends.x2()>>6);
        extends.y1(extends.y1()>>6);
        extends.y2(extends.y2()>>6);
        return extends;
    }

    int font::chars_in_rect(const rectangle& rect, const wstring& text, int kerning_mode)
    {
        int maxcount = text.length();
        if (height_ > rect.height())
            return 0;
        int len = 0, count = 0;
        while (count < maxcount)
        {
            len += glyph(text[count])->advance_x();
            if ((len >> 6) > rect.width())
                break;
            if (count < maxcount-1) // FIXME, Kerning is 2d!!!
                len += kerning(text[count], text[count+1], kerning_mode).x();
            count++;
        }
        if (count > maxcount)
            count = maxcount;
        return count;
    }

    point font::kerning(wchar_t left, wchar_t right, int kerning_mode)
    {
        FT_Vector kerning;
        int error = FT_Get_Kerning(face_, // handle to face object
                                   glyph(left)->index(), // left glyph index
                                   glyph(right)->index(), // right glyph index
                                   kerning_mode, // kerning mode
                                   &kerning); // target vector
        if (error)
            return 0;
        return point(kerning.x,kerning.y);
    }


} // namespace stk
