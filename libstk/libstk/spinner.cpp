/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Spinner widget implementation.
 *     AUTHORS: Darren Hart
 *  START DATE: 2003/Apr/26
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include "libstk/list.h"
#include "libstk/spinner.h"
#include "libstk/event.h"
#include "libstk/key_event.h"
#include "libstk/mouse_event.h"
#include "libstk/keycode.h"

namespace stk
{
    spinner::ptr spinner::create(container::ptr parent, const rectangle& rect, bool wrap)
    {
        spinner::ptr new_spinner(new spinner(parent, rect, wrap));
        parent->add(new_spinner);
        return new_spinner;
    }

    spinner::spinner(container::ptr parent, const rectangle& rect, bool wrap) : 
        list(parent, rect), wrap_(wrap)
    {
        INFO("constructor");
        focusable(true);
    }

    spinner::~spinner()
    {
        INFO("destructor");
    }

    void spinner::handle_event(event::ptr e)
    {
        if (items_.size() > 0) // do nothing if there are no items
        {
            switch (e->type())
            {
            case event::key_down:
            {
                key_event::ptr ke = boost::shared_static_cast<key_event>(e);
                switch ( ke->fn_key() )
                {
                case key_uparrow:
                    --current_;
                    if (current_ < 0) 
                    {
                        if (wrap_) current_ %= items_.size();
                        else current_ = 0;
                    }
                    on_change();
                    redraw(rect_);
                    return;
                    break;
                case key_enter:
                case key_downarrow:
                    ++current_;
                    if (current_ >= items_.size())
                    {
                        if (wrap_) current_ %= items_.size();
                        else current_ = items_.size()-1;
                    }
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
                {
                    --current_;
                    if (current_ < 0) 
                    {
                        if (wrap_) current_ %= items_.size();
                        else current_ = 0;
                    }
                }
                else
                {
                    ++current_;
                    if (current_ >= items_.size())
                    {
                        if (wrap_) current_ %= items_.size();
                        else current_ = items_.size()-1;
                    }
                }
                on_change();
                redraw(rect_);
                return;
            }
            }
        }
        widget::handle_event(e);
    }

    unsigned int spinner::selected()
    {
        return current_;
    }
    void spinner::selected(unsigned int index)
    {
        if (index < items_.size())
        {
            current_ = index;
            redraw(rect_);
        }
        else
            throw error_message_exception("spinner::selected() - index out of bounds");
    }

}
