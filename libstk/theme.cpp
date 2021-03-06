/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Theme class implementation
 *     AUTHORS: Darren Hart, Vernon Mauery, Marc Str�mke
 *  START DATE: 2003/Mar/17
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Str�mke, Dirk H�rner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include "libstk/theme.h"

namespace stk
{
    // static class members
    theme::weak_ptr theme::instance_;
    boost::shared_ptr<user_theme> theme::user_theme_;

    theme::theme(surface::ptr surface) : surface_(surface)
    {}

    theme::~theme()
    {
        user_theme_.reset();
    }

    theme::ptr theme::get()
    {
        theme::ptr instance = instance_.lock();
        if (!instance)
            throw error_message_exception("theme::get() - instance_ null, call create() first");
        return instance ;
    }

    boost::shared_ptr<user_theme> theme::user()
    {
        if (!user_theme_)
            throw error_message_exception("theme::user() - user_theme_ null, call create() first");
        return user_theme_;
    }
}
