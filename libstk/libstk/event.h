/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Basic event class.
 *     AUTHORS: Darren Hart, Marc Strämke
 *  START DATE: 2003/Feb/22
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_EVENT_H
#define STK_EVENT_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace stk
{
    class event
    {
    public:
        typedef boost::shared_ptr<event> ptr;
        typedef boost::weak_ptr<event> weak_ptr;

        /// fixme: should this be static ?
        enum event_type
        {
            key_down,
            key_up,
            mouse_down,
            mouse_up,
            mouse_motion,
            mouse_enter,
            mouse_leave,
            focus,
            un_focus,
            quit,
            none,
            unknown
        };

    private:

    protected:
        event(event_type type) : type_(type)
        { }
        event_type type_;

    public:
        static event::ptr create(event_type type)
        {
            event::ptr new_event(new event(type));
            return new_event;
        }
        virtual ~event()
        { }
        event_type type()
        {
            return type_;
        }
        bool operator!()
        {
            return type_ == none;
        }

    }; //class event
} // namespace stk

#endif
