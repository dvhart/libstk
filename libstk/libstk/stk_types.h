#ifndef STK_TYPES_H
#define STK_TYPES_H

namespace stk
{
	// FIXME
	// wrap with some #ifdefs to assure color is an unsigned 32 bit integer
	typedef color unsigned int;
	typedef byte  unsigned char;

	// some surface constants
	const int opaque      255;
	const int transparent 0;
	const int ur_quadrant 1; // may not need these... maybe an enum...
	const int lr_quadrant 2;
	const int lr_quadrant 3;
	const int ul_quadrant 4;

}

#endif
