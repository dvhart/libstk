/**************************************************************************************************
 *    FILENAME: font_manager.cpp
 * DESCRIPTION: Implementation of font manager classes.
 *     AUTHORS: Marc Straemke, Darren Hart
 *  START DATE: 13/May/2003  LAST UPDATE: 02/Aug/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *     license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include <iostream>
#include "libstk/font_manager.h"
#include "libstk/logging.h"

namespace stk
{

    font_manager::ptr font_manager::instance_;


    bool font_properties::operator<(const font_properties& rhs) const
    {
        // check the high order
        if (fontname < rhs.fontname) return true;
        // check the low order
        if (fontname == rhs.fontname) return (height < rhs.height);
        return false;
    }
    
    font_manager::ptr font_manager::get()
    {
        if (!instance_) instance_.reset(new font_manager());
        return instance_;
    }
    
    font_manager::font_manager()
    {
        INFO("constructor");
    }
    
    font_manager::~font_manager()
    {
        INFO("destructor");
    }

    font::ptr font_manager::get_font(const font_properties& properties)
    {
        Tfonts::iterator font_iter = fonts.find(properties);
        if (font_iter == fonts.end())
        {
            font::ptr newfont(new font(properties.fontname,properties.height,0));
            fonts[properties] = newfont;
            return newfont;
        }
        else
        {
            font::ptr font_ptr = font_iter->second;
            if (font_ptr)
            {
                return font_ptr;
            }
            font::ptr newfont(new font(properties.fontname, properties.height,0));
            fonts[properties] = newfont;
            return newfont;
        }
    }

}
