/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Implementation of font manager classes.
 *     AUTHORS: Marc Str�mke, Darren Hart, Vernon Mauery
 *  START DATE: 2003/May/13
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Str�mke, Dirk H�rner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include <iostream>
#include "libstk/font_manager.h"
#include "libstk/logging.h"

namespace stk
{

    font_manager::ptr font_manager::instance_;

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
            font::ptr newfont = font::create(properties);
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
            font::ptr newfont = font::create(properties);
            fonts[properties] = newfont;
            return newfont;
        }
    }

}
