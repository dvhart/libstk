/******************************************************************************
 *    FILENAME: posix_time.h
 * DESCRIPTION: POSIX stk time functions.
 *     AUTHORS: Darren Hart
 *  START DATE: 20/May/2003  LAST UPDATE: 20/May/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available
 *              with the source as license.txt or at 
 *              http://www.libstk.org/index.php?page=docs/license
 *****************************************************************************/

#ifndef STK_POSIX_TIME
#define STK_PODIX_TIME

#include <ctime>
#include <sys/time.h>

namespace stk
{
	class time_value;
	time_value get_current_time_value();
}

#endif
