/******************************************************************************
 *    FILENAME:  
 * DESCRIPTION: 
 *     AUTHORS: First Last, First Last, etc.
 *  START DATE: DD/MMM/YYYY  LAST UPDATE: DD/MMM/YYY
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available
 *              with the source as license.txt or at 
 *              http://www.libstk.org/index.php?page=docs/license
 *****************************************************************************/

#include "libstk/timer.h"
#include "libstk/exceptions.h"

#include <ctime>
#include <sys/time.h> // this is POSIX only

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
		gettimeofday(&last_tv_, NULL);
	}

	timer::~timer()
	{
	}

	bool timer::update()
	{
		struct timeval cur_tv;
		gettimeofday(&cur_tv, NULL);

		// WRITEME
		// decrement counter_ by elapsed millis since last time ready() was called
		int millis = 0;
		millis += cur_tv.tv_sec*1000;
		millis += cur_tv.tv_usec/1000;
		millis -= last_tv_.tv_sec*1000;
		millis -= last_tv_.tv_usec/1000;
		counter_ -= millis;
		last_tv_ = cur_tv;
		
		if (counter_ <= 0)
		{
			if (repeat_) counter_ = interval_;
			return (on_timer() && repeat_);
		}
		
		return true;
	}

}
