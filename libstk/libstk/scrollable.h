/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: An abstract base class providing vertical and horizontal scroll models to widgets
 *              that need to be aware of their scrolling properties, such as text_are and viewport.
 *     AUTHORS: Darren Hart 
 *  START DATE: 2003/Oct/10
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_SCROLLABLE_H
#define STK_SCROLLABLE_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <libstk/widget.h>
#include <libstk/container.h>
#include <libstk/scroll_model.h>

namespace stk
{
    class scrollable : public widget
    {
        public:
            typedef boost::shared_ptr<scrollable> ptr;
            typedef boost::weak_ptr<scrollable> weak_ptr;

        protected:
            scrollable(container::ptr parent, const rectangle& rect);
            scroll_model::ptr v_scroll_;
            scroll_model::ptr h_scroll_;

        public:
            static scrollable::ptr create(container::ptr parent, const rectangle& rect);
            virtual ~scrollable();

            /********** SCROLLABLE INTERFACE **********/
            scroll_model::ptr h_scroll() { return h_scroll_; }
            void h_scroll(scroll_model::ptr model);

            scroll_model::ptr v_scroll() { return v_scroll_; }
            void v_scroll(scroll_model::ptr model);
            /********** END SCROLLABLE INTERFACE **********/
    };
} // end namespace

#endif
