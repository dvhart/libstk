/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Used to Bind keys to functors that can be connected widget 
 *              on_keyup signals.
 *     AUTHORS: Darren Hart
 *  START DATE: 2003/Jun/03
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Str�mke, Dirk H�rner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_HOTKEY_H
#define STK_HOTKEY_H

#include <boost/signal.hpp>
#include <libstk/keycode.h>
#include <libstk/logging.h>

namespace stk
{

    class hotkey
    {
    private:
        stk::keycode key_;

    public:
        hotkey(const stk::keycode key = key_unknown) : key_(key)
        { }

        boost::signal<bool ()> sig;

        // FIXME: this should receive a key_event (keycode only defines the fn_keys)
        bool operator()(stk::keycode key)
        {
            INFO("operator() - received a " << key);
            if (!sig.empty() && key == key_)
            {
                INFO("\texecuting slot");
                return sig();
            }
            return false; // not handled
        }
    };
}
#endif
