/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Timer class implementation.
 *     AUTHORS: Darren Hart
 *  START DATE: 2003/May/13
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include "libstk/timer.h"
#include "libstk/exceptions.h"
#include "libstk/time_value.h"
#include "libstk/logging.h"

/// FIXME: make this configured at compile time
#include "libstk/posix_time.h"

namespace stk
{
    timer::ptr timer::create(int interval, bool repeat)
    {
        timer::ptr new_timer(new timer(interval, repeat));
        return new_timer;
    }

    timer::timer(int interval, bool repeat)
            : interval_(interval), repeat_(repeat), counter_(interval)
    {
        last_tv_ = get_current_time_value();
    }

    timer::~timer()
    {}

    bool timer::update()
    {
        // decrement counter_ by elapsed millis since last time update() was called
        time_value cur_tv = get_current_time_value();
        time_value diff_tv = cur_tv - last_tv_;
        last_tv_ = cur_tv;

        int millis = 1000*diff_tv.seconds() + diff_tv.millis();
        counter_ -= millis;

        if (counter_ <= 0)
        {
            if (repeat_)
                counter_ = interval_;
            return (on_timer() && repeat_);
        }

        return true;
    }

}
