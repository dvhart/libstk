/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Libstk composite class of the composite pattern, component
 *              defines the component poriton of the pattern.
 *     AUTHORS: Dirk H�rner, Marc Str�mke, Darren Hart
 *  START DATE: 2002/Sep/17
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Str�mke, Dirk H�rner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_CONTAINER_H
#define STK_CONTAINER_H

#include <map>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <libstk/application.h>
#include <libstk/widget.h>
#include <libstk/mouse_event.h>

namespace stk
{
    class container : public widget
    {
    public:
        typedef boost::shared_ptr<container> ptr;
        typedef boost::weak_ptr<container> weak_ptr;

    private:

    protected:
        std::vector<widget::ptr> children_;
        container(const rectangle& rect);
        
        /// This routine draws the child given in its second parameter
        /// The surface offset and clip_rect is set for the child and then the child
        /// draw routine is called.  Finally the offset is restored, but the clip_rect
        /// is not as it is expected that the clip_rect is set explicitly before each draw.
        /// \param surface The surface to draw to 
        /// \param clip_rect The clipping rectangle to use
        /// \param child The container's child widget to draw
        void draw_child(surface::ptr surface, const rectangle& clip_rect, widget::ptr child);

    public:
        virtual ~container();

        /********** EVENT HANDLER INTERFACE **********/
        virtual void handle_event(event::ptr e);
        /********** END EVENT HANDLER INTERFACE **********/

        /********** DRAWABLE INTERFACE **********/
        //virtual surface::ptr surface();
        /// Draw each of the children
        /// All derived containers should call container::draw(surface, clip_rect) to draw their
        /// children.
        /// \param surface The surface to draw to 
        /// \param clip_rect The clipping rectangle to use
        virtual void draw(surface::ptr surface, const rectangle& clip_rect = rectangle());
        virtual void redraw(const rectangle& rect, drawable* source=NULL, bool transform=false);
        /********** END DRAWABLE INTERFACE **********/

        /********** COMPONENT INTERFACE **********/
        /// Return the next focusable widget.
        /// Find the focused widget, and return the next focusable one.  If we
        /// have no children, or reach the end, ask return an empty widget::ptr.
        virtual widget::ptr focus_next();
        /// Return the prev focusable widget.
        /// Find the focused widget, and return the previous focusable one.  If
        /// we have no children, or reach the beginning,  return an empty
        /// widget::ptr.
        virtual widget::ptr focus_prev();
        /// Add a child widget to the children vector
        virtual void add(widget::ptr w);
        /// Remove a child widget from the children vector
        virtual void remove(widget::ptr item);
        /********** END COMPONENT INTERFACE **********/

        /********** WIDGET INTERFACE **********/
        /// If any of the children widgets are focused, return true.
        virtual bool focused() const;
        virtual bool is_container() { return true; }
        /********** END WIDGET INTERFACE **********/

        /********** CONTAINER INTERFACE **********/
        // the container interface really defines the composite methods
        // that parent and widget declared

        /// Returns a widget::ptr to the container's first child that contains
        /// x,y.  It will call widget_at on a child that is a container.
        virtual widget::ptr widget_at(int x, int y);
        /// Pass mouse events down to widgets.
        virtual widget::ptr delegate_mouse_event(mouse_event::ptr me);
        // FIXME: add stl style iterator interface
        /********** END CONTAINER INTERFACE **********/
    };

} // namespace stk

#endif
