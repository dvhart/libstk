/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: A simple rectangle class with some utilities like contains().
 *     AUTHORS: Darren Hart, Marc Strämke
 *  START DATE: 2002/Dec/10
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
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
    /// dvhart docs of the rectangle class
    class rectangle
    {
    private:

    protected:
        point p1_, p2_;

    public:
        rectangle(int x1 = 0, int y1 = 0, int w = 0, int h = 0);
        rectangle(const point &p1, const point &p2);
        virtual ~rectangle() { }

        // getters
        /// Returns the upper left corner of the rectangle (this is duplicated by position())
        virtual point p1() const { return p1_; }
        /// Returns the lower right corner of the rectangle
        virtual point p2() const { return p2_; }
        /// Returns the x coordinate of the left edge
        virtual int x1() const { return p1_.x(); }
        /// Returns the y coordinate of the top edge
        virtual int y1() const { return p1_.y(); }
        /// Returns the x coordinate of the right edge
        virtual int x2() const { return p2_.x(); }
        /// Returns the y coordinate of the bottom edge
        virtual int y2() const { return p2_.y(); }
        virtual int width() const { return p2_.x() - p1_.x() + 1; }  // inclusive width
        virtual int height() const { return p2_.y() - p1_.y() + 1; } // inclusive height

        // setters
        virtual void p1(const point&  p);
        virtual void p2(const point&  p);
        virtual void x1(int val);
        virtual void y1(int val);
        virtual void x2(int val);
        virtual void y2(int val);
        virtual void width(int val);
        virtual void height(int val);
        virtual void position(int x, int y);
        virtual void position(const point& p);

        // utilities
        /// Returns true if the rectangle is empty(w==0 h==0)
        virtual bool empty() const { return (!width() && !height()); }
        /// Returns true if the given Point lies inside the rectangle
        virtual bool contains(int x, int y) const;
        /// Returns true if the given Point lies inside the rectangle
        virtual bool contains(const point& p) const;
        /// Returns true if the given rectangle intersects with this one
        virtual bool intersects(const rectangle& rect) const;
        /// Returns the area of intersection between both rectangles (the area they share)
        virtual rectangle intersection(const rectangle& rhs) const;
        /// Returns the upper left corner of the rectangle (this is duplicated by p1())
        virtual point position() const { return p1_; }

        // operators
        const rectangle operator+(const rectangle& rect) const;
        const rectangle& operator+=(const rectangle& rect);
        bool operator==(const rectangle& rect);
        bool operator!=(const rectangle& rect) { return !(*this == rect); }
    };
}
std::ostream& operator<<(std::ostream& os, const stk::rectangle& rect);
#endif
