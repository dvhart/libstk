/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: A simple ownerdraw_widget widget.
 *     AUTHORS: Darren Hart, Vernon Mauery, Marc Strämke
 *  START DATE: 2003/Mar/03
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_OWNERDRAW_WIDGET_H
#define STK_OWNERDRAW_WIDGET_H

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/signal.hpp>
#include <boost/optional.hpp>

#include <libstk/widget.h>
#include <libstk/container.h>
#include <libstk/font_properties.h>

namespace stk
{
    class ownerdraw_widget : public widget
    {
    public:
        typedef boost::shared_ptr<ownerdraw_widget> ptr;
        typedef boost::weak_ptr<ownerdraw_widget> weak_ptr;

    private:
    protected:
        ownerdraw_widget(const rectangle& rect);

    public:
        static ownerdraw_widget::ptr create(container::ptr parent,
                                            const rectangle& rect);
        virtual ~ownerdraw_widget();

        //! Called when the widget has to be drawn
        boost::function<void(surface::ptr, rectangle const&)> draw_callback;
        //! Called when an event is received, return true if Handled!
        boost::function<bool(event::ptr)> event_callback;
        
        virtual void draw(surface::ptr surface, rectangle const& clip_rect = rectangle());
        virtual void handle_event(event::ptr e);
    };

} // namespace stk

#endif
