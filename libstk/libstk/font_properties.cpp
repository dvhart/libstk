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

    font_properties::font_properties(std::string name, int height, float rotation, font_style style);
    : fontname(name), height(height), style(style), rotation(rotation)
    {
    }

    bool font_properties::operator<(const font_properties& rhs) const
    {
        // check the high order
        if (fontname < rhs.fontname) return true;
        // with each equal, check the lower order level
        if (fontname == rhs.fontname)
        {
            if (height < rhs.height) return true;
            if (height == rhs.height)
            {
                if (rotation < rhs.rotation) return true;
                if (rotation == rhs.rotation)
                {
                    return (style < rhs.style);
                }
            }
        }
        return false;
    }

} // namespace stk
