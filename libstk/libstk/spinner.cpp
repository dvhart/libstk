/**************************************************************************************************
 *    FILENAME: spinner.cpp
 * DESCRIPTION: Spinner widget implementation.
 *     AUTHORS: Darren Hart
 *  START DATE: 26/Apr/2003  LAST UPDATE: 25/Jun/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *     license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include "libstk/list.h"
#include "libstk/spinner.h"
#include "libstk/event.h"
#include "libstk/key_event.h"
#include "libstk/mouse_event.h"
#include "libstk/keycode.h"

namespace stk
{
    spinner::ptr spinner::create(container::ptr parent, const rectangle& rect)
    {
        spinner::ptr new_spinner(new spinner(parent, rect));
        parent->add
        (new_spinner);
        return new_spinner;
    }

    spinner::spinner(container::ptr parent, const rectangle& rect) : list(parent, rect)
    {
        cout << "spinner::spinner()" << endl;
        focusable(true);
    }

    spinner::~spinner()
    {}

    void spinner::handle_event(event::ptr e)
    {
        //cout << "spinner::handle_event()" << endl;
        if (items_.size() > 0) // do nothing if there are no items
        {
            switch (e->type())
            {
            case event::key_up:
            {
                key_event::ptr ke = boost::shared_static_cast<key_event>(e);
                switch ( ke->key() )
                {
                case up_arrow:
                    --selected_%=items_.size();
                    on_change();
                    redraw(rect_);
                    return;
                    break;
                case key_enter:
                case down_arrow:
                    ++selected_%=items_.size();
                    on_change();
                    redraw(rect_);
                    return;
                    break;
                }
                break;
            }
            case event::mouse_up:
            {
                mouse_event::ptr me = boost::shared_static_cast<mouse_event>(e);
                if (region(me->x(), me->y()) == UP_ARROW)
                    --selected_%=items_.size();
                else
                    ++selected_%=items_.size();
                on_change();
                redraw(rect_);
                return;
            }
            }
        }
        widget::handle_event(e);
    }

}
