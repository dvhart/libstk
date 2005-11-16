/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Directfb event producer backend.
 *     AUTHORS: Marc Strämke
 *  START DATE: 2004/Aug/21
 *
 *   COPYRIGHT: 2004 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef EVENT_PRODUCER_DFB_H
#define EVENT_PRODUCER_DFB_H

#include <libstk/keycode.h>
#include <libstk/event_producer.h>
#include <libstk/event_system.h>
#include <libstk/event.h>
#include <libstk/key_event.h>
#include <libstk/backend_dfb.h>

namespace stk
{
    class event_producer_dfb : public event_producer
    {
    public:
        typedef boost::shared_ptr<event_producer_dfb> ptr;
        typedef boost::weak_ptr<event_producer_dfb> weak_ptr;
    private:

    protected:
        IDirectFBEventBuffer *event_buffer;
        event_producer_dfb();
        backend_dfb::ptr backend_handle;
    public:
        static event_producer_dfb::ptr create();
        virtual ~event_producer_dfb();
        virtual event::ptr poll_event();
    };
}

#endif
