#include "libstk/timed_event.h"
#include "libstk/exceptions.h"

#include <ctime>
#include <sys/time.h> // this is POSIX only

namespace stk
{
	timed_event::ptr timed_event::create(struct timeval tv, int interval)
	{
		timed_event::ptr new_timed_event(new timed_event(tv, interval));
		return new_timed_event;
	}

	timed_event::timed_event(struct timeval tv, int interval) 
		: tv_(tv), interval_(interval)
	{
	}

	timed_event::~timed_event()
	{
	}

	bool timed_event::ready()
	{
		struct timeval cur_tv;
		gettimeofday(&cur_tv, NULL);
		// FIXME: rethink how time is handled and stored	
		return (timercmp(&cur_tv, &tv_, >));
	}

	void timed_event::update()
	{
		if (interval_ >= 0)
		{
			long add_secs = interval_ / 1000;
			long add_usecs = interval_ % 1000;
			
			tv_.tv_usec += add_usecs;
			if (tv_.tv_usec > 1000000)
			{
				tv_.tv_usec -= 1000000;
				tv_.tv_sec++;
			}

			tv_.tv_sec += add_secs;
		}
		else
		{
			throw error_message_exception("timed_event::update() - attempt to update a non-recurring event");
		}
	}
}
