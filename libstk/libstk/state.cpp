/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: State container implementation.
 *     AUTHORS: Marc Str�mke, Darren Hart
 *  START DATE: 2003/Nov/29
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Str�mke, Dirk H�rner
 *     LICENSE: This software is licenced under the Libstk license available with the source as
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include <iostream>
#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include "libstk/state.h"
#include "libstk/container.h"
#include "libstk/application.h"

namespace stk
{
    state::ptr state::create(application::ptr parent, std::string bg_image_path)
    {
        state::ptr new_state(new state(parent));
        new_state->bg_image_ = image::create(new_state->surface(), bg_image_path);
        parent->add_state(new_state);
        return new_state;
    }

    state::ptr state::create(application::ptr parent)
    {
        state::ptr new_state(new state(parent));
        parent->add_state(new_state);
        return new_state;
    }

    state::state(boost::shared_ptr<application> parent) : container(parent->surface()->rect())
    {
        INFO("state::state()");
        parent_ = boost::shared_static_cast<component>(parent);
        focusable_ = false;
    }

    state::~state()
    {}

    // event_handler interface - default back to parent
    void state::handle_event(event::ptr e)
    {
        //INFO("state::handle_event()");
        // handle appropriate events here
        // WRITEME ...

        container::handle_event(e);
    }

    // FIXME: delve into containers too
    // FIXME: error checking (empty children_, no focusable children, etc.)
    // we can return an empty pointer
    widget::ptr state::focus_first()
    {
        //INFO("state::focus_first()");
        widget::ptr temp_widget;
        std::vector<widget::ptr>::iterator iter = children_.begin();
        for (; iter != children_.end(); iter++)
        {
            if (!temp_widget && (*iter)->focusable())
            {
                temp_widget = *iter;
                //temp_widget->focused(true);
                continue;
            }
            // keep looking for the focused widget if there is one
            if ((*iter)->focused())
            {
                //(*iter)->focused(false);
                break; // only one can be focused at a time
            }
        }
        return temp_widget;
    }

    // FIXME: error checking (empty children_, no focusable children, etc.)
    // we can return an empty pointer
    widget::ptr state::focus_last()
    {
        //INFO("state::focus_last()");
        widget::ptr temp_widget;
        std::vector<widget::ptr>::iterator iter = children_.begin();
        for (; iter != children_.end(); iter++)
        {
            if ((*iter)->focusable())
            {
                temp_widget = *iter;
            }
            //if ((*iter)->focused()) (*iter)->focused(false);
        }
        //if (temp_widget)
        //temp_widget->focused(true);
        return temp_widget;
    }

}
