/***************************************************************************
     exceptions.h  -  declares all exceptions
        -------------------
    begin  : Mon Sep 09 2002
    copyright     : (C) 2002 by Dirk Hoerner
    email  : dirkhoerner@gmx.de
***************************************************************************/

#ifndef STK_EXCEPTION_H
#define STK_EXCEPTION_H

#include <string>

namespace stk
{
	/* exception base class
	 * 
	 */
	class exception
	{
		private:
		public:
			exception() { }
			virtual ~exception() { }
			virtual std::string to_string()
			{
				return std::string("exception");
			}
	}; // class exception

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
			virtual ~error_message_exception()
			{}

			std::string message()
			{
				return message_;
			}
			void message(std::string message)
			{
				message_=message;
			}

			virtual std::string to_string()
			{
				return std::string();
			}
	}; // class error_message_exception
	
} // namespace stk
#endif
