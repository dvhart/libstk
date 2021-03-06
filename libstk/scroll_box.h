/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Scroll Box container with scroll bar policies.
 *     AUTHORS: Darren Hart
 *  START DATE: 2003/Oct/22
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Str�mke, Dirk H�rner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_SCROLL_BOX_H
#define STK_SCROLL_BOX_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <libstk/container.h>
#include <libstk/scroll_bar.h>
#include <libstk/scrollable.h>
#include <libstk/viewport.h>

namespace stk
{
    class scroll_box : public container
    {
    public:
        typedef boost::shared_ptr<scroll_box> ptr;
        typedef boost::weak_ptr<scroll_box> weak_ptr;
        typedef enum { always, never, automatic} scroll_policy;

    protected:
        scroll_box(const rectangle& rect);
        void update_model_cons();
        void model_change();

        // FIXME: this is not thread safe
        bool check_scrollable_;
        
        scroll_policy v_policy_;
        scroll_policy h_policy_;
        rectangle child_rect_;
        
        scroll_bar::ptr h_scroll_bar_;
        scroll_bar::ptr v_scroll_bar_;
        viewport::ptr viewport_;
        widget::ptr child_;

        boost::signals::connection v_scroll_con_;
        boost::signals::connection h_scroll_con_;

    public:
        static scroll_box::ptr create(container::ptr parent, const rectangle& rect, 
                scroll_policy v_policy, scroll_policy h_policy);
        ~scroll_box();

        /********** DRAWABLE INTERFACE **********/
        virtual void draw(surface::ptr surface, const rectangle& clip_rect = rectangle());
        /********** END DRAWABLE INTERFACE **********/

        /********** EVENT HANDLER INTERFACE **********/
        virtual void handle_event(event::ptr e);
        /********** END EVENT HANDLER INTERFACE **********/

        /********** COMPONENT INTERFACE **********/
        /// Set the internal widget, replace it if already set.
        /// scroll_box only manages one child.
        virtual void add(widget::ptr item);
        /********** END COMPONENT INTERFACE **********/

        /********** SCROLL BOX INTERFACE **********/
        scroll_model::ptr h_scroll();
        void h_scroll(scroll_model::ptr model); // FIXME: should we provide this?

        scroll_model::ptr v_scroll();
        void v_scroll(scroll_model::ptr model); // FIXME: should we provide this?

        void scroll_policies(scroll_policy h_policy, scroll_policy v_policy);
        /********** END SCROLL BOX INTERFACE **********/
    };
}

#endif
