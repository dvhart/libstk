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
#include "libstk/list.h"
#include "libstk/event.h"
#include "libstk/key_event.h"
#include "libstk/mouse_event.h"
#include "libstk/keycode.h"

#include <boost/bind.hpp>

namespace stk
{
    list::ptr list::create(container::ptr parent, const rectangle& rect)
    {
        list::ptr new_list(new list(parent, rect));
        parent->add(new_list);
        return new_list;
    }

    list::list(container::ptr parent, const rectangle& rect) : widget(parent, rect), scrollable(),
        current_(0)
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
                if (y > me->y()-y1()) break;
            }
            if ((unsigned int)current_ < items_.size()) // Crashes otherwise
                items_[current_]->selected(true);
            redraw(rect());
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
                else current_ = 0;
                redraw(rect());
                return;
                break;
            case key_downarrow:
                if (current_ < (int)items_.size()-1)
                {
                    current_++;
                    if (items_[current_]->y2() >= v_scroll_->end())
                        v_scroll_->begin(v_scroll_->begin()+items_[current_]->height());
                    if (!(ke->modlist() & mod_control))
                    {
                        select_none();
                        items_[current_]->selected(true);
                    }
                }
                else current_ = items_.size()-1;
                redraw(rect());
                return;
                break;
            case key_enter:
                if (!(ke->modlist() & mod_control)) select_none();
                items_[current_]->selected(!items_[current_]->selected());
                redraw(rect());
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
        int index = std::find(items_.begin(), items_.end(), item) - items_.begin();

        // FIXME: the y1() should be removed from the item's y1 position, allowing
        //        the surface offset to handle that (children x,y relative to parent)
        item->rect(rectangle(0, v_scroll_->size(), width(), item->height()));
        
        // adjust scroll properties
        //  FIXME: implement horiz scrolling for lists
        //if (h_scroll_->size() < item->width()) h_scroll_->size(item->width());
        v_scroll_->size(v_scroll_->size()+item->height());
        redraw(rect());
        return index;
    }
    void list::remove_item(int index)
    {
        items_.erase(items_.begin()+index);
        redraw(rect());
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
        items_.clear();
        redraw(rect());
    }
    int list::size()
    {
        return items_.size();
    }
    
    void list::v_scroll(scroll_model::ptr value)
    {
        v_scroll_con_.disconnect();
        v_scroll_ = value;
//        v_scroll_con_=value->on_change.connect(boost::bind(&widget::redraw,(widget*)this,rect(),NULL,false));
        v_scroll_->size(height());
        v_scroll_->vis_size(height());
    }
}
