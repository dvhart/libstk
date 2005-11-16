/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Classes to manage colors, caching them by surface and RGBA.
 *     AUTHORS: Darren Hart 
 *  START DATE: 2003/May/05
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Str�mke, Dirk H�rner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *     license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_COLOR_MANAGER_H
#define STK_COLOR_MANAGER_H

#include <boost/smart_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <string>
#include <map>
#include <libstk/surface.h>

namespace stk
{
    /// Public color properties class.
    struct color_properties
    {
        /// fixme: these are redundant with int_color, but we use them to create the color, perhaps
        /// we should just store the std::string in 0xRRGGBBAA.  Maybe pack the color and add 
        /// access like my_color.red, etc.
        unsigned char red, green, blue, alpha;
        /// The color represented as 0xRRGGBBAA, used for comparison in the map.
        unsigned int int_color;
        surface::ptr surface_;
        color_properties(unsigned char r, unsigned char g, unsigned char b, unsigned char a, 
                surface::ptr s) : red(r), green(g), blue(b), alpha(a), surface_(s)
        {
            int_color = (r << 24)|(g << 16)|(b << 8)|a;
        }
        color_properties(const std::string& rgba, surface::ptr s)
                : surface_(s)
        {
            int_color = strtoll(rgba.c_str(), NULL, 16);
            red = (int_color & 0xFF000000) >> 24;
            green = (int_color & 0x00FF0000) >> 16;
            blue = (int_color & 0x0000FF00) >> 8;
            alpha = (int_color & 0x000000FF);
        }

        /// Less than comparison operator used by color_manager's map.
        bool operator<(const color_properties& rhs) const;
    };

    /// Static class for managing surface colors.
    /// This class manages all creation and retrieval of color instances, the
    /// colors are stored according to surface and rgba values.
    class color_manager
    {
    public:
        typedef boost::shared_ptr<color_manager> ptr;
        typedef boost::weak_ptr<color_manager> weak_ptr;
        typedef std::map<color_properties, color> Tcolor_map;

    private:
        static color_manager::ptr instance_;
        Tcolor_map color_map_;

    public:
        static color_manager::ptr get();
        /// Get a color with the provided Properties
        color get_color(const color_properties& properties);
    };

}

#endif
