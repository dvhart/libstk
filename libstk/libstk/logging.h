/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Utilities for Logging
 *     AUTHORS: Marc Strämke, Darren Hart
 *  START DATE: 2003/JUN/28
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_LOGGING_H
#define STK_LOGGING_H

#include <libstk/config.h>

#ifdef HAVE_LOGGING
#include <sstream>
#include <boost/smart_ptr.hpp>
#include <list>

namespace stk
{
    enum EN_LOG_LEVEL
    {
        LL_Info,
        LL_Warning,
        LL_Error
    };

    class logger;
    class logger
    {
    private:
        struct target_info
        {
            std::ostream* outstream;
            int min_level;      // Everything Greater or equal to this gets logged

            bool operator==(target_info &rhs)
            {
                return rhs.outstream == outstream;
            }
            bool operator==(std::ostream *rhs)
            {
                return rhs == outstream;
            }

        };

        static boost::shared_ptr<logger> instance_;
        typedef std::list<target_info> Ttargets;
        Ttargets targets;
    public:
        static boost::shared_ptr<logger> get();
        logger();
        ~logger();
        void add_target(std::ostream* target, int min_level);
        void remove_target(std::ostream* target);
        void log(const std::string& filename, int line, const std::string& message, int severity);
    };

}

#define INFO(msg) { \
    std::ostringstream stream; \
    stream << msg ; \
    stk::logger::get()->log(__FILE__, __LINE__, stream.str(), stk::LL_Info);\
}

#define WARN(msg) { \
    std::ostringstream stream; \
    stream << msg ; \
    stk::logger::get()->log(__FILE__, __LINE__, stream.str(), stk::LL_Warning);\
}
#define ERROR(msg) { \
    std::ostringstream stream; \
    stream << msg ; \
    stk::logger::get()->log(__FILE__, __LINE__, stream.str(), stk::LL_Error);\
}
#else
#define INFO(msg)
#define WARN(msg)
#define ERROR(msg)
#endif

#endif
