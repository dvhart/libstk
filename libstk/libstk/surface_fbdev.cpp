/**************************************************************************************************
 *    FILENAME: surface_fbdev.cpp
 * DESCRIPTION: Direct FB surface implementation.
 *     AUTHORS: Vernon Mauery, Darren Hart 
 *  START DATE: 03/Mar/2003  LAST UPDATE: 20/Jul/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include "libstk/surface_fbdev.h"
#include "libstk/overlay.h"
//#include "libstk/backend_fbdev.h"
#include "logging.h"
#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <sys/mman.h>

#include <sys/ioctl.h>
#include <linux/fb.h>

#include <memory.h>
#include <assert.h>

#define BRUN(A,B) ((0xff >> (7-A)) & (0xff << B))

namespace stk
{
    surface_fbdev::surface_fbdev(boost::optional<rectangle> rect,bool primary)
        : m_fbdev(0), m_width(0), m_height(0), m_linelength(0),
    m_bytesperpixel(0), m_screen(NULL), m_screensize(0)
    {
        INFO("surface create");
        m_fbdev = open("/dev/fb0", O_RDWR);
        if( m_fbdev == -1 )
        {
            throw error_message_exception("could not open fbdev");
        }

        // Get screen information
        struct fb_var_screeninfo vscreeninfo;
        struct fb_fix_screeninfo fscreeninfo;
        ioctl(m_fbdev, FBIOGET_VSCREENINFO, &vscreeninfo);
        ioctl(m_fbdev, FBIOGET_FSCREENINFO, &fscreeninfo);

        m_width			= vscreeninfo.xres;
        m_height		= vscreeninfo.yres;
        m_linelength	= fscreeninfo.line_length;
        m_screensize	= m_linelength * m_height; //fscreeninfo.smem_len;
        m_bytesperpixel	= vscreeninfo.bits_per_pixel/8;

        cout << "   m_width         = " << m_width << endl 
            << "   m_height        = " << m_height << endl
            << "   m_linelength    = " << m_linelength << endl
            << "   m_screensize    = " << m_screensize << endl
            << "   m_bytesperpixel = " << m_bytesperpixel << endl;

        ioctl (m_fbdev, FBIOPAN_DISPLAY, &vscreeninfo);

        // Map to video memory (much faster than calling lseek and write a million times)
        m_screen = (unsigned char*)mmap(0, m_screensize, PROT_READ | PROT_WRITE, MAP_SHARED, m_fbdev, 0);
        if( m_screen == MAP_FAILED )
        {
            throw error_message_exception("could not mmap fbdev");
        }
    }

    surface_fbdev::~surface_fbdev()
    {
        if( m_fbdev )
            close(m_fbdev);
        m_fbdev = 0;

        if( m_screen )
            munmap(m_screen, m_screensize);
        m_screen = (unsigned char*)0;
        INFO("~surface_fbdev");
    }

    surface_fbdev::ptr surface_fbdev::create()
    {
        surface_fbdev::ptr res(new surface_fbdev(boost::optional<rectangle>(),true));

        return res;
    }

    surface_fbdev::ptr surface_fbdev::create(const rectangle& rect, bool primary)
    {
        surface_fbdev::ptr res(new surface_fbdev(boost::optional<rectangle>(rect),primary));
        return res;
    }

    void surface_fbdev::put_pixel(int x, int y, color clr)
    {
        if (x < 0 || (x > m_width-1) || y < 0 || (y > m_height-1))
            return;

        unsigned char red = (clr >> 24) & 0xff;
        unsigned char green = (clr >> 16) & 0xff;
        unsigned char blue = (clr >> 8) & 0xff;
        unsigned char alpha = (clr >> 0) & 0xff;

        /*
           red = (red * alpha) / 255;
           green = (green * alpha) / 255;
           blue = (blue * alpha) / 255;
           */

        switch (m_bytesperpixel)
        {
            case 1: // 8 bit color
                *(unsigned char*)&m_screen[m_linelength*y + x] = 
                    BRUN(7,6) & (red >> 0) |
                    BRUN(5,3) & (green >> 2) |
                    BRUN(2,0) & (blue >> 5);
                //((red<<4) & 0xE0) | ((green) & 0x18) | ((blue>>4) & 0x03);
                break;
            case 2:	// 16-bit (64k) color mode
                //cout << "before put (" << x << ", " << y << ")" << endl;
                *(unsigned short*)&m_screen[m_linelength*y + m_bytesperpixel*x] = 
                    ((red<<8) & 0xF800) | ((green<<3) & 0x07E0) | ((blue>>3) & 0x001F);
                //cout << "after put" << endl;
                break;
            case 3: // 24 bit color
                // same as 32bpp ??

            case 4:	// 32-bit (Real) color mode
                {
                    int offset = m_linelength*y + m_bytesperpixel*x;
                    m_screen[offset+0] = blue;
                    m_screen[offset+1] = green;
                    m_screen[offset+2] = red;
                }
                break;
            default:
                throw error_message_exception("unsupported bitdepth");
                break;
        }
    }

    void surface_fbdev::put_pixel_aa(int x, int y, double distance, color clr)
    {
        //      clr&=~0xff;

        put_pixel(x,y,clr);
    }

    void surface_fbdev::put_pixel_aa(int x, int y, unsigned char alpha_a, color clr)
    {
        // FIXME: confirm the format of clr
        // will it always be in SDL color format?
        // or will it always be in some 32 bit stk RGBA format ??
        unsigned char red_a, green_a, blue_a;
        red_a = (clr >> 24) & 0xff;
        green_a = (clr >> 16) & 0xff;
        blue_a = (clr >> 8) & 0xff;

        unsigned char red_b, green_b, blue_b, alpha_b;
        color color_b = get_pixel(x, y);
        red_b = (color_b >> 24) & 0xff;
        green_b = (color_b >> 16) & 0xff;
        blue_b = (color_b >> 8) & 0xff;
        alpha_b = 0xff; //(color_b >> 0) & 0xff;

        unsigned char red_o, green_o, blue_o, alpha_o;
        alpha_o = composite_alpha(alpha_a, alpha_b);
        float alpha_a_f = alpha_a / 255.0;
        float alpha_b_f = alpha_b / 255.0;
        float alpha_o_f = alpha_o / 255.0;

        red_o   = composite_a_over_b(red_a, red_b, alpha_a_f, alpha_b_f, alpha_o_f);
        green_o = composite_a_over_b(green_a, green_b, alpha_a_f, alpha_b_f, alpha_o_f);
        blue_o  = composite_a_over_b(blue_a, blue_b, alpha_a_f, alpha_b_f, alpha_o_f);

        put_pixel(x, y, gen_color(red_o, green_o, blue_o, alpha_o));
    }

    color surface_fbdev::get_pixel(int x, int y) const
    {
    }

    color surface_fbdev::gen_color(const std::string &str_color) const
    {
        unsigned long int_color = strtoll(str_color.c_str(), NULL, 16);
        return int_color;
    }

    color surface_fbdev::gen_color(byte r, byte g, byte b, byte a) const
    {
        return r << 24 | g << 16 | b << 8 | a;
    }

    void surface_fbdev::lock(rectangle &rect, int flags, color** buf, int &stride)
    {
    }

    void surface_fbdev::unlock()
    {
    }

    void surface_fbdev::update(const rectangle& u_rect)
    {
    }

    overlay::ptr surface_fbdev::create_overlay(int width, int height, int format)
    {
        cout << "surface_fbdev::create_overlay() - not implemented" << endl;
        return overlay::ptr();
    }

    // overridden drawing routines
    void surface_fbdev::blit(surface &dst_surface)
    {

    }

    void surface_fbdev::fill_rect(int x1, int y1, int x2, int y2)
    {
        color mcolor = (color)gc_->fill_color();
        for (int x=x1; x<x2; x++)
            for (int y=y1; y<y2; y++)
                put_pixel(x, y, mcolor);
    }

    void surface_fbdev::fill_rect(const rectangle& rect)
    {
        color mcolor = (color)gc_->fill_color();
        for (int x=rect.x1(); x<rect.x2(); x++)
            for (int y=rect.y1(); y<rect.y2(); y++)
                put_pixel(x, y, mcolor);
    }

}
