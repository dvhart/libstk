/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: A simple item for lists, can be derived to contain images, etc.
 *     AUTHORS: Darren Hart 
 *  START DATE: 2003/Mar/03
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef LIST_ITEM_H
#define LIST_ITEM_H

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <libstk/rectangle.h>
#include <libstk/event_handler.h>
#include <libstk/drawable.h>
#include <libstk/event.h>

namespace stk
{
    class list;

    class list_item : public event_handler, public drawable
    {
    public:
        typedef boost::weak_ptr<list_item> weak_ptr;
        typedef boost::shared_ptr<list_item> ptr;

    private:

    protected:
        boost::weak_ptr<list> parent_;
        bool current_, selected_;
        rectangle rect_;
        list_item(boost::shared_ptr<list> parent, std::wstring label);
        std::wstring label_;

    public:
        static list_item::ptr create(boost::shared_ptr<list> parent, std::wstring label);
        virtual ~list_item();

        /********** EVENT HANDLER INTERFACE **********/
        virtual void handle_event(event::ptr e);
        /********** END EVENT HANDLER INTERFACE **********/

        /********** DRAWABLE INTERFACE **********/
        virtual surface::ptr surface();
        virtual void draw(surface::ptr surface, const rectangle& clip_rect = rectangle());
        //virtual void redraw(const rectangle& rect);
        /********** END DRAWABLE INTERFACE **********/

        /********** LIST ITEM INTERFACE **********/
        std::wstring label() const { return label_; }
        void label(const std::wstring& label) { label_ = label; }

        // wrap the rectangle interface
        // getters
        rectangle rect() const { return rect_; }
        point p1() const { return rect_.p1(); }
        point p2() const { return rect_.p2(); }
        int x1() const { return rect_.x1(); }
        int y1() const { return rect_.y1(); }
        int x2() const { return rect_.x2(); }
        int y2() const { return rect_.y2(); }
        int width() const { return rect_.width(); }
        virtual int height(); // implemented by the theme
        // setters
        void rect(const rectangle& rect) { rect_ = rect; }
        void x1(int val) { rect_.x1(val); }
        void y1(int val) { rect_.y1(val); }
        void x2(int val) { rect_.x2(val); }
        void y2(int val) { rect_.y2(val); }
        void width(int val) { rect_.width(val); }
        //void height(int val) { rect_.height(val); }
        // utils
        bool empty() const { return rect_.empty(); }
        bool contains(int x, int y) const { return rect_.contains(x, y); }
        bool contains(const point& p) const { return rect_.contains(p); }
        bool intersects(const rectangle& rect) const { return rect_.intersects(rect); }
        rectangle intersection(const rectangle& rhs) const { return rect_.intersection(rhs); }
        point position() const { return rect_.position(); }
        void position(int x, int y) { rect_.position(x, y); }
        void position(const point& p) { rect_.position(p); }
        

        bool selected() const { return selected_; }
        void selected(bool val) { selected_ = val; }
        bool current() const { return current_; }
        void current(bool val) { current_ = val; }
        /********** END LIST ITEM INTERFACE **********/
    };
}

#endif
