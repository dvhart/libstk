#include <iostream>
#include "glyph.h"

namespace stk
{
	glyph::ptr glyph::create(const FT_GlyphSlot g, unsigned int indx)
	{
		glyph::ptr new_glyph(new glyph(g, indx));
		return new_glyph;
	}

	glyph::glyph(const FT_GlyphSlot g, unsigned int indx)
	{
		advance_x_ = g->advance.x;
		advance_y_ = g->advance.y;
		bearing_x_ = g->metrics.horiBearingX;
		bearing_y_ = g->metrics.horiBearingY;
		FT_Bitmap b = g->bitmap;
		height_ = b.rows;
		width_ = b.width;
		index_ = indx;
		// copy the FT_Bitmap data
		rdata_ = boost::shared_array<unsigned char>(new unsigned char[width_*height_]);
		memcpy(rdata_.get(), b.buffer, width_*height_);
	}
	
	glyph::~glyph()
	{
		// free the glyph data -- done automatically
	}
	
} // namespace stk

