/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Templated abstract list base class
 *     AUTHORS: Darren Hart, Marc Str�mke
 *  START DATE: 2003/Mar/03
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_LIST_TEMPLATE_H
#define STK_LIST_TEMPLATE_H

#include <iostream>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/bind.hpp>

#include <libstk/container.h>
#include "libstk/event.h"
#include "libstk/key_event.h"
#include "libstk/keycode.h"
#include "libstk/mouse_event.h"
#include "libstk/override_return.h"
#include <libstk/rectangle.h>
#include <libstk/widget.h>
#include <libstk/scroll_model.h>
#include <libstk/scrollable.h>

namespace stk
{
    /// \todo is widget right.. not a container ?
    /// Titem must be a derivative of list_item
    template<class Titem>
    class list_template : public widget, public scrollable
    {
    public:
        typedef boost::shared_ptr<list_template<Titem> > ptr;
        typedef boost::weak_ptr<list_template<Titem> > weak_ptr;

    private:

    protected:
        int current_;
        list_template(const rectangle& rect) : widget(rect), scrollable(), current_(0)
        {
            INFO("constructor");
            focusable(true);
            v_scroll_->vis_size(height());
            h_scroll_->vis_size(width());
        }
        std::vector<Titem> items_;

        // scrolling members
        boost::signals::connection v_scroll_con_;

    public:
        static list_template<Titem>::ptr create(container::ptr parent, const rectangle& rect)
        {
            list_template<Titem>::ptr new_list_template(new list_template<Titem>(rect));
            new_list_template->on_resize.connect(boost::function<bool()>(
                        (boost::bind(&scrollable::update_vis_sizes, new_list_template.get(), 
                                     boost::bind(&rectangle::height, new_list_template.get()), 
                                     boost::bind(&rectangle::width, new_list_template.get())), true)));
            new_list_template->parent(parent);
            return new_list_template;

        }

        virtual ~list_template() { INFO("destructor"); }

        /********** EVENT HANDLER INTERFACE **********/
        virtual void handle_event(event::ptr e)
        {
            // handle list_item selection and clicking here...
            switch (e->type())
            {
                case event::mouse_down:
                    {
                        mouse_event::ptr me = boost::shared_static_cast<mouse_event>(e);
                        int y = 0;
                        // FIXME: if CNTRL is pressed, don't deselect everything else
                        select_none();
                        for (current_ = 0; current_ < (int)items_.size(); current_++)
                        {
                            y += items_[current_]->height();
                            if (y > me->y()-rect_.y1()+v_scroll_->begin()) break;
                        }
                        on_update_current();
                        if ((unsigned int)current_ < items_.size()) // Crashes otherwise
                            items_[current_]->selected(true);
                        redraw(widget::rect());
                        return;
                        break;
                    }
                case event::key_down:
                    {
                        key_event::ptr ke = boost::shared_static_cast<key_event>(e);
                        switch ( ke->fn_key() )
                        {
                            case key_uparrow:
                                if (current_ > 0)
                                {
                                    current_--;
                                    if (items_[current_]->y1() < v_scroll_->begin())
                                        v_scroll_->begin(items_[current_]->y1());
                                    on_update_current();

                                    if (!(ke->modlist() & mod_control))
                                    {
                                        select_none();
                                        items_[current_]->selected(true);
                                    }
                                    on_update_selection();
                                }
                                else 
                                {
                                    if (current_ != 0) // superfluous ?
                                    {
                                        current_ = 0;
                                        on_update_current();
                                    }
                                    if (current() && !current()->selected())
                                    {
                                        current()->selected(true);
                                        on_update_selection();
                                    }
                                }
                                redraw(widget::rect());
                                return;
                                break;
                            case key_downarrow:
                                if (current_ < (int)items_.size()-1)
                                {
                                    current_++;
                                    if (items_[current_]->y2() >= v_scroll_->end())
                                        v_scroll_->begin(v_scroll_->begin()+items_[current_]->height());
                                    on_update_current();

                                    if (!(ke->modlist() & mod_control))
                                    {
                                        select_none();
                                        items_[current_]->selected(true);
                                    }
                                }
                                else 
                                {
                                    if (current_ != items_.size()-1) // superfluous ?
                                    {
                                        current_ = items_.size()-1;
                                        on_update_current();
                                    }
                                    if (current() && !current()->selected())
                                    {
                                        current()->selected(true);
                                        on_update_selection();
                                    }
                                }
                                redraw(widget::rect());
                                return;
                                break;
                            case key_enter:
                                if (!(ke->modlist() & mod_control)) select_none();
                                items_[current_]->selected(!items_[current_]->selected());
                                redraw(widget::rect());
                                return;
                                break;
                            default:
                                break;
                        }
                    }
            }
            widget::handle_event(e);
        }
        /********** END EVENT HANDLER INTERFACE **********/

