/**************************************************************************************************
 *    FILENAME: state.cpp
 * DESCRIPTION: State container implementation.
 *     AUTHORS: Marc Straemke, Darren Hart
 *  START DATE: 29/Nov/2003  LAST UPDATE: 13/Jul/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available with the source as
 *     license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include <iostream>
#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>

#include "libstk/state.h"
#include "libstk/container.h"
#include "libstk/application.h"

using std::cout;
using std::endl;

namespace stk
{

    state::ptr state::create(application::ptr parent)
    {
        state::ptr new_state(new state(parent));
        parent->add_state(new_state);
        return new_state;
    }

    state::state(boost::shared_ptr<application> parent) :
            container(boost::shared_static_cast<component>(parent), parent->surface()->rect())
    {
        cout << "state::state()" << endl;
        focusable_ = false;
    }

    state::~state()
    {}

    // event_handler interface - default back to parent
    void state::handle_event(event::ptr e)
    {
        //cout << "state::handle_event()" << endl;
        // handle appropriate events here
        // WRITEME ...

        container::handle_event(e);
    }

    // FIXME: delve into containers too
    // FIXME: error checking (empty children_, no focusable children, etc.)
    // we can return an empty pointer
    widget::ptr state::focus_first()
    {
        //cout << "state::focus_first()" << endl;
        widget::ptr temp_widget;
        std::vector<widget::ptr>::iterator iter = children_.begin();
        for (iter; iter != children_.end(); iter++)
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
        //cout << "state::focus_last()" << endl;
        widget::ptr temp_widget;
        std::vector<widget::ptr>::iterator iter = children_.begin();
        for (iter; iter != children_.end(); iter++)
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
