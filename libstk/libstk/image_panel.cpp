/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: image_panel class implementation.
 *     AUTHORS: Darren Hart, Marc Strämke 
 *  START DATE: 2003/Apr/14
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include "libstk/image_panel.h"
#include "libstk/image.h"
#include "libstk/logging.h"

namespace stk
{
    image_panel::ptr image_panel::create(container::ptr parent, const rectangle& rect,
                                         image::ptr img)
    {
        image_panel::ptr new_image_panel(new image_panel(parent, rect, img));
        parent->add(new_image_panel);
        return new_image_panel;
    }

    image_panel::image_panel(widget::ptr parent, const rectangle& rect, image::ptr img)
            : widget(parent, rect), image_(img)
    {
        INFO("constructor");
    }

    image_panel::~image_panel()
    {
        INFO("destructor");
    }

}
