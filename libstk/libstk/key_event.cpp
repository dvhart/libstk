/******************************************************************************
 *    FILENAME:  
 * DESCRIPTION: 
 *     AUTHORS: First Last, First Last, etc.
 *  START DATE: DD/MMM/YYYY  LAST UPDATE: DD/MMM/YYY
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
	{
	}

	key_event::~key_event()
	{
	}
	
} // namespace stk
