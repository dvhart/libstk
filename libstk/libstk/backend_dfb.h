/**************************************************************************************************
 *    FILENAME: backend_dfb.h 
 * DESCRIPTION: Used to hold a singleton which manages the IDirectfb Instance shared by all
 *              DFB users
 *     AUTHORS: Marc Strämke
 *  START DATE: 28/JUN/2003  LAST UPDATE: 28/JUN/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/


#ifndef BACKEND_DFB_H
#define BACKEND_DFB_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <directfb.h>

namespace stk
{
    
    class backend_dfb;
    
    class backend_dfb
    {
    public:
        typedef boost::shared_ptr<backend_dfb> ptr;
        typedef boost::weak_ptr<backend_dfb> weak_ptr;
    protected:
        static boost::weak_ptr<backend_dfb> instance_;
        IDirectFB* primary_pointer;
    public:
        DFBSurfacePixelFormat format;
        static boost::shared_ptr<backend_dfb> get();
        backend_dfb();
        ~backend_dfb();
        IDirectFB* get_interface();
    };
}

#endif
