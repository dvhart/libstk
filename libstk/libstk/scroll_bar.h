/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Scroll bar class 
 *     AUTHORS: Marc Strämke, Darren Hart
 *  START DATE: 2003/Oct/8
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_SCROLL_BAR_H
#define STK_SCROLL_BAR_H

#include <boost/smart_ptr.hpp>
#include <boost/signal.hpp>

#include <libstk/scroll_model.h>
#include <libstk/widget.h>
#include <libstk/container.h>

namespace stk
{
    class scroll_bar : public widget
    {
    public:
        typedef boost::shared_ptr<scroll_bar> ptr;
        typedef boost::weak_ptr<scroll_bar> weak_ptr;
        /// the default width/height of a vertical/horizontal scrollbar, define in each theme
        static int default_size;

    protected:
        scroll_bar(container::ptr parent, const rectangle& rect, scroll_model::ptr model);

        scroll_model::ptr model_;
        boost::signals::connection scroll_con_;

        /// helper function to redraw the whole widget
        void redraw();

    public:
        static scroll_bar::ptr create(container::ptr parent, const rectangle& rect, 
                scroll_model::ptr model);
        virtual ~scroll_bar();

        /********** EVENT HANDLER INTERFACE **********/
        virtual void handle_event(event::ptr e);
        /********** END EVENT HANDLER INTERFACE **********/

        /********** DRAWABLE INTERFACE **********/
        virtual void draw(surface::ptr surface, const rectangle& clip_rect = rectangle());
        /********** END DRAWABLE INTERFACE **********/

        /********** COMPONENT INTERFACE **********/
        /********** END COMPONENT INTERFACE **********/

        /********** SCROLL BAR INTERFACE **********/
        scroll_model::ptr model() { return model_; }
        void model(scroll_model::ptr model);
        /********** END SCROLL BAR INTERFACE **********/

    };
} // namespace stk

#endif
