/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: An abstract base class providing vertical and horizontal scroll models to widgets
 *              that need to be aware of their scrolling properties, such as text_are and viewport.
 *     AUTHORS: Darren Hart 
 *  START DATE: 2003/Oct/10
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include "libstk/scrollable.h"

namespace stk
{
    scrollable::scrollable()
    {
        INFO("constructor");
    }

    scrollable::~scrollable()
    {
        INFO("destructor");
    }
    
    void scrollable::h_scroll(scroll_model::ptr model)
    {
        h_scroll_con_.disconnect();
        h_scroll_ = model;
    }

    void scrollable::v_scroll(scroll_model::ptr model)
    {
        v_scroll_con_.disconnect();
        v_scroll_ = model;
    }

} // namespace stk
