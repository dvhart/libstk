/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: A Text edit box widget implementation
 *     AUTHORS: Chris Slade
 *  START DATE: 2003/Oct/3
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include <iostream>

#include "libstk/text_area.h"
#include "libstk/container.h"
#include "libstk/event.h"
#include "libstk/key_event.h"
#include "libstk/font_manager.h"
#include "libstk/font.h"

namespace stk
{
    text_area::ptr text_area::create(container::ptr parent, const std::wstring& text, 
            const rectangle& rect)
    {
        text_area::ptr new_text_area(new text_area(parent, text, rect));
        parent->add(new_text_area);
        return new_text_area;
    }

    text_area::text_area(container::ptr parent, const std::wstring& text, const rectangle& rect) 
        : widget(parent, rect), text_(text), selection_start_(0), selection_end_(0), 
          pressed_(false)
    {
        INFO("constructor");
        focusable(true);
    }

    text_area::~text_area()
    {
        INFO("destructor");
    }

    // event handler
    void text_area::handle_event(event::ptr e)
    {
        switch (e->type())
        {
        case event::key_down:
        {
            key_event::ptr ke = boost::shared_static_cast<key_event>(e);
            int sel_min = MIN(selection_end_, selection_start_);
            int sel_width = abs(selection_end_-selection_start_);
            int current_pos, new_pos;//for up and down
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
                else if (sel_min < (int)text_.size())
                {
                    text_.erase(sel_min, 1);
                    redraw(rect());
                    on_change(text_);
                }
                return;
                break;
            case key_enter:
                if (selection_end_ != selection_start_)
                {
                    text_.erase(sel_min, sel_width);
                    selection_start_ = selection_end_ = sel_min;
                }
                text_.insert(selection_end_, L"\n");
                selection_start_ = ++selection_end_;
                redraw(rect());
                on_change(text_);
                return;
                break;
            case key_leftarrow:
                if (selection_end_ > 0) selection_end_--;
                if (!(ke->modlist() & mod_shift)) selection_start_ = selection_end_;
                redraw(rect());
                return;
                break;
            case key_rightarrow:
                if (selection_end_ < (int)text_.size()) selection_end_++;
                if (!(ke->modlist() & mod_shift)) selection_start_ = selection_end_;
                redraw(rect());
                return;
                break;
            case key_uparrow:
                if (line_ == 0) //can't go up any more, ignore
                    return;
                else
                {
                    //position of cursor on current line
                    current_pos = selection_end_ - line_start_position(line_);
                    new_pos = MIN(chars_in_line(line_ - 1), current_pos);
                    selection_end_ = line_start_position(line_ - 1) + new_pos;
                }
                if (!(ke->modlist() & mod_shift)) selection_start_ = selection_end_;
                redraw(rect());
                return;
                break;
            case key_downarrow:
                //position of cursor on current line
                current_pos = selection_end_ - line_start_position(line_);
                new_pos = chars_in_line(line_ +1);
                if (new_pos == -1) //would move down to a non-existant line, ignore
                    return;
                new_pos = MIN(new_pos, current_pos);
                new_pos = line_start_position(line_ + 1) + new_pos;
                selection_end_ = new_pos;
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
            hover_ = false;
            redraw(rect());
            return;
            break;
        }
        }
        // if we haven't handled it, try the defaults and possibly pass it up
        widget::handle_event(e);
    }
}
