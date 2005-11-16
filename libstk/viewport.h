/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: A scrolling area container.
 *     AUTHORS: Darren Hart, Marc Strämke
 *  START DATE: 2003/Apr/29
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available
 *              with the source as license.txt or at 
 *              http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_VIEWPORT_H
#define STK_VIEWPORT_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <libstk/container.h>
#include <libstk/scroll_model.h>
#include <libstk/scrollable.h>

namespace stk
{
    class viewport : public container, public scrollable
    {
    public:
        typedef boost::shared_ptr<viewport> ptr;
        typedef boost::weak_ptr<viewport> weak_ptr;
        
        // FIXME: we shouldn't need these !
        using scrollable::h_scroll;
        using scrollable::v_scroll;
    private:

    protected:
        viewport(const rectangle& rect);

        boost::signals::connection h_scroll_con;
        boost::signals::connection v_scroll_con;

        /// helper function to redraw the whole widget
        void redraw();
    public:
        static viewport::ptr create(container::ptr parent, const rectangle& rect);
        virtual ~viewport();

        /********** DRAWABLE INTERFACE **********/
        virtual void draw(surface::ptr surface, const rectangle& clip_rect = rectangle());
        /********** END DRAWABLE INTERFACE **********/

        /********** EVENT HANDLER INTERFACE **********/
        virtual void handle_event(event::ptr e);
        /********** END EVENT HANDLER INTERFACE **********/

        /********** COMPONENT INTERFACE **********/
        /// Set the internal widget, replace it if already set.
        /// viewport only manages one child.
        virtual void add(widget::ptr item);
        /********** END COMPONENT INTERFACE **********/

        /********** VIEWPORT INTERFACE **********/
        virtual void h_scroll(scroll_model::ptr model);
        virtual void v_scroll(scroll_model::ptr model);
        /********** END VIEWPORT INTERFACE **********/
    };
}

#endif
