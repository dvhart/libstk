/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Widget abstract base class implementation.
 *     AUTHORS: Darren Hart, Marc Strämke, Dirk Hörner
 *  START DATE: 2003/Sep/08
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <string>

#include "libstk/application.h"
#include "libstk/component.h"
#include "libstk/container.h"
#include "libstk/key_event.h"
#include "libstk/mouse_event.h"
#include "libstk/widget.h"

namespace stk
{

    widget::widget(component::ptr parent, const rectangle& rect) : parent_(parent), rect_(rect),
        focusable_(false), pressed_(false), focused_(false), hover_(false), frame_(0)
    {
        INFO("constructor");
    }

    widget::~widget()
    {
        INFO("destructor");
    }

    /// Handle common widget events
    /// Try to handle the passed event, if there is nothing to do with it pass it up to the parent.
    /// Every non container should call widget::handle_event(e) from within its handle_event
    /// routine if it hasn't handled the event on its own.
    void widget::handle_event(event::ptr e)
    {
        // default handlers for focus/un_focus mouse_enter/leave events
        // a derived widget should call widget::handle_event() for these basic events
        switch (e->type())
        {
        case event::focus:
            if (focusable_)
            {
                focused_ = true;
                redraw(rect_);
            }
            return;
            break;
        case event::un_focus:
            if (focusable_)
            {
                focused_ = false;
                pressed_ = false;
                redraw(rect_);
            }
            return;
            break;
        case event::mouse_enter:
            hover_ = true;
            redraw(rect_);
            return;
            break;
        case event::mouse_leave:
            hover_ = false;
            pressed_ = false;
            redraw(rect_);
            return;
            break;
        case event::key_up:
            key_event::ptr ke = boost::shared_static_cast<key_event>(e);
            if (on_keyup(ke->fn_key())) return;
            break;
        }

        parent_.lock()->handle_event(e);
    }

    // drawable interface
    surface::ptr widget::surface()
    {
        // MSTR broken!
        return parent_.lock()->surface();
    }

    void widget::redraw(const rectangle& rect)
    {
        parent_.lock()->redraw(rect);
    }

}
