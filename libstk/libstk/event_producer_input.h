/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Linux input event producer backend.
 *     AUTHORS: Darren Hart
 *  START DATE: 2004/Apr/21
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_EVENT_PRODUCER_INPUT_H
#define STK_EVENT_PRODUCER_INPUT_H

#include <linux/input.h>

#include <libstk/keycode.h>
#include <libstk/event_producer.h>
#include <libstk/event_system.h>
#include <libstk/event.h>
#include <libstk/key_event.h>

namespace stk
{
    class event_producer_input : public event_producer
    {
    private:
        modcode input2stk_mod(__u16 input_mod);
        key_event::ptr input2stk_key_event(const input_event& ev, stk::event::event_type type);
        int device_fd_;

    protected:
        event_producer_input(const std::string& device_path);

    public:
        static event_producer_input::ptr create(const std::string& device_path);
        virtual ~event_producer_input();
        virtual event::ptr poll_event();
    };
}

#endif
