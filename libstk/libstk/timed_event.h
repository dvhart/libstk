#ifndef STK_TIMED_EVENT
#define STK_TIMED_EVENT

#include <ctime>
#include <sys/time.h>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/signal.hpp>
#include "libstk/combiners.h"

namespace stk
{
	class timed_event
	{
		public:
			typedef boost::shared_ptr<timed_event> ptr;
			typedef boost::weak_ptr<timed_event> weak_ptr;

		private:
			timed_event(struct timeval tv, int interval);
			struct timeval tv_;
			int interval_;
			
		protected:

		public:
			static timed_event::ptr create(struct timeval tv, int interval);
			~timed_event();
			bool ready();
			void update();
			struct timeval timeval() const { return tv_; }
			void timeval(struct timeval tv) { tv_ = tv; }
			int interval() const { return interval_; }
			void interval(int val) { interval_ = val; }
			boost::signal<bool (), combiner::logical_and<bool> > on_timer;
	};
}

#endif
