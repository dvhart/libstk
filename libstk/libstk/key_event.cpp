/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: key_event class implementation.
 *     AUTHORS: Darren Hart, Marc Straemke 
 *  START DATE: 2003/Mar/03
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include "libstk/event.h"
#include "libstk/key_event.h"

namespace stk
{
    key_event::key_event(keycode fn_key, event_type type, modcode modlist, wchar_t character) :
            event(type), fn_key_(fn_key), modlist_(modlist), character_(character)
    {
    }

    key_event::~key_event()
    {
    }

} // namespace stk
