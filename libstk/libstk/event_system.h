/******************************************************************************
 *    FILENAME: event_system.h
 * DESCRIPTION: Abstract base class for all backend event systems (i.e. SDL).
 *     AUTHORS: Darren Hart, Marc Straemke
 *  START DATE: 22/Feb/2003  LAST UPDATE: 13/May/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available
 *              with the source as license.txt or at 
 *              http://www.libstk.org/index.php?page=docs/license
 *****************************************************************************/

#ifndef STK_EVENT_SYSTEM_H
#define STK_EVENT_SYSTEM_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "libstk/event.h"

namespace stk
{
	class event_system
	{
		public:
			typedef boost::shared_ptr<event_system> ptr;
			typedef boost::weak_ptr<event_system> weak_ptr;
		private:
		
		protected:
			event_system() { };
			
		public:
			virtual ~event_system() { };
			virtual event::ptr poll_event() = 0;
	};

}

#endif
