/******************************************************************************
 *    FILENAME: timer.cpp
 * DESCRIPTION: Timer class implementation.
 *     AUTHORS: Darren Hart
 *  START DATE: 13/May/2003  LAST UPDATE: 14/May/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available
 *              with the source as license.txt or at 
 *              http://www.libstk.org/index.php?page=docs/license
 *****************************************************************************/

#include "libstk/timer.h"
#include "libstk/exceptions.h"
#include "libstk/time_value.h"

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
		: interval_(interval), counter_(interval), repeat_(repeat)
	{
		last_tv_ = get_current_time_value();
	}

	timer::~timer()
	{
	}

	bool timer::update()
	{
		// decrement counter_ by elapsed millis since last time ready() was called
		time_value cur_tv = get_current_time_value();
		counter_ -= (cur_tv - last_tv_).millis();
		last_tv_ = cur_tv;
		
		if (counter_ <= 0)
		{
			if (repeat_) counter_ = interval_;
			return (on_timer() && repeat_);
		}
		
		return true;
	}

}
