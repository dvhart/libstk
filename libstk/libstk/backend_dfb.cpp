/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Used to hold a singleton which manages the IDirectfb Instance shared by all
 *              DFB users
 *     AUTHORS: Marc Strämke
 *  START DATE: 2003/JUN/28
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include <boost/smart_ptr.hpp>
#include "libstk/backend_dfb.h"
#include "libstk/logging.h"

#include <directfb.h>
#include <iostream>

namespace stk
{
    boost::weak_ptr<backend_dfb> backend_dfb::instance_;
    
    boost::shared_ptr<backend_dfb> backend_dfb::get()
    {
        backend_dfb::ptr instance = instance_.lock();        
        if(instance)
            return instance;
        instance.reset(new backend_dfb());
        instance_ = instance;
        return instance;
    }
    
    backend_dfb::backend_dfb()
    {
        INFO("Backend DFB constructed");
        DirectFBInit(0,NULL);
        DirectFBCreate(&primary_pointer);
    }
    backend_dfb::~backend_dfb()
    {
        INFO("Backend DFB destructed");
        primary_pointer->Release(primary_pointer);
    }
    
    IDirectFB* backend_dfb::get_interface()
    {
        return primary_pointer;
    }
    
}
