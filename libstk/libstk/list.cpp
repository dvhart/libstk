/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: List widget implementation.
 *     AUTHORS: Darren Hart, Marc Str�mke
 *  START DATE: 2003/Mar/03
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Str�mke, Dirk H�rner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include <iostream>
#include "libstk/list.h"
#include "libstk/event.h"
#include "libstk/key_event.h"
#include "libstk/mouse_event.h"
#include "libstk/keycode.h"

namespace stk
{
    list::ptr list::create(container::ptr parent, const rectangle& rect,
                           scroll_model::ptr v_scroll)
    {
        list::ptr new_list(new list(parent, rect, v_scroll));
        parent->add
        (new_list);
        return new_list;
    }

    list::list(container::ptr parent, const rectangle& rect,
               scroll_model::ptr v_scroll) : widget(parent, rect), current_(-1), selected_(0),
            prev_selected_(0), v_scroll_(v_scroll)
    {
        INFO("constructor");
        focusable(true);
        v_scroll_->vis_size(rect_.height());
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
            for (selected_ = 0; selected_ < items_.size(); selected_++)
            {
                y += items_[selected_]->height();
                if (y > me->y()-rect_.y1()) break;
            }
            current_ = selected_;
            redraw(rect_);
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
                        selected_ = current_;
                        on_update_selection();
                    }
                    redraw(rect_);
                }
                else current_ = 0;
                return;
                break;
            case key_downarrow:
                if (current_ < items_.size() - 1)
                {
                    current_++;
                    if (items_[current_]->y2() >= v_scroll_->end())
                        v_scroll_->begin(v_scroll_->begin()+items_[current_]->height());
                    if (!(ke->modlist() & mod_control)) selected_ = current_;
                    redraw(rect_);
                }
                else current_ = items_.size();
                return;
                break;
            case key_enter:
                selected_ = current_;
                redraw(rect_);
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

        // FIXME: the rect_.y1() should be removed from the item's y1 position, allowing
        //        the surface offset to handle that (children x,y relative to parent)
        item->rect(rectangle(rect_.x1(), v_scroll_->size(), 
                    rect_.width(), item->height()));
        
        // adjust scroll properties
        // FIXME: add width to the list_item API
        //if (h_scroll()->size() < item->width()) h_scroll()->size(item->width());
        v_scroll_->size(v_scroll_->size()+item->height());
        return index;
    }
    void list::remove_item(int index)
    {
        items_.erase(items_.begin()+index);
        redraw(rect_);
    }
    int list::selected()
    {
        return selected_;
    }
    void list::selected(int index)
    {
        selected_=index;
        redraw(rect_);
    }
    list_item::ptr list::operator[](int index)
    {
        return items_.at(index);
    }
    void list::clear()
    {
        items_.clear();
        redraw(rect_);
    }
    int list::size()
    {
        return items_.size();
    }
}
