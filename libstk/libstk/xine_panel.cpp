/**************************************************************************************************
 *    FILENAME: xine_panel.cpp
 * DESCRIPTION: Xine media panel implementation
 *     AUTHORS: Darren Hart
 *  START DATE: 21/Jul/2003  LAST UPDATE: 21/Jul/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include <iostream>
#include "libstk/xine_panel.h"

namespace stk
{
    xine_panel::xine_panel(container::ptr parent, const rectangle& rect) :
        widget(parent, rect)
    {
    }
    
    xine_panel::ptr xine_panel::create(container::ptr parent, const rectangle& rect)
    {
        xine_panel::ptr new_xine_panel(new xine_panel(parent, rect));
        return new_xine_panel;
    }
    
    xine_panel::~xine_panel()
    {
    }
} 


