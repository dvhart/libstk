/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: A generic image class.
 *     AUTHORS: Darren Hart, Marc Strämke
 *  START DATE: 2003/Mar/03
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_IMAGE_H
#define STK_IMAGE_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <libstk/rectangle.h>
#include <libstk/surface.h>

namespace stk
{
    class image
    {
    public:
        typedef boost::shared_ptr<image> ptr;
        typedef boost::weak_ptr<image> weak_ptr;
    private:
    protected:
        image(stk::surface::ptr onscreen_surface,const std::string& filename="");
        image(stk::surface::ptr onscreen_surface,const rectangle& rect);
        void init_pixels();
        int width_;
        int height_;
        stk::surface::ptr onscreen_surface; /// Used for offscreen surface construction
    public:
        static image::ptr create(stk::surface::ptr onscreen_surface,const std::string& filename="");
        static image::ptr create(stk::surface::ptr onscreen_surface,const rectangle& _rect);
        virtual ~image();

        stk::surface::ptr offscreen_surface;
        /********** IMAGE INTERFACE **********/
        void load_ppmx(const std::string filename);
        void load_png(const std::string filename);
        
        //color pixel(int x, int y) const; // \FIXME Needed anymore if we are using offscreen surfaces?
        //void pixel(int x, int y, color new_color); // \FIXME Needed anymore if we are using offscreen surfaces?
        //int width() // \FIXME Needed anymore if we are using offscreen surfaces?
        //{
        //    return width_;
        //}
        //int height() // \FIXME Needed anymore if we are using offscreen surfaces?
        //{
        //    return height_;
        //}
        /********** END IMAGE INTERFACE **********/
    };
}

#endif
