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
#include <boost/bind.hpp>

#include "libstk/text_area.h"
#include "libstk/container.h"
#include "libstk/event.h"
#include "libstk/key_event.h"
#include "libstk/font_manager.h"
#include "libstk/font.h"
#include "libstk/override_return.h"

namespace stk
{
    text_area::ptr text_area::create(container::ptr parent, const std::wstring& text, 
            const rectangle& rect, bool line_wrap, bool editable)
    {
        text_area::ptr new_text_area(new text_area(text, rect, line_wrap, editable));
        new_text_area->on_resize.connect(boost::function<bool()>(
                (boost::bind(&scrollable::update_vis_sizes, new_text_area.get(), 
                    boost::bind(&rectangle::height, 
                        boost::bind(&text_area::text_rectangle, new_text_area.get())),
                    boost::bind(&rectangle::width, 
                        boost::bind(&text_area::text_rectangle, new_text_area.get())) ), true)));
        new_text_area->on_resize.connect(boost::function<bool()>(
                    (boost::bind(&text_area::resize, new_text_area.get()), true)));
        new_text_area->parent(parent);
        new_text_area->on_resize();
        return new_text_area;
    }

    text_area::text_area(const std::wstring& text, const rectangle& rect, bool line_wrap, bool editable) : widget(rect), 
         text_(text), selection_start_(0), selection_end_(0), pressed_(false), line_wrap_(line_wrap), editable_(editable)
    {
        INFO("constructor");
        focusable(editable);
    }

    text_area::~text_area()
    {
        INFO("destructor");
    }
    void text_area::editable(bool edit)
    {
        focusable(edit);
        editable_ = edit;
    }
    void text_area::resize()
    {
        if (line_wrap_)
        {
            v_scroll_->size(MAX(total_lines()*(get_font()->height()+line_spacing()), 
                        v_scroll_->vis_size()));
            h_scroll_->size(h_scroll_->vis_size());
        }
        else
        {
            //resize in one sweep so we don't parse through text twice.
            //i.e don't use total_lines()
            int scr = h_scroll_->begin();//remember scroll
            int line_cnt = 0;
            rest_of_text_ = text_;
            std::wstring line_str;
            //reset the width
            h_scroll_->size(h_scroll_->vis_size());
            while(rest_of_text_.length()||new_line_)
            {
                line_str = next_line();
                if (get_font()->draw_len(line_str) > h_scroll_->size())
                    h_scroll_->size(get_font()->draw_len(line_str));
                line_cnt++;
            }
            v_scroll_->size(MAX( (line_cnt+0)*(get_font()->height()+line_spacing()), 
                            v_scroll_->vis_size()));
            h_scroll_->begin(scr);
        }
        scroll();
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
                    resize();
                    redraw(widget::rect());
                    on_change(text_);
                }
                else if (selection_end_ > 0)
                {
                    sel_min--;
                    text_.erase(sel_min, 1);
                    selection_start_ = selection_end_ = sel_min;
                    resize();
                    redraw(widget::rect());
                    on_change(text_);
                }
                
