/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: The abstract base class for all widgets.
 *     AUTHORS: Darren Hart
 *  START DATE: 2002/Sep/08
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_WIDGET_H
#define STK_WIDGET_H

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/signal.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <libstk/component.h>
#include <libstk/keycode.h>
#include <libstk/combiners.h>
#include <libstk/mouse_event.h>

namespace stk
{
    class application;
    class container;
    class state;

    class widget : public component, public boost::enable_shared_from_this<widget>
    {
    public:
        typedef boost::shared_ptr<widget> ptr;
        typedef boost::weak_ptr<widget> weak_ptr;

    private:

    protected:
        widget(const rectangle& rect=rectangle(0,0,0,0));
        component::weak_ptr parent_;
        rectangle rect_;

        // widget attributes
        bool focusable_;
        bool visible_;
        bool modal_;
        /// used for animation by the draw routine, or ignored for non-animated widgets
        int frame_;

        // drawing related attributes
        // FIXME: handle mouse presses for pressed in widget::handle_event
        bool pressed_; // does this belong in button only ?
        // it is nice to have it here so that handle_event can handle
        // the common cases, see handle_event()
        bool focused_;
        bool hover_;

    public:
        virtual ~widget();

        /********** RECTANGLE INTERFACE **********/
        /// Get the rectangular region ocupied by the widget
        virtual rectangle rect() { return rect_; }
        virtual void rect(const rectangle& rect);
        virtual void width(int val);
        virtual int width() { return rect_.width(); }
        virtual void height(int val);
        virtual int height() { return rect_.height(); }
        virtual point position() { return rect_.p1(); }
        virtual void position(const point& p);
        /********** END RECTANGLE INTERFACE **********/

        /********** EVENT HANDLER INTERFACE **********/
        /// Try to handle standard events.
        /// Handing the event up to parent_ if the event isn't handled.
        virtual void handle_event(event::ptr e);
        /********** END EVENT HANDLER INTERFACE **********/

        /********** DRAWABLE INTERFACE **********/
        virtual surface::ptr surface();
        virtual void redraw(const rectangle& rect, drawable* source=NULL, bool transform=false);
        /********** END DRAWABLE INTERFACE **********/

        /********** COMPONENT INTERFACE **********/
        /// Return the parent pointer.
        virtual component::ptr parent() { return parent_.lock(); }
        virtual void parent(boost::shared_ptr<container> parent);
        /********** END COMPONENT INTERFACE **********/

        /********** WIDGET INTERFACE **********/
        virtual bool is_container() { return false; }
        // widget attribute accessor methods
        // FIXME: have the setters return bool ?
        // (ie label would return false for a focus(true) call) ?
        /// Return a bool indicating if the widget is focusable
        virtual bool focusable() const { return focusable_; }
        /// Set the focusable property of the widget
        virtual void focusable(bool val) { focusable_ = val; }
        /// Return a bool indicating if the widget is visible
        virtual bool visible() const { return visible_; }
        /// Set the visible property of the widget
        virtual void visible(bool val) { visible_ = val; }
        /// Return a bool indicating if the widget is modal
        virtual bool modal() const { return modal_; }
        /// Set the modal property of the widget
        virtual void modal(bool val) { modal_ = val; }
        /// Return the pressed property of the widget
        virtual bool pressed() const { return pressed_; }
        /// Return the focused property of the widget
        virtual bool focused() const { return focused_; }
        /// Return the hover property of the widget
        virtual bool hover() const { return hover_; }

        boost::signal<bool (), combiner::logical_or<bool> > on_resize;
        boost::signal<bool (), combiner::logical_or<bool> > on_focus;
        boost::signal<bool (), combiner::logical_or<bool> > on_unfocus;
        boost::signal<bool (), combiner::logical_or<bool> > on_mouse_enter;
        boost::signal<bool (), combiner::logical_or<bool> > on_mouse_leave;
        boost::signal<bool (stk::keycode), combiner::logical_or<bool> > on_keydown;
        boost::signal<bool (stk::keycode), combiner::logical_or<bool> > on_keyup;
        /********** END WIDGET INTERFACE **********/

        /********** COMPONENT INTERFACE **********/
        virtual widget::ptr widget_at(int x, int y);
        virtual widget::ptr delegate_mouse_event(mouse_event::ptr me);
        virtual widget::ptr get_active_child() { return shared_from_this(); }
        /********** END COMPONENT INTERFACE **********/


        /*
        // some cool things we can do with boost::signals
        //
        video_panel::ptr my_vp = video_panel::create(...);
        button::ptr my_button::create(...);

        // we don't need any arguments in general for slots since we can connect
        // functors, and specific object member methods
        my_button->on_click.connect(boost::bind(&stk::video_panel::play, my_vp));

        // perhaps we want to rewind and start over on_click (in that order)
        my_button->on_click.connect(0, boost::bind(&stk::video_panel::rewind, my_vp));
        my_button->on_click.connect(1, boost::bind(&stk::video_panel::play, my_vp));
        */

    }
    ; // class widget
} // namespace stk

#endif
