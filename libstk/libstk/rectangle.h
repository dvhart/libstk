/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: A simple rectangle class with some utilities like contains().
 *     AUTHORS: Darren Hart, Marc Str�mke
 *  START DATE: 2002/Dec/10
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Str�mke, Dirk H�rner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_RECTANGLE_H
#define STK_RECTANGLE_H

#include <libstk/stk_types.h>
#include <libstk/point.h>
#include <libstk/exceptions.h>

namespace stk
{
    class rectangle
    {
    private:
        point p1_, p2_;

    protected:

    public:
        rectangle(int x1 = 0, int y1 = 0, int w = 0, int h = 0);
        rectangle(const point &p1, const point &p2);

        // getters
        point p1() const { return p1_; }
        point p2() const { return p2_; }
        int x1() const { return p1_.x(); }
        int y1() const { return p1_.y(); }
        int x2() const { return p2_.x(); }
        int y2() const { return p2_.y(); }
        int width() const { return p2_.x() - p1_.x() + 1; } // inclusive width
        int height() const { return p2_.y() - p1_.y() + 1; } // inclusive height

        // setters
        void x1(int val) { p1_.x(val); }
        void y1(int val) { p1_.y(val); }
        void x2(int val) { p2_.x(val); }
        void y2(int val) { p2_.y(val); }
        void width(int val) { p2_.x(val + p1_.x() - 1); }
        void height(int val) { p2_.y(val + p1_.y() - 1); }

        // utilities
        /// Returns True if the Rectangle is Empty(w==0 h==0)
        bool empty() const { return (!width() && !height()); }
        /// Returns True if the given Point lies inside the rectangle
        bool contains(int x, int y) const;
        /// Returns True if the given Point lies inside the rectangle
        bool contains(const point& p) const;
        /// Returns True if the given rectangle intersects with this one
        bool intersects(const rectangle& rect) const;
        /// Returns the Lower Left Corner of the Rectangle
        point position() const { return p1_; }
        /// Sets the Lower Left Corner of the Rectangle
        void position(int x, int y);
        /// Sets the Lower Left Corner of the Rectangle
        void position(const point& p);
        /// Returns the Area of intersection between both rectangles (the area they share)
        rectangle intersection(rectangle &rhs) const;

        // operators
        const rectangle operator+(const rectangle& rect) const;
        const rectangle& operator+=(const rectangle& rect);
    };
}
std::ostream& operator<<(std::ostream& os, const stk::rectangle& rect);
#endif
