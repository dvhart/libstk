/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Rectangle class implementation.
 *     AUTHORS: Darren Hart, Marc Strämke
 *  START DATE: 2003/Mar/22
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include <iostream>
#include "libstk/rectangle.h"

namespace stk
{
    rectangle::rectangle(int x1, int y1, int w, int h) :
            p1_(point(x1, y1)), p2_(x1 + w - 1, y1 + h - 1)
    {
        // enforce p1 as upper left coordinate
        // (this assume 0,0 is the upper left corner of the screen (like SDL))
        if ((x1 > x1+w) || (y1 > y1+h))
            throw error_message_exception("x1,y1 must be less than x2,y2");
    }

    rectangle::rectangle(const point &p1, const point &p2) : p1_(p1), p2_(p2)
    {
        // enforce p1 as upper left coordinate
        // (this assume 0,0 is the upper left corner of the screen (like SDL))
        if ((p1.x() > p2.x()) || (p1.y() > p2.y()))
            throw error_message_exception(
                "p1.x(),p1.y() must be less than p2.x(),p2.y()");
    }

    // utilities
    bool rectangle::contains(int x, int y) const
    {
        return (x >= p1_.x() && x <= p2_.x() && y >= p1_.y() && y <= p2_.y());
    }

    bool rectangle::contains(const point& p) const
    {
        return (p.x(), p.y());
    }

    bool rectangle::intersects(const rectangle& rect) const
    {
        bool x_in = ((rect.x1() > x1() && rect.x1() < x2()) ||
                     (rect.x2() > x1() && rect.x2() < x2()));
        bool x_span = (rect.x1() <= x1() && rect.x2() >= x2());
        bool y_in = ((rect.y1() > y1() && rect.y1() < y2()) ||
                     (rect.y2() > y1() && rect.y2() < y2()));
        bool y_span (rect.y1() <= y1() && rect.y2() >= y2());
        return ((x_span || x_in) && (y_span || y_in));
    }
 
    void rectangle::position(int x, int y)
    {
        int old_width = width();
        int old_height = height();

        p1_.x(x);
        p1_.y(y);
        width(old_width);
        height(old_height);
    }

    void rectangle::position(const point& p)
    {
        position(p.x(), p.y());
    }

    // operators
    const rectangle rectangle::operator+(const rectangle& rect) const
    {
        if (empty())
            return rect;

        rectangle new_rect(MIN(x1(), rect.x1()), MIN(y1(), rect.y1()),
                           MAX(x2(), rect.x2()), MAX(y2(), rect.y2()));
        return new_rect;
    }

    const rectangle& rectangle::operator+=(const rectangle& rect)
    {
        if (!empty())
        {
            x1(MIN(x1(), rect.x1()));
            y1(MIN(y1(), rect.y1()));
            x2(MAX(x2(), rect.x2()));
            y2(MAX(y2(), rect.y2()));
        }
        else
        {
            x1(rect.x1());
            y1(rect.y1());
            x2(rect.x2());
            y2(rect.y2());
        }
        return *this;
    }
    rectangle rectangle::intersection(rectangle &rhs) const
    {
        rectangle retval;
        if(!intersects(rhs))
        {
            retval.x1(x1());
            retval.y1(y1());
        }
        else
        {
            retval.x1(MAX(x1(),rhs.x1()));
            retval.x2(MIN(x2(),rhs.x2()));
            retval.y1(MAX(y1(),rhs.y1()));
            retval.y2(MIN(y2(),rhs.y2()));
        }
        return retval;
    }

}

std::ostream& operator<<(std::ostream& os, const stk::rectangle& rect)
{
    os << rect.x1() << "," << rect.y1() << " " << rect.x2() << "," << rect.y2()
    << " w:" << rect.width() << ", h:" << rect.height();
    return os;
}
