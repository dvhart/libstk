/**************************************************************************************************
 *    FILENAME: key_event.h
 * DESCRIPTION: Event class for keyboard and other buttons (remotes, etc.).
 *     AUTHORS: Darren Hart, Marc Strämke
 *  START DATE: 03/Mar/2003  LAST UPDATE: 26/Jul/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_KEY_EVENT_H
#define STK_KEY_EVENT_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <libstk/event.h>
#include <libstk/keycode.h>

namespace stk
{
    class key_event : public event
    {
    public:
        typedef boost::shared_ptr<key_event> ptr;
        typedef boost::shared_ptr<key_event> weak_ptr;
    private:

    protected:
        keycode keycode_;

    public:
        key_event(keycode key=key_null, event_type type=key_up);
        virtual ~key_event();
        keycode key()
        {
            return keycode_;
        }
    }; //class key_event
} // namespace stk

#endif
