/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: A class used by surface to store colors, line styles, etc.
 *     AUTHORS: Darren Hart, Vernon Mauery, Marc Strämke
 *  START DATE: 2003/Dec/10
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_GRAPHICS_CONTEXT_H
#define STK_GRAPHICS_CONTEXT_H

#include <libstk/stk_types.h>
#include <libstk/font.h>

namespace stk
{
    class graphics_context
    {
    public:
        typedef boost::shared_ptr<graphics_context> graphics_context::ptr;

        /// Enumeration of the types of line segment joints
        enum line_joint_type
        {
            /// outer corners are rounded with radius line_width/2, inner corners are sharp
            round,
            /// divide the angle in half and allow the lines to intersect
            mitre,
            /// like mitre but trimmed perpendicular to the mitre at a certain distance
            trimmed_mitre
        };

        /// Enumeration of the types of line caps
        enum line_cap_type
        {
            /// line ends are capped with discs of radius line_width/2
            disc,
            /// line ends are cut perpendicular to their centerline
            square
        };

    private:
        /// The width in pixels of the pen used to draw primitives
        int line_width_;
        /// The type of joint drawn between line segments: round, mitre, trimmed_mitre
        line_joint_type line_joint_;
        /// The way line ends are drawn: disc, square
        line_cap_type line_cap_;
        color line_color_;
        color fill_color_;
        // font stuff
        stk::font::ptr font_;
        color font_fill_color_;
        color font_outline_color_;
    protected:
        graphics_context()
        {}

    public:
        static graphics_context::ptr create()
        {
            return graphics_context::ptr(new graphics_context());
        }
        virtual ~graphics_context()
        {}
        // drawing style
        int line_width() const
        {
            return line_width_;
        }
        void line_width(int val)
        {
            line_width_ = val;
        }
        line_joint_type line_joint() const
        {
            return line_joint_;
        }
        void line_joint(line_joint_type val)
        {
            line_joint_ = val;
        }
        line_cap_type line_cap() const
        {
            return line_cap_;
        }
        void line_cap(line_cap_type val)
        {
            line_cap_ = val;
        }
        color line_color() const
        {
            return line_color_;
        }
        void line_color(color clr)
        {
            line_color_ = clr;
        }
        color fill_color() const
        {
            return fill_color_;
        }
        void fill_color(color clr)
        {
            fill_color_ = clr;
        }
        // font stuff
        stk::font::ptr font() const
        {
            return font_;
        }
        void font(stk::font::ptr f)
        {
            font_ = f;
        }
        color font_fill_color() const
        {
            return font_fill_color_;
        }
        void font_fill_color(color clr)
        {
            font_fill_color_ = clr;
        }
        color font_outline_color() const
        {
            return font_outline_color_;
        }
        void font_outline_color(color clr)
        {
            font_outline_color_ = clr;
        }
    };
}

#endif
