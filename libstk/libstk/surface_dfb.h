/**************************************************************************************************
 *    FILENAME: surface_dfb.h
 * DESCRIPTION: The surface backend for Direct FB.
 *     AUTHORS: Darren Hart
 *  START DATE: 13/May/2003  LAST UPDATE: 20/Jul/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include <vector>
#include <boost/optional.hpp>
#include <libstk/surface.h>
#include <libstk/surface_impl.h>
#include <libstk/point.h>
#include <libstk/stk_types.h>
#include <libstk/backend_dfb.h>

#define MIN(A,B) (((A)<(B))?(A):(B))

namespace stk
{
    class surface_dfb : public surface_impl<surface_dfb>
    {
    public:
        typedef boost::shared_ptr<surface_dfb> ptr;
        typedef boost::weak_ptr<surface_dfb> weak_ptr;
    protected:
        IDirectFBSurface *surface;
        surface_dfb(boost::optional<rectangle> rect, bool primary);

        backend_dfb::ptr backend_handle;
        
    public:
        static surface_dfb::ptr create(const rectangle& rect, bool primary=true); // Create with given screen size
        static surface_dfb::ptr create(); // Create with screen size without switching resolutions, has to be used ONLY for creating the primary surface
        
        ~surface_dfb();

        // methods which MUST be implemented in derived classes
        inline void put_pixel(int x, int y, color clr);
        inline void put_pixel_aa(int x, int y, double distance, color clr);
        inline void put_pixel_aa(int x, int y, unsigned char alpha_a, color clr);
        inline color get_pixel(int x, int y) const;
        virtual color gen_color(const std::string &str_color) const;
        virtual color gen_color(byte r, byte g, byte b, byte a) const;
        virtual void lock(rectangle &rect, int flags, color** buf, int &stride);
        virtual void unlock();
        virtual void update(const rectangle& u_rect=rectangle());
        virtual boost::shared_ptr<overlay> create_overlay(int width, int height, int format);

        // overridden drawing routines
        virtual void blit(stk::surface &dst_surface);
        virtual void blit(stk::surface &dst_surface, rectangle src_rect, rectangle dst_rect);
        virtual void fill_rect(int x1, int y1, int x2, int y2);
        virtual void fill_rect(const rectangle& rect);
        virtual void draw_line(int x1, int y1, int x2, int y2) ;
        virtual void draw_rect(const rectangle& rect);
        virtual void draw_rect(int x1, int y1, int x2, int y2);
    };
} //end namespace stk
