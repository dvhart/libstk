/******************************************************************************
 *    FILENAME: timer.h
 * DESCRIPTION: A timer class that emits its signal after a specified interval,
 *              with the option to repeat.
 *     AUTHORS: Darren Hart 
 *  START DATE: 13/May/2003  LAST UPDATE: 13/May/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available
 *              with the source as license.txt or at 
 *              http://www.libstk.org/index.php?page=docs/license
 *****************************************************************************/

#ifndef STK_TIMER
#define STK_TIMER

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/signal.hpp>
#include <libstk/combiners.h>
#include <libstk/time_value.h>

namespace stk
{
    // The timer class's interval is in milliseconds, it is decremented by
    // time elapsed since the last update() call each time update() is called.
    // It contains a signal that is called in update() if the counter drops
    // to or below zero. Update returns false when the signal's combiner does
    // or after the signal is executed and repeat is false, this indicates to
    // application that this time can be removed from the list of timers.
    class timer
    {
    public:
        typedef boost::shared_ptr<timer> ptr;
        typedef boost::weak_ptr<timer> weak_ptr;

    private:
        timer(int interval, bool repeat);
        /// Interval between ready states
        int interval_;
        /// Whether or not to reset the counter after a ready state
        bool repeat_;
        int counter_;
        /// The last time it was updated
        time_value last_tv_;

    protected:

    public:
        static timer::ptr create(int interval, bool repeat);
        ~timer();
        bool update();
        int interval() const
        {
            return interval_;
        }
        void interval(int val)
        {
            interval_ = val;
            counter_ = val;
        }
        boost::signal<bool (), combiner::logical_or<bool> > on_timer;
    };
}

#endif
