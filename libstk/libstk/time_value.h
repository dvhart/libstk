/**************************************************************************************************
 *    FILENAME: time_value.h
 * DESCRIPTION: A time container.
 *     AUTHORS: Darren Hart
 *  START DATE: 20/May/2003  LAST UPDATE: 26/Jul/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_TIME_VALUE_H
#define STK_TIME_VALUE_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

/// FIXME: make this configured at compile time
#include <libstk/posix_time.h>

namespace stk
{
    class time_value
    {
    private:
        int seconds_;
        int millis_;

    public:
        time_value()
        {
            time_value tv = get_current_time_value();
            seconds_ = tv.seconds();
            millis_ = tv.millis();
        }
        time_value(int seconds, int millis)
                : seconds_(seconds), millis_(millis)
        { }
        ~time_value()
        { }

        int millis() const
        {
            return millis_;
        }
        int seconds() const
        {
            return seconds_;
        }

        time_value operator-(const time_value& rhs)
        {
            int carry = 0;
            int t_millis = millis_ - rhs.millis();
            if (t_millis < 0)
            {
                t_millis += 1000;
                carry = 1;
            }
            return time_value(seconds_ - rhs.seconds() - carry, t_millis);
        }

        time_value operator+(const time_value& rhs)
        {
            int carry = 0;
            int t_millis = millis_ + rhs.millis();
            if (t_millis > 1000)
            {
                carry = 1;
                t_millis -= 1000;
            }
            return time_value(seconds_ + rhs.seconds() + carry, t_millis);
        }
    };
}

#endif

