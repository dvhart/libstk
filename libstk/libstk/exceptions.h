/***************************************************************************
			  exceptions.h  -  declares all exceptions
			     -------------------
    begin		: Mon Sep 09 2002
    copyright	    : (C) 2002 by Dirk Hoerner
    email		: dirkhoerner@gmx.de
***************************************************************************/

/***************************************************************************
 *									 *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.				   *
 *									 *
 ***************************************************************************/

#ifndef _STK_EXCEPTION_H_
#define _STK_EXCEPTION_H_

#include <string>

namespace stk
{
	class exception	{};

	class error_exception : public exception
	{
	private:
		std::string _error;

		exception() {}
	public:

		exception(const std::string) { _error = error; }
		~exception() {}

		std::string get_error() { return _error; }
	};
} // namespace stk
#endif
