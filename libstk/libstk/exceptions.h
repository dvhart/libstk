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
class exception
    {}
;

class error_message_exception : public exception
{
private:
    std::string message_;

    error_message_exception()
    {}
public:
    error_message_exception(const std::string &message)
    {
        message_ = message;
    }
    ~error_message_exception()
    {}

    std::string message()
    {
        return message_;
    }
    void message(std::string message)
    {
        message_=message;
    }
};
} // namespace stk
#endif
