/**************************************************************************************************
 *    FILENAME: event_producer.h
 * DESCRIPTION: Abstract base class for all event_producer backends (SDL, etc.)
 *     AUTHORS: Darren Hart
 *  START DATE: 09/Jun/2003  LAST UPDATE: 26/Jul/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_EVENT_PRODUCER_H
#define STK_EVENT_PRODUCER_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <libstk/event.h>
#include <libstk/event_system.h>

namespace stk
{
    class event_producer
    {
    public:
        typedef boost::shared_ptr<event_producer> ptr;
        typedef boost::weak_ptr<event_producer> weak_ptr;

    private:

    protected:
        event_system::ptr event_system_;
        event_producer()
        { }

    public:
        virtual ~event_producer()
        { }
        virtual boost::shared_ptr<stk::event> poll_event() = 0;
    };
}

#endif
