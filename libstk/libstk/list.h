/******************************************************************************
 *     CVS $Id$
 * DESCRIPTION: A listbox widget.  It contains a vector of list_items.
 *     AUTHORS: Darren Hart, Marc Strämke
 *  START DATE: 2003/Mar/03
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available
 *              with the source as license.txt or at 
 *              http://www.libstk.org/index.php?page=docs/license
 *****************************************************************************/

#ifndef STK_LIST_H
#define STK_LIST_H

#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <libstk/rectangle.h>
#include <libstk/widget.h>
#include <libstk/container.h>
#include <libstk/list_item.h>
#include <libstk/scroll_model.h>
#include <libstk/scrollable.h>

namespace stk
{
    /// \todo is widget right.. not a container ?
    class list : public widget, public scrollable
    {
    public:
        typedef boost::shared_ptr<list> ptr;
        typedef boost::weak_ptr<list> weak_ptr;

    private:

    protected:
        int current_;
        list(const rectangle& rect);
        std::vector<list_item::ptr> items_;

        // scrolling members
        boost::signals::connection v_scroll_con_;

    public:
        static list::ptr create(container::ptr parent, const rectangle& rect);
        virtual ~list();

        /********** RECTANGLE INTERFACE **********/
        // FIXME: every scrollable widget has to do this... is there another way?
        virtual void rect(const rectangle& rect) 
        {
            widget::rect(rect);
            h_scroll_->vis_size(width());
            v_scroll_->vis_size(height());
        }
        /********** END RECTANGLE INTERFACE **********/

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

        /********** SCROLLABLE INTERFACE **********/
        virtual void v_scroll(scroll_model::ptr value);
        /********** END SCROLLABLE INTERFACE **********/

        /********** LIST INTERFACE **********/
        /// Appends a new item to the listbox, returns the index of the object added
        virtual int add_item(list_item::ptr item);  
        virtual void remove_item(int index);  // removes the item at the specified index 
        virtual std::vector<list_item::ptr> selection();
        virtual void select_none();
        virtual void select_all();
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
