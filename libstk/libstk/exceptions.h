/***************************************************************************
     exceptions.h  -  declares all exceptions
        -------------------
    begin  : Mon Sep 09 2002
    copyright     : (C) 2002 by Dirk Hoerner
    email  : dirkhoerner@gmx.de
***************************************************************************/

#ifndef STK_EXCEPTION_H
#define STK_EXCEPTION_H

#include <exception>
#include <stdexcept>
#include <string>

namespace stk
{
	/* exception base class
	 * FIXME why not use the std exceptions as base classes ?
	 * 
	 */
	class exception
	{
		private:
		public:
			exception() { }
			virtual ~exception() { }
			virtual std::string what() const { return std::string("exception"); }
	}; // class exception

	class error_message_exception : public exception
	{
		private:
			std::string message_;
			error_message_exception() { } // what is this for ?
			
		public:
			error_message_exception(const std::string& message)
			{
				message_ = message;
			}
			virtual ~error_message_exception() { }
			virtual std::string what() const { return message_; }
			void message(const std::string& message) { message_=message; }
	}; // class error_message_exception
	
} // namespace stk
#endif