        /********** DRAWABLE INTERFACE **********/
        virtual void draw(surface::ptr surface, const rectangle& clip_rect = rectangle()) = 0;
        /********** END DRAWABLE INTERFACE **********/

        /********** COMPONENT INTERFACE **********/
        /// \todo this would have to return a this pointer!!! unless we make list_items a widget and list a container!!!
        //virtual widget::ptr focus_next();
        //virtual widget::ptr focus_prev();
        /********** END COMPONENT INTERFACE **********/

        /********** SCROLLABLE INTERFACE **********/
        virtual void v_scroll(scroll_model::ptr model)
        {
            v_scroll_con_.disconnect();
            v_scroll_ = model;
            //v_scroll_con_ = model->on_change.connect(boost::bind(&widget::redraw, (widget*)this, widget::rect(), NULL, false));
            v_scroll_->size(height());
            v_scroll_->vis_size(height());
        }
        /********** END SCROLLABLE INTERFACE **********/

        /********** LIST INTERFACE **********/
        /// Appends a new item to the listbox, returns the index of the object added
        virtual int add_item(Titem item)
        {
            items_.push_back(item);
            // FIXME: we might as well return items_.size()-1 (since we always add it to the end)
            int index = std::find(items_.begin(), items_.end(), item) - items_.begin();
            if (index == current_) on_update_current();

            // assign the new item a rectangle (this depends on us adding item to the end)
            item->rect(rectangle(0, v_scroll_->size(), width(), item->height()));

            // adjust scroll properties
            if (h_scroll_->size() < item->width()) h_scroll_->size(item->width());
            v_scroll_->size(v_scroll_->size()+item->height());
            redraw(widget::rect());
            return index;
        }

        // removes the item at the specified index 
        virtual void remove_item(int index)
        {
            Titem item = items_.at(index);
            if (item)
            {
                items_.erase(items_.begin()+index);

                // Regenerate Item positionen
                v_scroll_->size(0);
                for (int i = 0; i < items_.size(); i++)
                {
                    Titem item = items_.at(i);
                    item->rect(rectangle(0, v_scroll_->size(), width(), items_[i]->height()));
                    v_scroll_->size(v_scroll_->size()+items_[i]->height());
                }
                current_ = MAX(0, MIN(current_, items_.size()-1));
                redraw(widget::rect());
            }
        }

        virtual std::vector<Titem> selection()
        {
            std::vector<Titem> selection_;
            for (unsigned int i = 0; i < items_.size(); i++) 
            {
                if (items_[i]->selected())
                    selection_.push_back(items_[i]);
            }
            return selection_;
        }

        virtual void select_none()
        {
            for (unsigned int i = 0; i < items_.size(); i++) items_[i]->selected(false);
        }

        virtual void select_all()
        {
            for (unsigned int i = 0; i < items_.size(); i++) items_[i]->selected(true);
        }

        /// returns the list_item at the specified location
        virtual Titem operator[](int index)
        {
            return items_.at(index);
        }

        /// Erases the entire content of the list
        virtual void clear()
        {
            bool sel_change = (selection().size() > 0);
            items_.clear();
            if (current_ != 0) on_update_current();
            current_ = 0;
            if (sel_change) on_update_selection();
            v_scroll_->size(0);
            redraw(widget::rect());
        }

        /// Returns the number of elements in the list
        virtual int size()
        {
            return items_.size();
        }

        /// Returns the current item or a null pointer
        virtual Titem current()
        {
            if (current_ < items_.size()) return items_[current_];
            return Titem(); 
        }

        /// Sets the current item, or the first on bad index
        virtual void current(int index)
        {
            if (current_ < items_.size()) items_[current_]->current(false);
            if (index >= 0 && index < items_.size())
            {
                current_ = index;
                items_[current_]->current(true);
            }
            else 
            {
                current_ = 0;
                if (current()) current()->current(true);
            }
            on_update_current();
        }

        // list signals FIXME: what should the signature of the slots be ?
        boost::signal<bool (), combiner::logical_or<bool> > on_update_selection;
        boost::signal<bool (), combiner::logical_or<bool> > on_update_current;
        /********** END LIST INTERFACE **********/
    };
}

#endif