                return;
                break;
            case key_delete:
                if (selection_end_ != selection_start_)
                {
                    text_.erase(sel_min, sel_width);
                    selection_start_ = selection_end_ = sel_min;
                    resize();
                    redraw(widget::rect());
                    on_change(text_);
                }
                else if (sel_min < (int)text_.size())
                {
                    text_.erase(sel_min, 1);
                    resize();
                    redraw(widget::rect());
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
                //make the size bigger
                resize();
                redraw(widget::rect());
                on_change(text_);
                return;
                break;
            case key_leftarrow:
                if (selection_end_ > 0) selection_end_--;
                if (!(ke->modlist() & mod_shift)) selection_start_ = selection_end_;
                scroll();
                redraw(widget::rect());
                return;
                break;
            case key_rightarrow:
                if (selection_end_ < (int)text_.size()) selection_end_++;
                if (!(ke->modlist() & mod_shift)) selection_start_ = selection_end_;
                scroll();
                redraw(widget::rect());
                return;
                break;
            case key_uparrow:
                if (line_ == 0)
                { //can't go up any more, ignore
                    return;
                } else 
                {
                    //position of cursor on current line
                    current_pos = selection_end_ - line_start_position(line_);
                    new_pos = MIN(chars_in_line(line_ - 1), current_pos);
                    selection_end_ = line_start_position(line_ - 1) + new_pos;
                    scroll();
                    
                }
                if (!(ke->modlist() & mod_shift)) selection_start_ = selection_end_;
                redraw(widget::rect());
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
                scroll();
                redraw(widget::rect());                
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
                resize();
                redraw(widget::rect());
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
            redraw(widget::rect());
            return;
            break; // mouse_down
        }
        case event::mouse_motion:
        {
            mouse_event::ptr me = boost::shared_static_cast<mouse_event>(e);            
            if (pressed_) 
            {
                selection_end_ = region(me->x(), me->y());
                redraw(widget::rect());
            }
            return;
            break;
        }
        case event::mouse_up:
        {
            pressed_ = false;
            redraw(widget::rect());
            return;
            break;
        }
        case event::mouse_leave:
        {
            pressed_ = false;
            hover_ = false;
            redraw(widget::rect());
            return;
            break;
        }
        }
        // if we haven't handled it, try the defaults and possibly pass it up
        widget::handle_event(e);
    }
    point text_area::draw_text(surface::ptr surface, const rectangle& clip_rect)
    {
        
        point temp = cursor_position();
        font::ptr font = get_font();
        int spacing = line_spacing();
        rectangle text_rect = text_rectangle();
        //set cursor
        point cursor_pos = point(-1, -1);//-1 means that you don't draw the cursor
        if (!text_.length())//no text, draw cursor at begining
            cursor_pos = point(text_rect.x1(), text_rect.y1());
        // variables to keep track of the lines
        int ypos = 0, num_chars = 0, y = 0;
        std::wstring line_str, print_str;
        rectangle line_rect;
        // variables for the selection/cursor
        int sel_min = MIN(selection_start_, selection_end_);
        int sel_max = MAX(selection_start_, selection_end_);
        int line_num = 0;
        //intialize next_line
        rest_of_text_ = text_;
        //skip all of the lines before begin
        while (ypos < v_scroll_->begin() && (rest_of_text_.length() || new_line_))
        {
            line_str = next_line();
            // move to next line
            num_chars += line_str.length()+new_line_;
            ypos += font->height()+spacing;
            line_num++;
        }
        //while still in v_scroll_ end and there is text to draw
        while ((rest_of_text_.length() || new_line_) && ypos+font->height()+spacing <= v_scroll_->end()) 
        {
            line_rect = rectangle(text_rect.x1(), text_rect.y1()+y, text_rect.x2()-text_rect.x1(), font->height()+spacing);
            // parse out the next line
            line_str = next_line();
            //figure out how much we have to cut off do to scrolling
            int cut_chars = font->chars_in_rect(rectangle(0,0, h_scroll_->begin(), font->height()), line_str);
            if (h_scroll_->begin() && cut_chars != (int)line_str.length()) cut_chars++;
            print_str = line_str.substr(cut_chars, line_str.length());
            // draw cursor or selection
            if (selection_start_ != selection_end_)
            {// draw selection

                if ( (sel_min >= num_chars && sel_min <= (num_chars+(int)line_str.length()))
                        && (sel_max >= num_chars && sel_max <= (num_chars+(int)line_str.length())) )
                {// selection starts and ends on this line
                    int sel_start = sel_min - num_chars;
                    int sel_end = sel_max - num_chars;

                    int start_x = (sel_start >= cut_chars) ? 
                        font->draw_len(line_str.substr(cut_chars, sel_start-cut_chars)) : text_rect.x1();
                    int end_x = (sel_end >= cut_chars) ? 
                        font->draw_len(line_str.substr(cut_chars, sel_end-cut_chars)) : text_rect.x1();
                    rectangle sel_rect = rectangle(line_rect.x1()+start_x, line_rect.y1(), 
                            end_x-start_x, font->height()+spacing);
                    surface->fill_rect(sel_rect);
                    line_ = line_num;
                } 
                else if  ( sel_min >= num_chars && sel_min <= (num_chars+(int)line_str.length()) )
                {// selection starts on this line
                    int sel_start = sel_min - num_chars;
                    int start_x = (sel_start >= cut_chars) ?
                        font->draw_len(line_str.substr(cut_chars, sel_start-cut_chars)) : text_rect.x1();
                    int end_x = MIN(font->draw_len(print_str), text_rect.x2());//to end of text
                    rectangle sel_rect = rectangle(line_rect.x1()+start_x, line_rect.y1(), 
                            end_x-start_x, font->height()+spacing);
                    surface->fill_rect(sel_rect);
                    // alway use selection_end_ to determine current line
                    if (selection_end_ == sel_min) line_ = line_num;
                } 
                else if ( sel_max >= num_chars && sel_max <= (num_chars+(int)line_str.length()) )
                {// selection ends of this line
                    int sel_end = sel_max - num_chars;

                    int end_x = (sel_end >= cut_chars) ?
                        font->draw_len(line_str.substr(cut_chars, sel_end-cut_chars)) : text_rect.x1();
                    rectangle sel_rect = rectangle(line_rect.x1(), line_rect.y1(), end_x, 
                            font->height()+spacing);
                    surface->fill_rect(sel_rect);
                    if (selection_end_ ==  sel_max) line_ = line_num;
                } 
                else if ( sel_min < num_chars  && sel_max > num_chars+(int)line_str.length() )
                {// selection covers line
                    int end_x = MIN(font->draw_len(print_str), text_rect.x2());
                    rectangle sel_rect = rectangle(line_rect.x1(), line_rect.y1(), end_x, 
                            font->height()+spacing);
                    surface->fill_rect(sel_rect);
                }
            } 
            // set the cursor_pos so the theme can draw it
            // and set the line number, so we know where the cursor is
            if ( selection_end_ >= num_chars &&
                    selection_end_ <= (num_chars+(int)line_str.length()) )
            {
                //if the cursor (selection_end_) is in this line
                int chars_before_cursor = selection_end_ - num_chars;
                //if it is in the middle of the line, or at end of line with a new line,
                // or it is at the end of the text
                if ((chars_before_cursor != (int)line_str.length()) ||    
                      new_line_ || ((num_chars+chars_before_cursor) == (int)text_.length()))
                { 
                    chars_before_cursor = (chars_before_cursor >= cut_chars) ? 
                        chars_before_cursor-cut_chars : 0;
                    int cursor_x = font->draw_len(print_str.substr(0, chars_before_cursor));
                    cursor_pos = point(cursor_x+line_rect.x1(), line_rect.y1());
                    line_ = line_num; // to remember where the cursor is
                }
               
            }  
            // draw line
            surface->draw_text(line_rect, print_str );
            // move to next line
            num_chars += line_str.length()+new_line_;
            ypos += font->height()+spacing;
            y += font->height()+spacing;
            line_num++;
        } // while
        //restore surface offset
        
        return cursor_pos;
    }
    
    std::wstring text_area::next_line()
    {
        std::wstring line_str;
        font::ptr font = get_font();
        rectangle text_rect = text_rectangle();
        int line_width = font->chars_in_rect(rectangle(0, 0, text_rect.x2()-text_rect.x1(),
                    font->height()+3), rest_of_text_);
        int nline_width = rest_of_text_.find(L'\n');
        if (!line_wrap_ && nline_width == -1)
        {
            line_width = rest_of_text_.length();
            new_line_ = 0;
        }
        else if ((nline_width  >= 0 && nline_width<line_width) || !line_wrap_) 
        {
            line_width = nline_width;
            new_line_ = 1;
        }
        else
        {
            //go to the last whitespace
            new_line_ = 0;
            if ((int)rest_of_text_.length() != line_width) 
            {   //is not at end of text
                int temp_width = rest_of_text_.rfind(L" ",line_width);
                line_width = (temp_width > 0) ? temp_width+1: line_width;
            }
        }
        line_str = rest_of_text_.substr(0, line_width);
        rest_of_text_ = rest_of_text_.substr(line_width+new_line_, 
                rest_of_text_.length()-line_width-new_line_);
        //go back and divide the line by the closest white space
        return line_str;
    }
    
    int text_area::region(int x, int y)
    {
        rectangle text_rect = text_rectangle();
        if (x-rect_.x1() < text_rect.x1() || x-rect_.x1() > text_rect.x2() ||
                y-rect_.y1() < text_rect.y1() || y-rect_.y1() > text_rect.y2())
        {
            //clicked outside of text area
            return selection_end_; //make it be the last position
        }
        //map coordinates
        int adjusted_y = v_scroll_->begin()+y;
        int adjusted_x = h_scroll_->begin()+x;

        font::ptr font = get_font();
        // pass through text to find the position
        // the start of y plus a line
        int ypos = (rect_.y1()+text_rect.y1()) + (font->height()+line_spacing()); 
        std::wstring line_str;
        //intialize next_line
        rest_of_text_ = text_;
        int num_chars = 0;
        while (rest_of_text_.length() || new_line_) 
        {
            // parse out the next line
            line_str = next_line();
            // if is on this line
            if (adjusted_y <= ypos) 
            {
                return num_chars + font->chars_in_rect(rectangle(text_rect.x1(), 0, 
                            adjusted_x-rect_.x1()-text_rect.x1(), font->height()+6), line_str);
            }
            num_chars+=line_str.length()+new_line_;
            // move to next line
            ypos += font->height()+line_spacing();
        }
        // trying to get the last character, just return the last character
        return text_.length();
    }
    
    int text_area::line_start_position(int line)
    {
        font::ptr font = get_font();
        // pass through text to find the position
        std::wstring line_str;
        //initialize next_line
        rest_of_text_ = text_;
        int num_chars = 0;
        int line_num = 0; 
        while (rest_of_text_.length() || new_line_) 
        {
            // parse out the next line
            line_str = next_line();
            // if is on this line
            if (line_num == line)
                return num_chars;

            num_chars += line_str.length()+new_line_;
            // move to next line
            line_num++;
        }

        return text_.length();
    }
    
    int text_area::chars_in_line(int line)
    {
        font::ptr font = get_font();
        //pass through text to find the position
        std::wstring line_str;
        //initialize next_linw
        std::wstring rest_of_text_ = text_;
        int line_num = 0; 
        while (rest_of_text_.length() || new_line_) 
        {
            // parse out the next line
            line_str = next_line();
            // if is on this line
            if (line_num == line)
                return line_str.length();
            //move to next line
            line_num++;
        }
        //there is no next_line
        return -1;
    }
    
    int text_area::total_lines()
    {
        int line_num = 0;
        rest_of_text_ = text_;
        while(rest_of_text_.length() || new_line_)
        {
            next_line();
            line_num++;
        }
        return line_num;
    }
    
    point text_area::cursor_position()
    {
        int line_num = 0;
        int num_chars = 0;
        std::wstring line_str;
        rest_of_text_=text_;
        while(rest_of_text_.length() || new_line_)
        {
            line_str = next_line();
            if ( selection_end_ >= num_chars &&
                    selection_end_ <= (num_chars+(int)line_str.length()) )
            {
                int chars_before_cursor = selection_end_ - num_chars;
                //if it is in the middle of the line, or at end of line with a new line,
                // or it is at the end of the text
                if ((chars_before_cursor != (int)line_str.length()) ||    
                      new_line_ || ((num_chars+chars_before_cursor) == (int)text_.length()))
                {
                    int ypos = line_num*(get_font()->height()+line_spacing());
                    int xpos = get_font()->draw_len(line_str.substr(0, selection_end_-num_chars));
                    return point(xpos,ypos);
                }
            }
            num_chars += line_str.length()+new_line_;
            line_num++;
        }
        //if at very end of the text
        if (selection_end_ == (int)text_.length())
        {
            int ypos = (line_num)*(get_font()->height()+line_spacing());
            int xpos = get_font()->draw_len(line_str);
            return point(xpos,ypos);
        }
        return point(0,0);
    }
    
    void text_area::scroll()
    {
        //get position of cursor
        point cur_pos = cursor_position();
        //scroll y
        if (cur_pos.y() < v_scroll_->begin())
        {
            //need to scroll up
            v_scroll_->begin(cur_pos.y());
        }
        else if ((cur_pos.y()+get_font()->height()+line_spacing()) > v_scroll_->end())
        {
            //need to scroll down a line
            //bottom of the line minus the vis_size because we are setting the top
            v_scroll_->begin(cur_pos.y()+get_font()->height()+line_spacing() - v_scroll_->vis_size());
        }
        //scroll x
        if (cur_pos.x() < h_scroll_->begin())
        {
            //need to scroll left
            h_scroll_->begin(cur_pos.x());
        }
        else if (cur_pos.x() > h_scroll_->end())
        {
            //need to scroll right
            h_scroll_->begin(cur_pos.x()-h_scroll_->vis_size());
        }
    }
    //public interface classes
    void text_area::selection(int start, int end)
    {
        selection_start_ = start;
        selection_end_ = end;
        redraw(widget::rect());
    }
    text_area::selection_pair text_area::selection()
    {
        return selection_pair(selection_start_, selection_end_);
    }
    std::wstring text_area::selected_text()
    {
        int start = MIN(selection_start_, selection_end_);
        int end = MAX(selection_start_, selection_end_);
        return text_.substr(start,end-start);
    }
    void text_area::v_scroll(scroll_model::ptr model)
    {
        v_scroll_con_.disconnect();
        v_scroll_ = model;
        v_scroll_->size(height());
        resize();
    }
    void text_area::h_scroll(scroll_model::ptr model)
    {
        h_scroll_con_.disconnect();
        h_scroll_ = model;
        h_scroll_->size(height());
        resize();
    }
}
