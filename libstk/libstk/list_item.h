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

    class list_item : public rectangle, public event_handler, public drawable
    {
    public:
        typedef boost::weak_ptr<list_item> weak_ptr;
        typedef boost::shared_ptr<list_item> ptr;

    private:

    protected:
        boost::weak_ptr<list> parent_;
        bool current_, selected_;
        list_item(boost::shared_ptr<list> parent, std::wstring label);
        std::wstring label_;

    public:
        static list_item::ptr create(boost::shared_ptr<list> parent, std::wstring label);
        virtual ~list_item();

        /********** RECTANGLE INTERFACE **********/
        /// Get the rectangular region oocupied by the drawable
        virtual rectangle rect() { return rectangle(p1_, p2_); }

        /// Set the position and size of the drawable
        virtual void rect(const rectangle& rect) 
        { 
            p1(rect.p1()); 
            p2(rect.p2()); 
        }
        /********** END RECTANGLE INTERFACE **********/

        /********** EVENT HANDLER INTERFACE **********/
        virtual void handle_event(event::ptr e);
        /********** END EVENT HANDLER INTERFACE **********/

        /********** DRAWABLE INTERFACE **********/
        virtual surface::ptr surface();
        virtual void draw(surface::ptr surface, const rectangle& clip_rect = rectangle());
        virtual void redraw(const rectangle& rect, drawable* source = NULL, bool transform = false);
        /********** END DRAWABLE INTERFACE **********/

        /********** LIST ITEM INTERFACE **********/
        std::wstring label() const { return label_; }
        void label(const std::wstring& label) { label_ = label; }
        virtual int height(); // implemented by the theme 
        bool selected() const { return selected_; }
        void selected(bool val) { selected_ = val; }
        bool current() const { return current_; }
        void current(bool val) { current_ = val; }
        /********** END LIST ITEM INTERFACE **********/
    };
}

#endif
