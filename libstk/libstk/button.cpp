/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Button widget implementation.
 *     AUTHORS: Darren Hart, Marc Str�mke 
 *  START DATE: 2003/Mar/03
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Str�mke, Dirk H�rner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include <iostream>
#include "libstk/button.h"
#include "libstk/widget.h"

// FIXME: should we include these like this, or maybe in one events.h file for convenience ?
#include "libstk/event.h"
#include "libstk/key_event.h"
#include "libstk/mouse_event.h"
#include "libstk/keycode.h"

namespace stk
{

    button::ptr button::create(container::ptr parent, const std::wstring label,
                               const rectangle& rect)
    {
        button::ptr new_button(new button(parent, label, rect));
        parent->add
        (new_button);
        return new_button;
    }

    button::button(boost::shared_ptr<container> parent, const std::wstring label,
                   const rectangle& rect) : widget(parent, rect), label_(label)
    {
        INFO("constructor");
        focusable(true);
    }

    button::~button()
    {
        INFO("destructor");
    }

    // event_handler interface
    void button::handle_event(event::ptr e)
    {
        switch (e->type())
        {
        case event::key_up:
        {
            // FIXME: where should default actions be taken care of ?
            // next and prev are currently in App, so activate (enter) should
            // probably be with them...
            key_event::ptr ke = boost::shared_static_cast<key_event>(e);
            switch ( ke->fn_key() )
            {
            case key_enter:
                if (pressed_)
                {
                    pressed_ = false;
                    redraw(rect_);
                    on_release();
                }
                return;
                break;
            }
            break; // key_up
        }
        case event::key_down:
        {
            key_event::ptr ke = boost::shared_static_cast<key_event>(e);
            switch ( ke->fn_key() )
            {
            case key_enter:
                pressed_ = true;
                redraw(rect_);
                on_press();
                return;
                break;
            }
            break; // key_down
        }
        // FIXME: this stuff should be moved to application or state I think
        case event::mouse_motion:
        {
            break; // mouse_motion
        }
        case event::mouse_down:
        {
            mouse_event::ptr me = boost::shared_static_cast<mouse_event>(e);
            if (!pressed())
            {
                pressed_ = true;
                redraw(rect_);
                on_press();
            }
            return;
            break; // mouse_down
        }
        case event::mouse_up:
        {
            mouse_event::ptr me = boost::shared_static_cast<mouse_event>(e);
            if (pressed())
            {
                pressed_ = false;
                redraw(rect_);
                on_release();
            }
            return;
            break; // mouse_up
        }
        }
        // if we haven't handled it, try the defaults and possibly pass it up
        widget::handle_event(e);
    }
}
