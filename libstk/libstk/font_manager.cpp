/**************************************************************************************************
 *    FILENAME: font_manager.cpp
 * DESCRIPTION: Implementation of font manager classes.
 *     AUTHORS: Marc Straemke, Darren Hart
 *  START DATE: 13/May/2003  LAST UPDATE: 25/Jun/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *     license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include "libstk/font_manager.h"

#include <iostream>
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
        std::cout << "Font manager constructed\n";
    }
    
    font_manager::~font_manager()
    {
        std::cout << "Font manager destructed\n";
    }

    font::ptr font_manager::get_font(const font_properties& properties)
    {
        //std::cout << "getting font " << properties.fontname << " " << properties.height << " ";
        Tfonts::iterator font_iter = fonts.find(properties);
        if (font_iter == fonts.end())
        {
            //std::cout << " creating" << std::endl;
            font::ptr newfont(new font(properties.fontname,properties.height,0));
            fonts[properties] = newfont;
            return newfont;
        }
        else
        {
            font::ptr font_ptr = font_iter->second;
            if (font_ptr)
            {
                //std::cout << "cached at " << font_iter->first.fontname 
                //          << " " << font_iter->first.height << std::endl;
                return font_ptr;
            }
            //std::cout << "uncached (error)" << std::endl;
            font::ptr newfont(new font(properties.fontname, properties.height,0));
            fonts[properties] = newfont;
            return newfont;
        }
    }

}
