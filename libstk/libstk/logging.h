#ifndef LOGGING_H
#define LOGGING_H

/*    FILENAME: logging.h
 * DESCRIPTION: Utilities for Logging
 *     AUTHORS: Marc Strämke
 *  START DATE: 28/JUN/2003  LAST UPDATE: 28/JUN/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

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
        void add_target(std::ostream* target,int min_level);
        void remove_target(std::ostream* target);
        void log(std::string FILE, int  LINE, std::string message,int severity);
    };
    
}

#define INFO(msg) { std::ostringstream stream; \
                    stream << msg ; \
                    stk::logger::get()->log(__FILE__,__LINE__,stream.str(),stk::LL_Info);\
                    }

#define WARN(msg) { std::ostringstream stream; \
                    stream << msg ; \
                    stk::logger::get()->log(__FILE__,__LINE__,stream.str(),stk::LL_Warning);\
                    }
#define ERROR(msg) { std::ostringstream stream; \
                    stream << msg ; \
                    stk::logger::get()->log(__FILE__,__LINE__,stream.str(),stk::LL_Error);\
                    }
#else
#define INFO(msg)
#define WARN(msg)
#define ERROR(msg)
#endif

#endif
