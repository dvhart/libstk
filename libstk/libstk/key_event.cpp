/******************************************************************************
 *    FILENAME: key_event.cpp
 * DESCRIPTION: key_event class implementation.
 *     AUTHORS: Darren Hart, Marc Straemke 
 *  START DATE: 03/Mar/2003  LAST UPDATE: 14/May/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available
 *              with the source as license.txt or at 
 *              http://www.libstk.org/index.php?page=docs/license
 *****************************************************************************/

#include "libstk/event.h"
#include "libstk/key_event.h"

namespace stk
{
    key_event::key_event(keycode key, event_type type) :
            event(type), keycode_(key)
    {}

    key_event::~key_event()
    {}

} // namespace stk
