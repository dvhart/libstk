/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Declaration of a surface class which draws to an in memory bitmap.
 *     AUTHORS: Marc Straemke
 *  START DATE: 2003/Oct/05
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include <boost/weak_ptr.hpp>

#include <libstk/config.h>
#include <libstk/surface.h>
#include <libstk/surface_impl.h>
#include <libstk/point.h>
#include <libstk/image.h>

namespace stk
{
    /// \brief Surface-Class for drawing to an in-memory Bitmap.
    /// This class allows the user to draw to an in memory bitmap
    /// which is usefull for drawing to inkjet printers etc
    class surface_bitmap  : public surface_impl<surface_bitmap>
    {
    public:
	typedef boost::shared_ptr<surface_bitmap> ptr;
	typedef boost::weak_ptr<surface_bitmap> weak_ptr;
    private:
	boost::shared_array<unsigned char> bitmap; // height*width*3 RGB
	surface_bitmap(const rectangle& size);
    public:
	static surface_bitmap::ptr create(const rectangle& size);
	const unsigned char* get_bitmap();

	inline void put_pixel(int x, int y, color clr);
        inline void put_pixel_aa(int x, int y, double distance, color clr);
        inline void put_pixel_aa(int x, int y, unsigned char alpha_a, color clr);
        color get_pixel(int x, int y) const;
	
        virtual color gen_color(byte r, byte g, byte b, byte a) const;
	virtual void lock();
        virtual void lock(rectangle &rect, int flags, color** buf, int &stride);
        virtual void unlock();
	virtual void update(const rectangle& u_rect=rectangle());
        virtual overlay::ptr create_overlay(int width, int height, int format);
	/// Factory for offscreen surfaces
        virtual surface::ptr create_surface(const rectangle& rect);
    };
}

    
