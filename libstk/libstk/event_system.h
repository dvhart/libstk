/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Event system singleton, primary access point for application to 
 *              get events from all event producers
 *     AUTHORS: Darren Hart, Marc Strämke
 *  START DATE: 2003/Feb/22
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_EVENT_SYSTEM_H
#define STK_EVENT_SYSTEM_H

#include <list>
#include <queue>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/thread/mutex.hpp> // make this inclusion configurable (#ifdef THREADED)
#include <libstk/event.h>

namespace stk
{
    class event_producer;

    class event_system
    {
    public:
        typedef boost::shared_ptr<event_system> ptr;
        typedef boost::weak_ptr<event_system> weak_ptr;

    private:
        event_system();
        static event_system::ptr instance_;
        boost::mutex queue_mutex; // FIXME: make this configurable
        std::queue<event::ptr> events_;
        std::list<boost::shared_ptr<event_producer> > event_producers_;

    public:
        static event_system::ptr get
            ();
        ~event_system();
        void add_producer(boost::shared_ptr<event_producer> producer);
        event::ptr poll_event();
        void push_event(event::ptr e);
    };

}

#endif
