/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: The default theme for Libstk, the user_theme class is declared
 *              here.
 *     AUTHORS: Darren Hart 
 *  START DATE: 2003/May/05
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_TRIBAL_THEME_H
#define STK_TRIBAL_THEME_H

#include <libstk/surface.h>

namespace stk
{

    // some tribal colors for this theme
    const std::string outline_color_normal_str  = "0xFFFFFFFF";
    const std::string outline_color_focused_str = "0xFFFFFFFF";
    const std::string outline_color_hover_str   = "0xFFFF00FF";
    const std::string outline_color_pressed_str = "0xFFFF00FF";

    const std::string fill_state_color_str      = "0x0C2C4CFF";
    const std::string fill_color_normal_str     = "0x185899FF";
    const std::string fill_color_focused_str    = "0x2893FFFF";
    const std::string fill_color_hover_str      = "0x185899FF";
    const std::string fill_color_pressed_str    = "0x2893FFFF";

    const std::string font_color_normal_str     = "0xFFFFFFFF";
    const std::string font_color_focused_str    = "0xFFFFFFFF";
    const std::string font_color_hover_str      = "0xFFFF00FF";
    const std::string font_color_pressed_str    = "0xFFFF00FF";

    /// The user_theme class is for user created theme data and methods.
    /// This is class is defined by the user (theme programmer) and found at link
    /// time (it is forward declared in theme)
    class user_theme
    {
    public:
        typedef boost::shared_ptr<user_theme> ptr;
        typedef boost::weak_ptr<user_theme> weak_ptr;

    private:
        // theme specific colors
        color outline_color_normal_;
        color outline_color_focused_;
        color outline_color_hover_;
        color outline_color_pressed_;

        color fill_state_color_;
        color fill_color_normal_;
        color fill_color_focused_;
        color fill_color_hover_;
        color fill_color_pressed_;

        color font_color_normal_;
        color font_color_focused_;
        color font_color_hover_;
        color font_color_pressed_;

    protected:

    public:
        user_theme(surface::ptr surface);
        ~user_theme();

        // tribal_theme drawing routines
        /// Draw an arrow in dir o'clock with x,y as its upper left corner.
        void draw_arrow(int x, int y, int dir, surface::ptr surface, bool fill);

        // access to theme colors
        color outline_color_normal() const
        {
            return outline_color_normal_;
        }
        color outline_color_focused() const
        {
            return outline_color_focused_;
        }
        color outline_color_hover() const
        {
            return outline_color_hover_;
        }
        color outline_color_pressed() const
        {
            return outline_color_pressed_;
        }

        color fill_color_normal() const
        {
            return fill_color_normal_;
        }
        color fill_color_focused() const
        {
            return fill_color_focused_;
        }
        color fill_color_hover() const
        {
            return fill_color_hover_;
        }
        color fill_color_pressed() const
        {
            return fill_color_pressed_;
        }

        color font_color_normal() const
        {
            return font_color_normal_;
        }
        color font_color_focused() const
        {
            return font_color_focused_;
        }
        color font_color_hover() const
        {
            return font_color_hover_;
        }
        color font_color_pressed() const
        {
            return font_color_pressed_;
        }

    };
}

#endif
