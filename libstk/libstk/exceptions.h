/**************************************************************************************************
 *    FILENAME: exceptions.h
 * DESCRIPTION: Libstk exception classes.
 *     AUTHORS: Dirk Hoerner, Darren Hart, Marc Strämke
 *  START DATE: 09/Sep/2002  LAST UPDATE: 26/Jul/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_EXCEPTION_H
#define STK_EXCEPTION_H

#include <exception>
#include <stdexcept>
#include <string>

// FIXME: make some useful exception types (more specific: overlay_create_failed or something

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
        exception()
        { }
        virtual ~exception()
        { }
        virtual std::string what() const
        {
            return std::string("exception");
        }
    }; // class exception

    class error_message_exception : public exception
    {
    private:
        std::string message_;
        error_message_exception()
        { } // what is this for ?

    public:
        error_message_exception(const std::string& message)
        {
            message_ = message;
        }
        virtual ~error_message_exception()
        { }
        virtual std::string what() const
        {
            return message_;
        }
        void message(const std::string& message)
        {
            message_=message;
        }
    }; // class error_message_exception

} // namespace stk
#endif
