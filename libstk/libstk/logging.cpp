/*    FILENAME: logging.h
 * DESCRIPTION: Utilities for Logging
 *     AUTHORS: Marc Strämke, Darren Hart
 *  START DATE: 28/JUN/2003  LAST UPDATE: 26/JUL/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include "logging.h"
#include <vector>

namespace stk
{
    boost::shared_ptr<logger> logger::instance_;
    
    boost::shared_ptr<logger> logger::get()
    {
        if(!instance_)
            instance_.reset(new logger());
        return instance_;
    }
    
    void logger::add_target(std::ostream* target, int min_level)
    {
        target_info temp;
        temp.outstream = target;
        temp.min_level = min_level;
        targets.push_back(temp);
    }
    
    void logger::remove_target(std::ostream* target)
    {
        Ttargets::iterator iter = std::find(targets.begin(), targets.end(), target);
        targets.erase(iter);
    }
    
    void logger::log(const std::string& filename, int line, const std::string& message, 
            int severity)
    {
        std::vector<std::string> severity_names;
        severity_names.push_back("Info");
        severity_names.push_back("Warning");
        severity_names.push_back("Error");

        for(Ttargets::iterator iter=targets.begin();iter!=targets.end();iter++)
        {
            if(severity >= iter->min_level)
                *iter->outstream << severity_names[severity] << "! " << filename << ":" << line 
                                 << " \t" << message << std::endl;
        }
        
    }
    
    
}
