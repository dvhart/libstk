/******************************************************************************
 *    FILENAME: list.h
 * DESCRIPTION: A listbox widget.  It contains a vector of list_items.
 *     AUTHORS: Darren Hart, Marc Strämke
 *  START DATE: 03/Mar/2003  LAST UPDATE: 22/May/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available
 *              with the source as license.txt or at 
 *              http://www.libstk.org/index.php?page=docs/license
 *****************************************************************************/

#ifndef STK_LIST_H
#define STK_LIST_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <libstk/rectangle.h>
#include <libstk/widget.h>
#include <libstk/container.h>
#include <libstk/list_item.h>
#include <libstk/scroll_model.h>

namespace stk
{
    /// \todo is widget right.. not a container ?
    class list : public widget
    {
    public:
        typedef boost::shared_ptr<list> ptr;
        typedef boost::weak_ptr<list> weak_ptr;

    private:

    protected:
        int current_, selected_, prev_selected_;
        list(container::ptr parent, const rectangle& rect,
             scroll_model::ptr v_scroll = scroll_model::create());
        std::vector<list_item::ptr> items_;

        // scrolling members
        scroll_model::ptr v_scroll_;
        boost::signals::connection v_scroll_con;

    public:
        static list::ptr create(container::ptr parent, const rectangle& rect,
                                scroll_model::ptr v_scroll = scroll_model::create());
        virtual ~list();

        /********** EVENT HANDLER INTERFACE **********/
        virtual void handle_event(event::ptr e);
        /********** END EVENT HANDLER INTERFACE **********/

        /********** DRAWABLE INTERFACE **********/
        virtual void draw(surface::ptr surface, const rectangle& clip_rect = rectangle());
        /********** END DRAWABLE INTERFACE **********/

        /********** COMPONENT INTERFACE **********/
        /// \todo this would have to return a this pointer!!! unless we make list_items a widget and list a container!!!
        //virtual widget::ptr focus_next();
        //virtual widget::ptr focus_prev();
        /********** END COMPONENT INTERFACE **********/


        /********** LIST INTERFACE **********/
        virtual int add_item(list_item::ptr item);  /// Appends a new Item to the listbox, returns the index of the object added
        virtual void remove_item(int index);  // removes the item at the specified index
        virtual int selected(); /// returns the index of the selected item
        virtual void selected(int index); /// sets the index of the selected item
        virtual list_item::ptr operator[](int index); /// returns the list_item at the specified location
        virtual void clear();   /// Erases the entire content of the list
        virtual int size();     /// Returns the number of elements in the list
        // list signals FIXME: what should the signature of the slots be ?
        boost::signal<bool (), combiner::logical_or<bool> > on_update_selection;
        boost::signal<bool (), combiner::logical_or<bool> > on_update_current;
        /********** END LIST INTERFACE **********/
    };
}

#endif
