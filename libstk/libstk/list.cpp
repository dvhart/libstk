/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: List widget implementation.
 *     AUTHORS: Darren Hart, Marc Strämke
 *  START DATE: 2003/Mar/03
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include <iostream>
#include <boost/bind.hpp>

#include "libstk/list.h"
#include "libstk/event.h"
#include "libstk/key_event.h"
#include "libstk/mouse_event.h"
#include "libstk/keycode.h"
#include "libstk/override_return.h"

namespace stk
{
    list::ptr list::create(container::ptr parent, const rectangle& rect)
    {
        list::ptr new_list(new list(rect));
        new_list->on_resize.connect(boost::function<bool()>(
                (boost::bind(&scrollable::update_vis_sizes, new_list.get(), 
                    boost::bind(&rectangle::height, new_list.get()), 
                    boost::bind(&rectangle::width, new_list.get())), true)));
        new_list->parent(parent);
        return new_list;
    }

    list::list(const rectangle& rect) : widget(rect), scrollable(), current_(0)
    {
        INFO("constructor");
        focusable(true);
        v_scroll_->vis_size(height());
        h_scroll_->vis_size(width());
    }

    list::~list()
    {
        INFO("destructor");
    }

    void list::handle_event(event::ptr e)
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
                if (y > me->y()-y1()+v_scroll_->begin()) break;
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

    int list::add_item(list_item::ptr item)
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
    void list::remove_item(int index)
    {
        list_item::ptr item = items_.at(index);
        
        if (item)
        {
            items_.erase(items_.begin()+index);

            // Regenerate Item positionen
            v_scroll_->size(0);
            for (int i = 0; i < items_.size(); i++)
            {
                list_item::ptr item = items_.at(i);
                item->rect(rectangle(0, v_scroll_->size(), width(), items_[i]->height()));
                v_scroll_->size(v_scroll_->size()+items_[i]->height());
            }
            current_ = MAX(0, MIN(current_, items_.size()-1));
            redraw(widget::rect());
        }
    }

    std::vector<list_item::ptr> list::selection()
    {
        std::vector<list_item::ptr> selection_;
        for (unsigned int i = 0; i < items_.size(); i++) 
        {
            if (items_[i]->selected())
                selection_.push_back(items_[i]);
        }
        return selection_;
    }

    void list::select_none()
    {
        for (unsigned int i = 0; i < items_.size(); i++) items_[i]->selected(false);
    }
    
    void list::select_all()
    {
        for (unsigned int i = 0; i < items_.size(); i++) items_[i]->selected(true);
    }
    
    list_item::ptr list::operator[](int index)
    {
        return items_.at(index);
    }
    
    void list::clear()
    {
        bool sel_change = (selection().size() > 0);
        items_.clear();
        if (current_ != 0) on_update_current();
        current_ = 0;
        if (sel_change) on_update_selection();
        v_scroll_->size(0);
        redraw(widget::rect());
    }
    
    int list::size()
    {
        return items_.size();
    }
    
    list_item::ptr list::current()
    { 
        if (current_ < items_.size()) return items_[current_];
        return list_item::ptr(); 
    }

    void list::current(int index)
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

    void list::v_scroll(scroll_model::ptr model)
    {
        v_scroll_con_.disconnect();
        v_scroll_ = model;
        //v_scroll_con_ = model->on_change.connect(boost::bind(&widget::redraw, (widget*)this, widget::rect(), NULL, false));
        v_scroll_->size(height());
        v_scroll_->vis_size(height());
    }
}
