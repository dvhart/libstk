/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Color manager class implementations.
 *     AUTHORS: Darren Hart
 *  START DATE: 2003/May/05
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include <iostream>
#include "libstk/color_manager.h"
#include "libstk/logging.h"

namespace stk
{
    color_manager::ptr color_manager::instance_;

    bool color_properties::operator<(const color_properties& rhs) const
    {
        // check the high order
        if (int_color < rhs.int_color)
            return true;
        // check the low order
        if (int_color == rhs.int_color)
            return (surface_ < rhs.surface_);
        return false;
    }

    color_manager::ptr color_manager::get()
    {
        if (!instance_) instance_.reset(new color_manager());
        return instance_;
    }
    
    color color_manager::get_color(const color_properties& properties)
    {
        //INFO("color_manager::get_color()");
        //INFO("\trgba: " << std::hex << properties.red << " " << properties.green
        //    << " " << properties.blue << " " << properties.alpha << " -> ";

        Tcolor_map::iterator color_iter = color_map_.find(properties);
        if (color_iter == color_map_.end())
        {
            //INFO("color not yet created...";
            color_map_[properties] = properties.surface_->gen_color(properties.red,
                                     properties.green, properties.blue, properties.alpha);
            //INFO("created: " << std::hex << color_map_[properties]);
            return color_map_[properties];
        }
        else
        {
            //INFO("cached: " << std::hex << color_iter->second);
            return color_iter->second;
        }
    }


}
