/******************************************************************************
 *    FILENAME: posix_time.cpp
 * DESCRIPTION: POSIX stk time functions implementation.
 *     AUTHORS: Darren Hart
 *  START DATE: 20/May/2003  LAST UPDATE: 20/May/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available
 *              with the source as license.txt or at 
 *              http://www.libstk.org/index.php?page=docs/license
 *****************************************************************************/

#include <ctime>
#include <sys/time.h>

#include "libstk/time_value.h"

namespace stk
{
    time_value get_current_time_value()
    {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        return time_value(tv.tv_sec, tv.tv_usec/1000);
    }
}

