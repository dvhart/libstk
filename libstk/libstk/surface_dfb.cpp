/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Direct FB surface implementation.
 *     AUTHORS: Marc Strämke, Darren Hart
 *  START DATE: 2003/Mar/03
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page = docs/license
 *************************************************************************************************/

#include "libstk/surface_dfb.h"
#include "libstk/backend_dfb.h"
#include "libstk/overlay.h"
#include "libstk/overlay_dfb.h"
#include "logging.h"
#include <iostream>

namespace stk
{
    surface_dfb::surface_dfb(boost::optional<rectangle> rect, bool primary)
    {
        DFBSurfaceDescription dsc;
        memset(&dsc, 0, sizeof(dsc));

        bool rect_specified = rect;
        
        dsc.flags = DSDESC_CAPS;
        backend_handle = backend_dfb::get();

        if (primary)
        {
            dsc.caps = static_cast<DFBSurfaceCapabilities>( DSCAPS_FLIPPING | DSCAPS_PRIMARY) ;
        }
        else
        {
            dsc.flags = (DFBSurfaceDescriptionFlags)( dsc.flags | DSDESC_PIXELFORMAT);
            dsc.pixelformat = backend_handle->format;
            dsc.pixelformat = DSPF_ARGB; // \FIXME Hardcode a format with alpha for now!
        }

        if (rect_specified)
        {
            dsc.flags = (DFBSurfaceDescriptionFlags)( dsc.flags | DSDESC_WIDTH | DSDESC_HEIGHT);
            dsc.width = (*rect).width();
            dsc.height = (*rect).height();
        }

        IDirectFB* dfb = backend_handle->get_interface();
        
        dfb->CreateSurface(dfb, &dsc, &surface);
        rect_ = *rect;

        if (primary)
        {
            surface->GetPixelFormat(surface, &backend_handle->format);
            INFO("Pixel format is = " << backend_handle->format);
        }
        
        INFO("surface create");
    }
    
    surface_dfb::~surface_dfb()
    {
        INFO("destructor");
        surface->Release(surface);
    }
    
    surface_dfb::ptr surface_dfb::create()
    {
        surface_dfb::ptr new_surface_dfb(new surface_dfb(boost::optional<rectangle>(), true));
        return new_surface_dfb;
    }
    
    surface_dfb::ptr surface_dfb::create(const rectangle& rect, bool primary)
    {
        surface_dfb::ptr res(new surface_dfb(boost::optional<rectangle>(rect), primary));
        return res;
    }

    surface::ptr surface_dfb::create_surface(const rectangle& rect)
    {
        return surface_dfb::create(rect, false);
    }
    
    void surface_dfb::put_pixel(int x, int y, color clr)
    {
        x += offset_.x();
        y += offset_.y();
        surface->SetDrawingFlags(surface, DSDRAW_BLEND);
        surface->SetColor(surface, (clr&0xff000000)>>24, (clr&0xff0000)>>16, (clr&0xff00)>>8, clr&0xff);
        // FIXME: sometimes this draws a line from 0,0 to x,y
        // FIXME: how do we get direct pixel access
        surface->DrawLine(surface, x, y, x, y); // hack *ggg*
    }
    
    void surface_dfb::put_pixel_aa(int x, int y, double distance, color clr)
    {
//      clr &= ~0xff;
        put_pixel(x, y, clr);
    }
    
    void surface_dfb::put_pixel_aa(int x, int y, unsigned char alpha_a, color clr)
    {
        clr &= ~0xff;
        clr |= alpha_a;
        //if(alpha_a>230
        put_pixel(x, y, clr);
    }
    
    color surface_dfb::get_pixel(int x, int y) const
    {
        x += offset_.x();
        y += offset_.y();
        WARN("surface_dfb::get_pixel(x, y)");
    }
    
    color surface_dfb::gen_color(const std::string &str_color) const
    {
        unsigned long int_color = strtoll(str_color.c_str(), NULL, 16);
        return int_color;
    }
    
    color surface_dfb::gen_color(byte r, byte g, byte b, byte a) const
    {
        return r << 24 | g << 16 | b << 8 | a;
    }
    
    void surface_dfb::lock()
    {
    }
    
