/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: A container of "tabs"
 *     AUTHORS: Marc Strämke
 *  START DATE: 2003/Sep/11
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available
 *              with the source as license.txt or at 
 *              http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_SWITCHED_PANEL_H
#define STK_SWITCHED_PANEL_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <libstk/container.h>

namespace stk
{
    class switched_panel : public container
    {
    public:
        typedef boost::shared_ptr<switched_panel> ptr;
        typedef boost::weak_ptr<switched_panel> weak_ptr;
    private:
    protected:
        widget::weak_ptr active_widget;
        switched_panel(container::ptr parent, rectangle rect);
    public:
        void active_child(widget::ptr child) throw(error_no_child);
        static switched_panel::ptr create(container::ptr parent, rectangle rect);

        /********** DRAWABLE INTERFACE **********/
        virtual void draw(surface::ptr surface, const rectangle& clip_rect = rectangle());
        virtual void redraw(const rectangle& rect, drawable* source=NULL, bool transform=false);
        /********** END DRAWABLE INTERFACE **********/
        
        /********** CONTAINER INTERFACE **********/
        virtual widget::ptr widget_at(int x, int y);
        virtual widget::ptr delegate_mouse_event(mouse_event::ptr me);
        /********** END CONTAINER INTERFACE **********/
    };
}


#endif
