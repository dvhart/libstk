#include <iostream>
#include "glyph.h"

namespace stk
{
	glyph::ptr glyph::create(const FT_GlyphSlot g)
	{
		glyph::ptr new_glyph(new glyph(g));
		return new_glyph;
	}

	glyph::glyph(const FT_GlyphSlot g)
	{
		advance_x_ = g->advance.x;
		advance_y_ = g->advance.y;
		FT_Bitmap b = g->bitmap;
		height_ = b.rows;
		width_ = b.width;
		// copy the FT_Bitmap data
		rdata_ = boost::shared_array<unsigned char>(new unsigned char[width_*height_]);
		memcpy(rdata_.get(), b.buffer, width_*height_);
	}
	
	glyph::~glyph()
	{
		// free the glyph data -- done automatically
		std::cout << "glyph::~glyph: rdata_.use_count = " << rdata_.use_count() << std::endl;	
	}
	
} // namespace stk

