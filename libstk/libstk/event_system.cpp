/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Event system implementation 
 *     AUTHORS: Darren Hart, Marc Strämke
 *  START DATE: 2003/Jun/09
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/thread/mutex.hpp> // make this inclusion configurable (#ifdef THREADED)
#include <libstk/event.h>
#include <libstk/event_system.h>
#include <libstk/event_producer.h>
#include <libstk/logging.h>

namespace stk
{
    event_system::ptr event_system::instance_;

    event_system::ptr event_system::get()
    {
        if (!instance_) instance_.reset(new event_system());
        return instance_;
    }

    event_system::event_system()
    {
        INFO("constructor");
    }

    event_system::~event_system()
    {
        INFO("destructor");
    }

    void event_system::add_producer(event_producer::ptr producer)
    {
        event_producers_.push_back(producer);
    }

    event::ptr event_system::poll_event()
    {
        // lock the queue FIXME: make this configuarable
        boost::mutex::scoped_lock scoped_lock(queue_mutex);

        // add any pending events to the queue
        event::ptr e = event::create(event::none);
        std::list<event_producer::ptr>::iterator iter = event_producers_.begin();
        for (iter; iter != event_producers_.end(); iter++)
        {
            e = (*iter)->poll_event();
            while (e->type() != event::none)
            {
                events_.push(e);
                e = (*iter)->poll_event();
            }
        }

        // return and pop the first item on the queue
        if (!events_.empty())
        {
            e = events_.front();
            events_.pop();
        }
        else
            e = event::create(event::none);
        return e;
    }

    void event_system::push_event(event::ptr e)
    {
        // lock the queue FIXME: make this configuarable
        boost::mutex::scoped_lock scoped_lock(queue_mutex);
        // post the event to the queue
        events_.push(e);
    }

}

