/**************************************************************************************************
 *    FILENAME: widget.h
 * DESCRIPTION: The abstract base class for all widgets.
 *     AUTHORS: Darren Hart
 *  START DATE: 08/Sep/2002  LAST UPDATE: 02/Aug/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_WIDGET_H
#define STK_WIDGET_H

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/signal.hpp>

#include <libstk/component.h>
#include <libstk/keycode.h>
#include <libstk/combiners.h>
#include <libstk/mouse_event.h>

namespace stk
{
    class application;
    class container;
    class state;

    class widget : public component
    {
    public:
        typedef boost::shared_ptr<widget> ptr;
        typedef boost::weak_ptr<widget> weak_ptr;

    private:

    protected:
        widget(component::ptr parent, const rectangle& rect=rectangle(0,0,0,0));
        component::weak_ptr parent_;

        rectangle rect_;

        // widget attributes
        // FIXME: add visible_
        bool focusable_;
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

        /********** EVENT HANDLER INTERFACE **********/
        /// Try to handle standard events.
        /// Handing the event up to parent_ if the event isn't handled.
        virtual void handle_event(event::ptr e);
        /********** END EVENT HANDLER INTERFACE **********/

        /********** DRAWABLE INTERFACE **********/
        virtual surface::ptr surface();
        virtual void redraw(const rectangle& rect);
        /********** END DRAWABLE INTERFACE **********/

        /********** COMPONENT INTERFACE **********/
        /// Return the parent pointer.
        virtual component::ptr parent() { return parent_.lock(); }
        /********** END COMPONENT INTERFACE **********/

        /********** WIDGET INTERFACE **********/
        virtual bool is_container() { return false; }
        /// FIXME: what would be a better way to handle all these rect wrappers?
        /// should widget possibly inherit from rectangle ?
        rectangle rect() { return rect_; }
        bool contains(int x, int y) { return rect_.contains(x, y); }
        bool intersects(const rectangle& rect) { return rect_.intersects(rect); }
        int width() { return rect_.width(); }
        int height() { return rect_.height(); }
        void position(int x, int y) { rect_.position(x, y); }
        int x1() { return rect_.x1(); }
        int y1() { return rect_.y1(); }
        int x2() { return rect_.x2(); }
        int y2() { return rect_.y2(); }

        // widget attribute accessor methods
        // FIXME: have the setters return bool ?
        // (ie label would return false for a focus(true) call) ?
        /// Return a bool indicating if the widget is focusable
        virtual bool focusable() { return focusable_; }
        /// Set the focusable property of the widget
        virtual void focusable(bool val) { focusable_ = val; }
        /// Return the pressed property of the widget
        virtual bool pressed() { return pressed_; }
        /// Return the focused property of the widget
        virtual bool focused() { return focused_; }
        /// Return the hover property of the widget
        virtual bool hover() { return hover_; }

        boost::signal<bool (), combiner::logical_or<bool> > on_focus;
        boost::signal<bool (), combiner::logical_or<bool> > on_unfocus;
        boost::signal<bool (), combiner::logical_or<bool> > on_mouse_enter;
        boost::signal<bool (), combiner::logical_or<bool> > on_mouse_leave;
        boost::signal<bool (stk::keycode), combiner::logical_or<bool> > on_keydown;
        boost::signal<bool (stk::keycode), combiner::logical_or<bool> > on_keyup;
        /********** END WIDGET INTERFACE **********/

        /********** COMPOSITE INTERFACE **********/
        // FIXME: make these throw an exception or something
        virtual widget::ptr widget_at(int x, int y)
        {
            return widget::ptr();
        }
        virtual void delegate_mouse_event(mouse_event::ptr me)
        { }
        virtual widget::ptr get_active_child()
        {
            return widget::ptr();
        }
        virtual rectangle redraw_rect()
        {
            return rectangle();
        }
        /********** END COMPOSITE INTERFACE **********/


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
