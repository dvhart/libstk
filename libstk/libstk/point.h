/******************************************************************************
 *    FILENAME: point.h
 * DESCRIPTION: A simple point class with += and -= defined.
 *     AUTHORS: Darren Hart, Marc Straemke
 *  START DATE: 10/Dec/2003  LAST UPDATE: 20/May/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available
 *              with the source as license.txt or at 
 *              http://www.libstk.org/index.php?page=docs/license
 *****************************************************************************/

#ifndef STK_POINT_H
#define STK_POINT_H

#include <boost/operators.hpp>

namespace stk
{
    class point : boost::addable<point>, boost::subtractable<point>
    {
    private:
        int x_, y_;

    public:
        point(int x = 0, int y = 0) : x_(x), y_(y)
        { }
        int x() const
        {
            return x_;
        }
        int y() const
        {
            return y_;
        }
        void x(int x)
        {
            x_ = x;
        }
        void y(int y)
        {
            y_ = y;
        }

        point& operator+=(const point& rhs)
        {
            x_ += rhs.x_;
            y_ += rhs.y_;
            return *this;
        }

        point& operator-=(const point& rhs)
        {
            x_ -= rhs.x_;
            y_ -= rhs.y_;
            return *this;
        }

    };
}

#endif
