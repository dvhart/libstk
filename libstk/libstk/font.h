/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Font class for caching glyphs.
 *     AUTHORS: Vernon Mauery, Marc Strämke, Darren Hart
 *  START DATE: 2003/Mar/15
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_FONT_H
#define STK_FONT_H

#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>
#include <string>
#include <boost/shared_ptr.hpp>
#include <libstk/rectangle.h>
#include <libstk/glyph.h>
#include <libstk/font_properties.h>

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

    protected:
        static FT_Library lib_;
        static int font_count_;
        FT_Face face_;
        int height_;
        float rotation_;
        FT_Matrix transformation_matrix;
        std::map<wchar_t, stk::glyph::ptr> glyph_cache_;

        font();
        font(const font_properties& prop);
        // finds a face from a list of fonts and properties
        FT_Face find_font(const font_properties& prop);

    public:
        virtual ~font();
        static font::ptr create(const font_properties& prop);

        const stk::glyph::ptr glyph(wchar_t c);
        /// Get the X size of the to be drawn text (depreciated?)
        int draw_len(const std::wstring& text, int kerning_mode = 0);
        /// Get the extends of the given text if it was to be drawn.
        /// Origin of the returned coordinates is the font baseline!
        rectangle draw_extends(const std::wstring& text, int kerning_mode = 0);
        
        int chars_in_rect(const rectangle& rect, const std::wstring& text, int kerning_mode = 0);
        point kerning(wchar_t left, wchar_t right, int kerning_mode = 0);
        int height() const { return height_; }
        float rotation() const { return rotation_; }
    }
    ; //class font


} // namespace stk

#endif
