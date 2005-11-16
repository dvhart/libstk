/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Abstract class defining the event handler interface to Libstk
 *              objects like widget and list_item.
 *     AUTHORS: Darren Hart, Marc Str�mke
 *  START DATE: 2003/Feb/22
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Str�mke, Dirk H�rner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_EVENT_HANDLER_H
#define STK_EVENT_HANDLER_H

#include <libstk/event.h>

namespace stk
{
    /// An abstract class declaring the event_handler interface.
    class event_handler
    {
    private:

    protected:

    public:
        event_handler()
        { }
        ;
        virtual ~event_handler()
        { }
        ;

        /// Receive an event and do something with it.
        /// \param e Event to be handled
        virtual void handle_event(event::ptr e) = 0;

    }; // class event_handler
} // namespace stk

#endif
