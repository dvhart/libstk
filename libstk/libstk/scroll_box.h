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
        // FIXME: consider scroll_bar show properties { never, always, if_needed }
        scroll_box(container::ptr parent, const rectangle& rect);

        bool constructed_;
        scroll_bar::ptr h_scroll_bar_;
        scroll_bar::ptr v_scroll_bar_;
        viewport::ptr viewport_;

        /// helper function to redraw the whole widget
        void redraw();
    public:
        static scroll_box::ptr create(container::ptr parent, const rectangle& rect, 
                bool v_scroll, bool h_scroll);
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
        scroll_model::ptr h_scroll() { return viewport_->h_scroll(); }
        void h_scroll(scroll_model::ptr model); 

        scroll_model::ptr v_scroll() { return viewport_->v_scroll(); }
        void v_scroll(scroll_model::ptr model);
        /********** END SCROLL BOX INTERFACE **********/
    };
}

#endif
