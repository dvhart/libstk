/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Mouse event for button clicks and mouse motion. 
 *     AUTHORS: Darren Hart, Marc Str�mke
 *  START DATE: 2003/Mar/03
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Str�mke, Dirk H�rner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_MOUSE_EVENT_H
#define STK_MOUSE_EVENT_H

#include <boost/shared_ptr.hpp>
#include <libstk/event.h>

namespace stk
{
    class mouse_event : public event
    {
    public:
        typedef boost::shared_ptr<mouse_event> ptr;
        typedef boost::weak_ptr<mouse_event> weak_ptr;
    private:

    protected:
        int x_, y_, button_;

    public:
        mouse_event(int x=-1, int y=-1, int button=-1, event_type type=mouse_up);
        virtual ~mouse_event();
        int x()
        {
            return x_;
        }
        int y()
        {
            return y_;
        }
        int button()
        {
            return button_;
        }

    }
    ; //class mouse_event


} // namespace stk

#endif
