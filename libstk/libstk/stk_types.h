/*************************************************************************************************
 *    FILENAME: stk_types.h
 * DESCRIPTION: Libstk typedefs used throughout Libstk.
 *     AUTHORS: Darren Hart 
 *  START DATE: 03/Jun/2003  LAST UPDATE: 20/Jul/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_TYPES_H
#define STK_TYPES_H

// macros
#ifndef MIN
#define MIN(A,B) (((A)<(B))?(A):(B))
#endif

#ifndef MAX
#define MAX(A,B) (((A)>(B))?(A):(B))
#endif

namespace stk
{
    // FIXME
    // wrap with some #ifdefs to assure color is an unsigned 32 bit integer
    typedef unsigned int color;
    typedef unsigned char byte;
}

#endif
