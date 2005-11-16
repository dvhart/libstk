/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Implementation of font properties class.
 *     AUTHORS: Marc Strämke, Darren Hart, Vernon Mauery
 *  START DATE: 2003/May/13
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include <iostream>
#include "libstk/font_manager.h"
#include "libstk/logging.h"

namespace stk
{

    font_properties::font_properties(std::string name, int height, font_style style, float rotation)
    : fontname(name), height(height), style(style), rotation(rotation)
    {
    }

    bool font_properties::operator<(const font_properties& rhs) const
    {
        // check the high order
        if ((fontname ? *fontname : "") < (rhs.fontname ? *rhs.fontname : "")) 
            return true;
        // with each equal, check the lower order level
        if ((fontname ? *fontname : "") == (rhs.fontname ? *rhs.fontname : ""))
        {
            if ((height ? *height : 0) < (rhs.height ? *rhs.height : 0)) 
                return true;
            if ((height ? *height : 0) == (rhs.height ? *rhs.height : 0))
            {
                if ((rotation ? *rotation : 0) < (rhs.rotation ? *rhs.rotation : 0)) 
                    return true;
                if ((rotation ? *rotation : 0) == (rhs.rotation ? *rhs.rotation : 0)) 
                    return ((style ? *style : plain) < (rhs.style ? *rhs.style : plain));
            }
        }
        return false;
    }

    void font_properties::merge(const font_properties& rhs)
    {
        if (!fontname) fontname = rhs.fontname;
        if (!height) height = rhs.height;
        if (!style) style = rhs.style;
        if (!rotation) rotation = rhs.rotation;
    }
} // namespace stk
