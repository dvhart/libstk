/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Mouse_event implementation.
 *     AUTHORS: Darren Hart, Marc Strämke 
 *  START DATE: 2003/Mar/03
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include <iostream>
#include "libstk/event.h"
#include "libstk/mouse_event.h"

namespace stk
{
    mouse_event::mouse_event(int x, int y, int button, event_type type) :
            event(type), x_(x), y_(y), button_(button)
    {
        /*
        INFO("mouse_event::mouse_event()");
        INFO("\ttype:   " << type);
        INFO("\tx:      " << x_);
        INFO("\ty:      " << y_);
        INFO("\tbutton: " << button);
        */
    }

    mouse_event::~mouse_event()
    {}

} // namespace stk
