/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Utilities for Logging
 *     AUTHORS: Marc Strämke, Darren Hart
 *  START DATE: 2003/Jun/28
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include <iostream>
#include <vector>
#include "logging.h"

namespace stk
{
    boost::shared_ptr<logger> logger::instance_;
    
    boost::shared_ptr<logger> logger::get()
    {
        if(!instance_)
            instance_.reset(new logger());
        return instance_;
    }
    
    logger::logger()
    {
        severity_names_.resize(LL_LENGTH);
        severity_names_[LL_Info] = "Info";
        severity_names_[LL_Warning] = "Warning";
        severity_names_[LL_Error] = "Error";
        severity_names_[LL_None] = "None";
    }

    logger::~logger()
    {
        log(__FILE__, __LINE__, "destructor", LL_Info);
    }
    
    void logger::add_target(std::ostream* target, log_level min_level)
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
            log_level severity)
    {
        for (Ttargets::iterator iter=targets.begin();iter!=targets.end();iter++)
        {
            if(severity >= iter->min_level)
                *iter->outstream << severity_names_[severity] << "! " << filename << ":" << line 
                                 << " \t" << message << std::endl;
        }
        
    }
    
    
}
