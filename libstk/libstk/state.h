/*************************************************************************************************
 *    FILENAME: state.h
 * DESCRIPTION: The primary container widget, managed by application. 
 *     AUTHORS: Darren Hart, Marc Straemke 
 *  START DATE: 29/Nov/2003  LAST UPDATE: 13/May/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STATE_H
#define STATE_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <libstk/widget.h>
#include <libstk/container.h>
#include <libstk/application.h>

namespace stk
{
    class state : public container
    {
    public:
        typedef boost::shared_ptr<state> ptr;
        typedef boost::weak_ptr<state> weak_ptr;

    private:

    protected:
        state(application::ptr parent);

    public:
        virtual ~state();
        static state::ptr create(application::ptr parent);

        // these two should probably be moved up to container
        virtual widget::ptr focus_first();
        virtual widget::ptr focus_last();

        // event_handler interface
        virtual void handle_event(event::ptr e);

        // drawable interface
        //virtual surface::ptr surface();
        virtual void draw(surface::ptr surface, const rectangle& clip_rect = rectangle());
        //virtual void redraw(bool val, const rectangle& rect);
    };
}

#endif
