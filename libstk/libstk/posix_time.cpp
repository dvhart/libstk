/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: POSIX stk time functions implementation.
 *     AUTHORS: Darren Hart
 *  START DATE: 2003/May/20
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include <ctime>
#include <sys/time.h>

#include "libstk/time_value.h"
#include "libstk/logging.h"

namespace stk
{
    time_value get_current_time_value()
    {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        unsigned int secs = tv.tv_sec;
        unsigned int millis = tv.tv_usec/1000;
        return time_value(secs, millis);
    }
}

