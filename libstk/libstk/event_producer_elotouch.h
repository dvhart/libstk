/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: EloTouch event producer backend.
 *     AUTHORS: Marc Strämke, Darren Hart
 *  START DATE: 2003/Jun/28
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 **************************************************************************************************/

#ifndef STK_EVENT_PRODUCER_ELOTOUCH_H
#define STK_EVENT_PRODUCER_ELOTOUCH_H

#include <libstk/event_producer.h>
#include <libstk/event_system.h>
#include <string>
#include <queue>

namespace stk
{
    class event_producer_elotouch : public event_producer
    {
    public:
        typedef boost::shared_ptr<event_producer_elotouch> ptr;
    private:
        int fd;
        std::queue<unsigned char> buffer;
        bool just_untouched;
    protected:
        event_producer_elotouch(std::string devicename);

    public:
        static event_producer_elotouch::ptr create(std::string devicename);
        virtual ~event_producer_elotouch();
        virtual event::ptr poll_event();
    };
}

#endif