    void surface_dfb::lock(rectangle &rect, int flags, color** buf, int &stride)
    {
    }
    
    void surface_dfb::unlock()
    {
    }
    
    void surface_dfb::update(const rectangle& u_rect)
    {
        DFBRegion region;
        region.x1 = u_rect.x1();
        region.y1 = u_rect.y1();
        region.x2 = u_rect.x2();
        region.y2 = u_rect.y2();
        
        surface->Flip(surface, &region, DSFLIP_BLIT); 
    }

    overlay::ptr surface_dfb::create_overlay(int width, int height, int format)
    {
        // FIXME: should it be created with caps = DSCAPS_SUBSURFACE
        overlay::ptr new_overlay = overlay_dfb::create(width, height, format);
        return new_overlay;
    }
    
    // overridden drawing routines
    void surface_dfb::blit(stk::surface& dst_surface)
    {
        // FIXME: implement me!
        WARN("surface_dfb::blit(surface) not implemented");
    }
    
    void surface_dfb::blit(stk::surface& dst_surface, rectangle src_rect, rectangle dst_rect)
    {
        INFO("blit: src(" << src_rect << ")  dst_rect(" << dst_rect << ")");
        surface_dfb* dst = dynamic_cast<surface_dfb*>(&dst_surface);
        if (dst != NULL)
        {
            dst_rect.x1(dst_rect.x1()+dst_surface.offset().x());
            dst_rect.y1(dst_rect.y1()+dst_surface.offset().y());
            
            DFBRectangle source_rect;
            source_rect.x = src_rect.x1();
            source_rect.y = src_rect.y1();
            source_rect.w = src_rect.width();
            source_rect.h = src_rect.height();

            dst->surface->Blit(dst->surface, surface, &source_rect, dst_rect.x1(), dst_rect.y1());
        }        
    }
    
    void surface_dfb::fill_rect(int x1, int y1, int x2, int y2)
    {
        x1 += offset_.x();
        y1 += offset_.y();
        x2 += offset_.x();
        y2 += offset_.y();
        color clr = gc_->fill_color();
        surface->SetColor(surface, (clr&0xff000000)>>24, (clr&0xff0000)>>16, (clr&0xff00)>>8, 0xff);
        surface->FillRectangle(surface, x1, y1, x2-x1, y2-y1);
    }
    
    void surface_dfb::fill_rect(const rectangle& rect)
    {
        fill_rect(rect.x1(), rect.y1(), rect.x2(), rect.y2());
    }
    
    void surface_dfb::draw_line(int x1, int y1, int x2, int y2)
    {
        x1 += offset_.x();
        y1 += offset_.y();
        x2 += offset_.x();
        y2 += offset_.y();
        color clr = gc_->line_color();
        surface->SetColor(surface, (clr&0xff000000)>>24, (clr&0xff0000)>>16, (clr&0xff00)>>8, 0xff);
        surface->DrawLine(surface, x1, y1, (x2-x1), (y2-y1));
    }
    
    void surface_dfb::draw_rect(const rectangle& rect)
    {
        draw_rect(rect.x1(), rect.y1(), rect.x2(), rect.y2());
    }
    
    void surface_dfb::draw_rect(int x1, int y1, int x2, int y2)
    {
        x1 += offset_.x();
        y1 += offset_.y();
        x2 += offset_.x();
        y2 += offset_.y();
        color clr = gc_->line_color();
        surface->SetColor(surface, (clr&0xff000000)>>24, (clr&0xff0000)>>16, (clr&0xff00)>>8, 0xff);
        surface->DrawRectangle(surface, x1, y1, (x2-x1), (y2-y1));
    }
    
    void surface_dfb::clip_rect(const rectangle& clip_rectangle)
    {
        // FIXME: this doesn't set the clip_rect_ in the surface_dfb class!
        if (clip_rectangle.empty())
            surface->SetClip(surface, NULL);
        else
        {
            DFBRegion region;
            region.x1 = clip_rectangle.x1();
            region.x2 = clip_rectangle.x2();
            region.y1 = clip_rectangle.y1();
            region.y2 = clip_rectangle.y2();
            
            surface->SetClip(surface, &region); 
        }
    }
}
