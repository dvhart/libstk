#ifndef STK_H
#define STK_H

/* This file contains macros and typedefs used throughout the stk codebase.
 * (Eventually this should contain all the files needed by the user, so
 * they can simply "#include <stk.h>"
 */

// macros
#ifndef MIN
#define MIN(A,B) (((A)<(B))?(A):(B))
#endif

namespace stk
{
	// FIXME
	// wrap with some #ifdefs to assure color is an unsigned 32 bit integer
	typedef unsigned int color;
	typedef unsigned char byte;
}


#endif
