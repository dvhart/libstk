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

#ifndef STK_LIST_ITEM_H
#define STK_LIST_ITEM_H

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <libstk/rectangle.h>
#include <libstk/event_handler.h>
#include <libstk/drawable.h>
#include <libstk/event.h>
#include <libstk/widget.h>

namespace stk
{
    class list;

    class list_item : public widget
    {
    public:
        typedef boost::weak_ptr<list_item> weak_ptr;
        typedef boost::shared_ptr<list_item> ptr;

    private:

    protected:
        bool current_, selected_;
        list_item(std::wstring label);
        std::wstring label_;

    public:
        static list_item::ptr create(boost::shared_ptr<list> parent, std::wstring label);
        virtual ~list_item();

        /********** EVENT HANDLER INTERFACE **********/
        virtual void handle_event(event::ptr e);
        /********** END EVENT HANDLER INTERFACE **********/

        /********** DRAWABLE INTERFACE **********/
        virtual void draw(surface::ptr surface, const rectangle& clip_rect = rectangle());
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
