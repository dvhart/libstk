/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Edit box widget implementation
 *     AUTHORS: Dirk Hörner, Darren Hart, Chris Slade
 *  START DATE: 2003/Jun/23
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include <iostream>

#include "libstk/edit_box.h"
#include "libstk/container.h"
#include "libstk/event.h"
#include "libstk/key_event.h"
#include "libstk/font_manager.h"
#include "libstk/font.h"

namespace stk
{
    edit_box::ptr edit_box::create(container::ptr parent, const std::wstring& text, 
            const rectangle& rect)
    {
        edit_box::ptr new_edit_box(new edit_box(text, rect));
        new_edit_box->parent(parent);
        return new_edit_box;
    }

    edit_box::edit_box(const std::wstring& text, const rectangle& rect)
        : widget(rect),text_(text), selection_start_(0), selection_end_(0),
          pressed_(false), text_visible_(true)
    {
        INFO("constructor");
        focusable_ = true;
    }

    edit_box::~edit_box()
    {
        INFO("destructor");
    }

    // event handler
    void edit_box::handle_event(event::ptr e)
    {
        switch (e->type())
        {
        case event::key_down:
        {
            key_event::ptr ke = boost::shared_static_cast<key_event>(e);
            unsigned int sel_min = MIN(selection_end_, selection_start_);
            unsigned int sel_width = abs(selection_end_-selection_start_);
            switch ( ke->fn_key() )
            {
            case key_backspace:
                if (selection_end_ != selection_start_)
                {
                    text_.erase(sel_min, sel_width);
                    selection_start_ = selection_end_ = sel_min;
                    redraw(rect());
                    on_change(text_);
                }
                else if (selection_end_ > 0)
                {
                    sel_min--;
                    text_.erase(sel_min, 1);
                    selection_start_ = selection_end_ = sel_min;
                    redraw(rect());
                    on_change(text_);
                }
                
                return;
                break;
            case key_delete:
                if (selection_end_ != selection_start_)
                {
                    text_.erase(sel_min, sel_width);
                    selection_start_ = selection_end_ = sel_min;
                    redraw(rect());
                    on_change(text_);
                }
                else if (sel_min < text_.size())
                {
                    text_.erase(sel_min, 1);
                    redraw(rect());
                    on_change(text_);
                }
                return;
                break;
            case key_enter:
                redraw(rect());
                on_confirm(text_);
                return;
                break;
            case key_leftarrow:
                if (selection_end_ > 0) selection_end_--;
                if (!(ke->modlist() & mod_shift)) selection_start_ = selection_end_;
                redraw(rect());
                return;
                break;
            case key_rightarrow:
                if (selection_end_ < text_.size()) selection_end_++;
                if (!(ke->modlist() & mod_shift)) selection_start_ = selection_end_;
                redraw(rect());
                return;
                break;
            }

            // handle text keys 
            if (!ke->fn_key() && ke->character() != 0)
            {
                std::wstring insert_str;
                insert_str += ke->character();
                if (selection_end_ != selection_start_)
                {
                    text_.erase(sel_min, sel_width);
                    selection_start_ = selection_end_ = sel_min;
                }
                text_.insert(selection_end_, insert_str);
                selection_start_ = ++selection_end_;
                redraw(rect());
                on_change(text_);
                return;
            }
            else
                INFO("received unknown key: " << ke->fn_key());
            
            break; // key_down
        }
        case event::mouse_down:
        {
            mouse_event::ptr me = boost::shared_static_cast<mouse_event>(e);
            selection_start_ = selection_end_ = region(me->x(), me->y());
            pressed_ = true;
            redraw(rect());
            return;
            break; // mouse_down
        }
        case event::mouse_motion:
        {
            mouse_event::ptr me = boost::shared_static_cast<mouse_event>(e);            
            if (pressed_) 
            {
                selection_end_ = region(me->x(), me->y());
                redraw(rect());
            }
            return;
            break;
        }
        case event::mouse_up:
        {
            pressed_ = false;
            redraw(rect());
            return;
            break;
        }
        case event::mouse_leave:
        {
            pressed_ = false;
            redraw(rect());
            return;
            break;
        }
        }
        // if we haven't handled it, try the defaults and possibly pass it up
        widget::handle_event(e);
    }
    //public interface classes
    void edit_box::selection(int start, int end)
    {
        selection_start_ = start;
        selection_end_ = end;
        redraw(widget::rect());
    }
    
    edit_box::selection_pair edit_box::selection()
    {
        return selection_pair(selection_start_, selection_end_);
    }
    
    std::wstring edit_box::selected_text()
    {
        int start = MIN(selection_start_, selection_end_);
        int end = MAX(selection_start_, selection_end_);
        return text_.substr(start,end-start);
    }


}
