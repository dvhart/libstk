/******************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Theme class implementation
 *     AUTHORS: Darren Hart, Vernon Mauery, Marc Straemke
 *  START DATE: 2003/Mar/17
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available
 *              with the source as license.txt or at 
 *              http://www.libstk.org/index.php?page=docs/license
 *****************************************************************************/

#include "libstk/theme.h"

namespace stk
{
    // static class members
    theme::ptr theme::instance_;
    boost::shared_ptr<user_theme> theme::user_theme_;

    theme::theme(surface::ptr surface) : surface_(surface)
    {}

    theme::~theme()
    {}

    theme::ptr theme::get
        ()
    {
        if (!instance_)
            throw error_message_exception("theme::get() - instance_ null, call create() first");
        return instance_;
    }

    boost::shared_ptr<user_theme> theme::user()
    {
        if (!user_theme_)
            throw error_message_exception("theme::user() - user_theme_ null, call create() first");
        return user_theme_;
    }
}
