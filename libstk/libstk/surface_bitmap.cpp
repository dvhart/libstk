/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Implementation of a surface class which draws to an in memory bitmap.
 *     AUTHORS: Marc Straemke
 *  START DATE: 2003/Oct/05
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/
#include "libstk/config.h"
#ifdef HAVE_LIBPNG
# include <png.h>
# include <stdio.h>
# include <unistd.h>
#endif

#include "surface_bitmap.h"
#include "logging.h"

namespace stk
{
    surface_bitmap::surface_bitmap(const rectangle& size) : surface_impl<surface_bitmap>(size)
    {
	bitmap.reset(new unsigned char[size.width()*size.height()*3]); // RGB format
	memset(bitmap.get(),0x00,size.width()*size.height()*3);
    }
    surface_bitmap::ptr surface_bitmap::create(const rectangle& size)
    {
	return surface_bitmap::ptr(new surface_bitmap(size));
    }
    /// Factory for offscreen surfaces
    surface::ptr surface_bitmap::create_surface(const rectangle& rect) 
    {
	return surface_bitmap::ptr(new surface_bitmap(rect));
    }
    const unsigned char* surface_bitmap::get_bitmap()
    {
	return bitmap.get();
    }
    
    void surface_bitmap::put_pixel(int x, int y, color clr)
    {
	x+=offset_.x();
	y+=offset_.y();
	if(!clip_rect_.contains(x,y))
	    return;
	if(!rect_.contains(x,y))
            return;
//        INFO("Putting pixel " << x << "," << y << "  With color=" << std::hex << clr);
	unsigned char src_red = (clr >> 24) & 0xff;
	unsigned char src_green = (clr >> 16) & 0xff;
	unsigned char src_blue = (clr >> 8) & 0xff;
        unsigned char src_alpha = (clr) & 0xff;

	if(src_alpha!=0xff)
	{
            return;
	    color dst_color=get_pixel(x,y);
	    unsigned char dst_red = (clr >> 24) & 0xff;
	    unsigned char dst_green = (clr >> 16) & 0xff;
	    unsigned char dst_blue = (clr >> 8) & 0xff;
	    src_red = (src_red*(int)src_alpha + dst_red*((int)0xff-src_alpha)) >> 8;
	    src_green = (src_green*(int)src_alpha + dst_green*((int)0xff-src_alpha)) >> 8;
	    src_blue = (src_blue*(int)src_alpha + dst_blue*((int)0xff-src_alpha)) >> 8;
	}
//	std::cout << (unsigned)src_red << " " << (unsigned)src_green << " " << (unsigned)src_blue << "  " ;
	
	bitmap[y*rect_.width()*3+x*3] = src_red;
	bitmap[y*rect_.width()*3+x*3+1] = src_green;
	bitmap[y*rect_.width()*3+x*3+2] = src_blue;
    }
    void surface_bitmap::put_pixel_aa(int x, int y, double distance, color clr)
    {
        put_pixel(x,y,clr);
    }
    void surface_bitmap::put_pixel_aa(int x, int y, unsigned char alpha_a, color clr)
    {
	put_pixel(x,y,(clr&(~0xff))|alpha_a);
    }
    
    color surface_bitmap::get_pixel(int x, int y) const
    {
	x+=offset_.x();
	y+=offset_.y();

        if(!clip_rect_.contains(x,y))
	    return 0;
	if(!rect_.contains(x,y))
            return 0;
        
	int color=0;
	color|=bitmap.get()[y*rect_.width()*3+x*3]<<24;
	color|=bitmap.get()[y*rect_.width()*3+x*3+1]<<16;
	color|=bitmap.get()[y*rect_.width()*3+x*3+2]<<8;

	return color;
	ERROR("surface_bitmap::get_pixel UNIMPLEMENTED");
    }
    
    color surface_bitmap::gen_color(byte r, byte g, byte b, byte a) const
    {
        unsigned int color = (int)a;
        color |= (int)b << 8;
        color |= (int)g << 16;
        color |= (int)r << 24;
        return color;
    }

    color surface_bitmap::rgba_color(color clr) const
    {
        return clr;
    }
    
    void surface_bitmap::lock()
    {
	INFO("surface_bitmap::lock called (Do nothing)");
    }
    
    void surface_bitmap::lock(rectangle &rect, int flags, color** buf, int &stride)
    {
	buf=NULL;
	stride=0;
	INFO("surface_bitmap::lock(rect,int,color**,int&) called (Do nothing)");
    }
    
    void surface_bitmap::unlock()
    {
	INFO("surface_bitmap::unlock called (Do nothing)");
    }
    
    void surface_bitmap::update(const rectangle& u_rect)
    {
	INFO("surface_bitmap::update Called (Do nothing)");
    }
    
    overlay::ptr surface_bitmap::create_overlay(int width, int height, int format)
    {
	INFO("surface_bitmap::create_overlay called(Do nothing)");
	return overlay::ptr();
    }
    
}

