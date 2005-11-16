/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: The theme class implements the pimpl idiom to provide access to
 *              the user defined user_theme which is used for storage and 
 *              common theme routines.
 *     AUTHORS: Darren Hart, Vernon Mauery, Marc Strämke
 *  START DATE: 2003/Mar/17
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_THEME_H
#define STK_THEME_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <libstk/surface.h>

namespace stk
{
    // forward declare the user theme class
    class user_theme;

    /// A singleton class for theme specific colors and draw routines.
    /// This singleton get initialized when the application is created.
    class theme
    {
    public:
        typedef boost::shared_ptr<theme> ptr;
        typedef boost::weak_ptr<theme> weak_ptr;

    private:
        theme(surface::ptr surface);
        static theme::weak_ptr instance_;
        surface::ptr surface_; // if there are multiple surfaces, it may not
        // make sense to store a reference to one here

        static boost::shared_ptr<user_theme> user_theme_;

    public:
        static theme::ptr create(surface::ptr surface);
        virtual ~theme();
        static theme::ptr get();
        static boost::shared_ptr<user_theme> user();


        // theme specific drawing routines
        // ie draw_corner(), etc.
    };
}

#endif
