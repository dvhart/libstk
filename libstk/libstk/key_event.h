/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Event class for keyboard and other buttons (remotes, etc.).
 *     AUTHORS: Darren Hart, Marc Strämke
 *  START DATE: 2003/Mar/03
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
        keycode fn_key_;
        modcode modlist_;
        wchar_t character_;

    public:
        key_event(keycode fn_key = key_null, event_type type = key_up, modcode modlist = mod_none, wchar_t character = 0);
        virtual ~key_event();
        keycode fn_key() const { return fn_key_; }
        modcode modlist() const { return modlist_; }
        wchar_t character() const { return character_; }
    }; //class key_event
} // namespace stk

#endif
