/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: An image widget.
 *     AUTHORS: Darren Hart, Marc Strämke
 *  START DATE: 2003/Apr/14
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_IMAGE_PANEL_H
#define STK_IMAGE_PANEL_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/signal.hpp>
#include <libstk/rectangle.h>
#include <libstk/widget.h>
#include <libstk/container.h>
#include <libstk/image.h>

namespace stk
{
    class image_panel : public widget
    {
    public:
        typedef boost::shared_ptr<image_panel> ptr;
        typedef boost::weak_ptr<image_panel> weak_ptr;

    private:

    protected:
        image_panel(const rectangle& rect = rectangle(), image::ptr img = image::ptr());
        image::ptr image_;

    public:
        static image_panel::ptr create(container::ptr parent, const rectangle& _rect, 
                image::ptr img = image::ptr());
        virtual ~image_panel();

        /********** EVENT HANDLER INTERFACE **********/
        /********** END EVENT HANDLER INTERFACE **********/

        /********** DRAWABLE INTERFACE **********/
        virtual void draw(surface::ptr surface, const rectangle& clip_rect = rectangle());
        /********** END DRAWABLE INTERFACE **********/

        /********** COMPONENT INTERFACE **********/
        /********** END COMPONENT INTERFACE **********/

        /********** WIDGET INTERFACE **********/
        /********** END WIDGET INTERFACE **********/

        /********** IMAGE_PANEL INTERFACE **********/
        image::ptr getimage() const { return image_; }
        void setimage(image::ptr img)
        {
            image_ = img;
            redraw(rect());
        }
        /********** END IMAGE_PANEL INTERFACE **********/
    };
}

#endif
