/******************************************************************************
 *    FILENAME: glyph.h
 * DESCRIPTION: A single character of a font.
 *     AUTHORS: Vernon Mauery
 *  START DATE: 17/Mar/2003  LAST UPDATE: 13/May/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available
 *              with the source as license.txt or at 
 *              http://www.libstk.org/index.php?page=docs/license
 *****************************************************************************/

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
			glyph(const FT_GlyphSlot g, unsigned int indx);

			int width_, height_;
			int advance_x_, advance_y_;
			int bearing_x_, bearing_y_;
			unsigned int index_;

			boost::shared_array<unsigned char> rdata_;
			
		public:
			static glyph::ptr create(const FT_GlyphSlot g, unsigned int indx);
			virtual ~glyph();
			const boost::shared_array<unsigned char> bitmap() const { return rdata_; } 

			int width() const { return width_; }
			int height() const { return height_; }
			
			int advance_x() const { return advance_x_; }
			int advance_y() const { return advance_y_; }
			
			int bearing_y() const { return bearing_y_; }
			int bearing_x() const { return bearing_x_; }

			unsigned int index() const { return index_; }
	}; //class glyph

} // namespace stk

#endif

