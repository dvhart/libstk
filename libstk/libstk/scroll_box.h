/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Scroll Box container with scroll bar policies.
 *     AUTHORS: Darren Hart
 *  START DATE: 2003/Oct/22
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
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

    private:

    protected:
        scroll_box(container::ptr parent, const rectangle& rect);

        // FIXME: this is not thread safe
        bool check_scrollable_;
        // FIXME: use an enum with always, never, auto
        bool v_policy_;
        bool h_policy_;
        rectangle child_rect_;
        
        scroll_bar::ptr h_scroll_bar_;
        scroll_bar::ptr v_scroll_bar_;
        viewport::ptr viewport_;
        widget::ptr child_;

    public:
        // FIXME: consider scroll_bar show properties { never, always, if_needed }
        static scroll_box::ptr create(container::ptr parent, const rectangle& rect, 
                bool v_policy, bool h_policy);
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
        void h_scroll(scroll_model::ptr model); 

        scroll_model::ptr v_scroll();
        void v_scroll(scroll_model::ptr model);

        // FIXME: use an enum with always, never, auto
        void scroll_policies(bool h_policy, bool v_policy);
        /********** END SCROLL BOX INTERFACE **********/
    };
}

#endif
