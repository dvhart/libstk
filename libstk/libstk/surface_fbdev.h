/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: The surface backend for Framebuffer device.
 *     AUTHORS: Vernon Mauery, Darren Hart
 *  START DATE: 2003/May/13
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_SURFACE_FBDEV_H
#define STK_SURFACE_FBDEV_H

#include <vector>
#include <libstk/surface.h>
#include <libstk/surface_impl.h>
#include <libstk/point.h>
#include <libstk/stk_types.h>
//#include <libstk/backend_fbdev.h>

#include <boost/optional.hpp>

#define MIN(A,B) (((A)<(B))?(A):(B))

namespace stk
{
    class surface_fbdev : public surface_impl<surface_fbdev>
    {
    public:
        typedef boost::shared_ptr<surface_fbdev> ptr;
        typedef boost::weak_ptr<surface_fbdev> weak_ptr;
    protected:
        surface_fbdev(boost::optional<rectangle> rect,bool primary);

        int m_fbdev;
        int m_width;
        int m_height;
        int m_linelength;
        int m_bytesperpixel;
        unsigned char* m_screen;
        int m_screensize;	

    public:
        static surface_fbdev::ptr create(const rectangle& rect, bool primary=true); // Create with given screen size
        static surface_fbdev::ptr create(); // Create with screen size without switching resolutions, has to be used ONLY for creating the primary surface

        ~surface_fbdev();

        // methods which MUST be implemented in derived classes
        inline void put_pixel(int x, int y, color clr);
        inline void put_pixel_aa(int x, int y, double distance, color clr);
        inline void put_pixel_aa(int x, int y, unsigned char alpha_a, color clr);
        inline color get_pixel(int x, int y) const;
        virtual color gen_color(const std::string &str_color) const;
        virtual color gen_color(byte r, byte g, byte b, byte a) const;
        virtual void lock();
        virtual void lock(rectangle &rect, int flags, color** buf, int &stride);
        virtual void unlock();
        virtual void update(const rectangle& u_rect=rectangle());
        virtual boost::shared_ptr<overlay> create_overlay(int width, int height, int format);
        virtual surface::ptr create_surface(const rectangle& rect);

        // overridden drawing routines
        virtual void blit(surface &dst_surface);
        virtual void fill_rect(int x1, int y1, int x2, int y2);
        virtual void fill_rect(const rectangle& rect);
    private:
        bool init();
    };
} //end namespace stk

#endif
