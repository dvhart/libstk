/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Default theme for Libstk.  Contains the user_theme class and 
 *              all concrete widget drawing routines.
 *     AUTHORS: Darren Hart, Marc Strämke, Chris Slade
 *  START DATE: 2003/Apr/27
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include "libstk/color_manager.h"
#include "libstk/exceptions.h"
#include "libstk/font_manager.h"
#include "libstk/graphics_context.h"
#include "libstk/point.h"
#include "libstk/theme.h"
#include "libstk/tribal_theme.h"

#include "libstk/button.h"
#include "libstk/edit_box.h"
#include "libstk/text_area.h"
#include "libstk/image_panel.h"
#include "libstk/label.h"
#include "libstk/list.h"
#include "libstk/list_item.h"
#include "libstk/numeric_spinner.h"
#include "libstk/progress.h"
#include "libstk/scroll_bar.h"
#include "libstk/scroll_box.h"
#include "libstk/spinner.h"
#include "libstk/spreadsheet.h"
#include "libstk/state.h"
#include "libstk/viewport.h"

// FIXME: store the colors we use here in user_theme rather than just the str

namespace stk
{
    // define the theme create constructor here (we need the user_theme header - tribal_theme.h)
    theme::ptr theme::create(surface::ptr surface)
    {
        instance_ = theme::ptr(new theme(surface));
        user_theme_ = boost::shared_ptr<user_theme>(new user_theme(surface));
        return instance_;
    }


    // define tribal user theme
    // FIXME: don't use surface, use the color manager to get colors (outside the constructor)
    user_theme::user_theme(surface::ptr surface)
    {
        INFO("constructor");
        // prepare our colors
        outline_color_normal_  = surface->gen_color(outline_color_normal_str);
        outline_color_focused_ = surface->gen_color(outline_color_focused_str);
        outline_color_hover_   = surface->gen_color(outline_color_hover_str);
        outline_color_pressed_ = surface->gen_color(outline_color_pressed_str);

        fill_state_color_      = surface->gen_color(fill_state_color_str);
        fill_color_normal_     = surface->gen_color(fill_color_normal_str);
        fill_color_focused_    = surface->gen_color(fill_color_focused_str);
        fill_color_hover_      = surface->gen_color(fill_color_hover_str);
        fill_color_pressed_    = surface->gen_color(fill_color_pressed_str);

        font_color_normal_     = surface->gen_color(font_color_normal_str);
        font_color_focused_    = surface->gen_color(font_color_focused_str);
        font_color_hover_      = surface->gen_color(font_color_hover_str);
        font_color_pressed_    = surface->gen_color(font_color_pressed_str);
    }

    user_theme::~user_theme()
    {
        INFO("destructor");
    }

    void user_theme::draw_arrow(int x, int y, int dir, surface::ptr surface, bool fill)
    {
        switch (dir)
        {
        case 3:
        {
            std::vector<point> arrow_points;
            arrow_points.push_back(point(x, y));
            arrow_points.push_back(point(x + 5, y + 5));
            arrow_points.push_back(point(x, y + 10));
	    if (fill) surface->fill_poly_aa(arrow_points);
	    else surface->draw_poly_aa(arrow_points);
            break;
        }
        case 6:
        {
            std::vector<point> arrow_points;
            arrow_points.push_back(point(x, y));
            arrow_points.push_back(point(x + 10, y));
            arrow_points.push_back(point(x + 5, y + 5));
	    if (fill) surface->fill_poly_aa(arrow_points);
	    else surface->draw_poly_aa(arrow_points);
            break;
        }
        case 9:
        {
            std::vector<point> arrow_points;
            arrow_points.push_back(point(x + 5, y));
            arrow_points.push_back(point(x, y + 5));
            arrow_points.push_back(point(x + 5, y + 10));
	    if (fill) surface->fill_poly_aa(arrow_points);
	    else surface->draw_poly_aa(arrow_points);
            break;
        }
        case 12:
        {
            std::vector<point> arrow_points;
            arrow_points.push_back(point(x, y + 5));
            arrow_points.push_back(point(x + 10, y + 5));
            arrow_points.push_back(point(x + 5, y));
	    if (fill) surface->fill_poly_aa(arrow_points);
	    else surface->draw_poly_aa(arrow_points);
            break;
        }

        default:
        throw error_message_exception("user_theme::draw_arrow - invalid direction");
        }
    }
    // end tribal user theme

    // define the various widget theme routines (draw(), region(), etc.)
    void state::draw(surface::ptr surface, const rectangle& clip_rect)
    {
        if (bg_image_)
        {
            surface->draw_image(0, 0, bg_image_);
        }
        else
        {
            graphics_context::ptr gc = graphics_context::create();
            gc->fill_color(color_manager::get()->get_color(
                        color_properties(fill_state_color_str, surface)));
            surface->gc(gc);
            surface->fill_rect(clip_rect);
        }
        container::draw(surface, clip_rect); // this will draw all the children - document this for theme howto
    }

    void button::draw(surface::ptr surface, const rectangle& clip_rect)
    {
        rectangle pressed_rect = rect(); pressed_rect.position(0, 0);
        rectangle interior_rect(3, 3, width()-6, height()-6);
        rectangle outline_rect(1, 1, width()-2, height()-2);

        graphics_context::ptr gc = graphics_context::create();

        // prepare the font

        font::ptr the_font = font_manager::get()->get_font(font_properties("Vera.ttf",18));
        gc->font(the_font);
        gc->font_fill_color(color_manager::get()->get_color(
                    color_properties(font_color_normal_str, surface)));

        if (pressed_)
        {
            gc->fill_color(color_manager::get()->get_color(
                        color_properties(fill_color_pressed_str, surface)));
            gc->line_color(color_manager::get()->get_color(
                        color_properties(outline_color_pressed_str, surface)));
        }
        else if(focused_)
        {
            gc->fill_color(color_manager::get()->get_color(
                        color_properties(fill_color_focused_str, surface)));
            if (hover_)
            {
                gc->line_color(color_manager::get()->get_color(
                            color_properties(outline_color_hover_str, surface)));
            }
            else
            {
                gc->line_color(color_manager::get()->get_color(
                            color_properties(outline_color_focused_str, surface)));
            }
        }
        else
        {
            gc->fill_color(color_manager::get()->get_color(
                        color_properties(fill_color_normal_str, surface)));
            if (hover_)
            {
                gc->line_color(color_manager::get()->get_color(
                            color_properties(outline_color_hover_str, surface)));
            }
            else
            {
                gc->line_color(color_manager::get()->get_color(
                            color_properties(outline_color_normal_str, surface)));
            }
        }

        // FIXME: I think we may be off by one in the rect draw/fill routines
        // draw the label for all states
        surface->gc(gc);
        surface->fill_rect(interior_rect);
        if (pressed_)
            surface->draw_rect(pressed_rect);
        surface->draw_rect(outline_rect);
        surface->draw_text(interior_rect, label_);
    }

    void progress::draw(surface::ptr surface, const rectangle& clip_rect)
    {
        graphics_context::ptr gc = graphics_context::create();
        gc->fill_color(color_manager::get()->get_color(
                    color_properties(fill_color_normal_str, surface)));
        gc->line_color(color_manager::get()->get_color(
                    color_properties(outline_color_normal_str, surface)));
        font::ptr Vera_18 = font_manager::get()->get_font(font_properties("Vera.ttf", 18));

        gc->font(Vera_18);
        gc->font_fill_color(color_manager::get()->get_color(
                    color_properties(font_color_normal_str, surface)));
        surface->gc(gc);
        rectangle progress_rect(2, 2, width()-4, height()-4);
        rectangle outline_rect = rect(); outline_rect.position(0, 0);
        progress_rect.width((int)(progress_rect.width()*percent()));
        surface->fill_rect(progress_rect);
        surface->draw_rect(outline_rect);
        surface->draw_text(outline_rect, label_);
    }

    void label::draw(surface::ptr surface, const rectangle& clip_rect)
    {
        graphics_context::ptr gc = graphics_context::create();
        gc->fill_color(color_manager::get()->get_color(
                    color_properties(fill_color_normal_str, surface)));
        gc->line_color(color_manager::get()->get_color(
                    color_properties(outline_color_normal_str, surface)));
        font::ptr Vera;
        if(text_size())
        {
            INFO("Drawing label with textsize=" << *text_size());
            Vera = font_manager::get()->get_font(font_properties("Vera.ttf", *text_size()));
        }
        else
            Vera = font_manager::get()->get_font(font_properties("Vera.ttf", 18));

        gc->font(Vera);
        gc->font_fill_color(color_manager::get()->get_color(
                    color_properties(font_color_normal_str, surface)));
        surface->gc(gc);
        rectangle outline_rect = rect(); outline_rect.position(0, 0);
        surface->draw_text(outline_rect, text_);
    }

    void image_panel::draw(surface::ptr surface, const rectangle& clip_rect)
    {
        surface->draw_image(0, 0, image_);
    }

    void list::draw(surface::ptr surface, const rectangle& clip_rect)
    {
        graphics_context::ptr gc = graphics_context::create();
        gc->line_color(color_manager::get()->get_color(
                    color_properties(outline_color_normal_str, surface)));
        if (focused_)
            gc->fill_color(color_manager::get()->get_color(
                        color_properties(fill_color_focused_str, surface)));
        else
            gc->fill_color(color_manager::get()->get_color(
                        color_properties(fill_color_normal_str, surface)));

        gc->font_fill_color(color_manager::get()->get_color(
                    color_properties(font_color_normal_str, surface)));
        font::ptr Vera_12 = font_manager::get()->get_font(font_properties("Vera.ttf", 12));
        gc->font(Vera_12);
        surface->gc(gc);

        // adjust surface offset to the vertical scroll_model
        surface->offset(surface->offset() - point(0, v_scroll_->begin()));
        
        // draw each visible item
        int y = 0;
        if (focused_ && current_ >= 0 && current_ < (int)items_.size()) 
            items_[current_]->current(true);
        for (unsigned int i = 0; i < items_.size(); i++)
        {
            if (y+items_[i]->height() > v_scroll_->begin() && y < v_scroll_->end())
            {
                // FIXME?: this clip_rect typically contains the whole list
                items_[i]->draw(surface, clip_rect);
            }
            y += items_[i]->height();
        }
        if (focused_ && current_ >= 0 && current_ < (int)items_.size()) 
            items_[current_]->current(false);
        
        // restore the surface offset
        surface->offset(surface->offset() + point(0, v_scroll_->begin()));

        // outline the list box
        rectangle outline_rect = rect(); outline_rect.position(0, 0);
        surface->draw_rect(outline_rect);
    }

    void list_item::draw(surface::ptr surface, const rectangle& clip_rect)
    {
        // draw list is responsible for setting the graphics context!!
        // FIXME: it shouldn't be!
        
        // FIXME: consider dropping the offsets IFF we change surface offset in list for every item
        rectangle label_rect(x1()+2, y1()+4, width()-4, height()-4);
        if (selected_) surface->fill_rect(rect());
        if (current_) surface->draw_rect(rect());
        surface->draw_text(label_rect, label_);
    }
    int list_item::height()
    {
        return 25;
    }

    void numeric_spinner::draw(surface::ptr surface, const rectangle& clip_rect)
    {
        rectangle interior_rect(3, 3, width()-6, height()-6);
        rectangle outline_rect(1, 1, width()-2, height()-2);
        rectangle pressed_rect = rect(); pressed_rect.position(0, 0);
        graphics_context::ptr gc = graphics_context::create();

        if(focused_)
        {
            gc->fill_color(color_manager::get()->get_color(
                        color_properties(fill_color_focused_str, surface)));
            if (hover_)
            {
                gc->line_color(color_manager::get()->get_color(
                            color_properties(outline_color_hover_str, surface)));
            }
            else
            {
                gc->line_color(color_manager::get()->get_color(
                            color_properties(outline_color_focused_str, surface)));
            }
        }
        else
        {
            gc->fill_color(color_manager::get()->get_color(
                        color_properties(fill_color_normal_str, surface)));
            if (hover_)
            {
                gc->line_color(color_manager::get()->get_color(
                            color_properties(outline_color_hover_str, surface)));
            }
            else
            {
                gc->line_color(color_manager::get()->get_color(
                            color_properties(outline_color_normal_str, surface)));
            }
        }

        gc->font_fill_color(color_manager::get()->get_color(
                    color_properties(font_color_normal_str, surface)));
        font::ptr Vera_18 = font_manager::get()->get_font(font_properties("Vera.ttf",18));
        gc->font(Vera_18);
        surface->gc(gc);

        // fill the spinner box
        surface->fill_rect(interior_rect);

        // outline the spinner box
        surface->draw_rect(outline_rect);
        if (pressed_)
            surface->draw_rect(pressed_rect);

        // draw the current value
        // WRITEME
        surface->draw_text(interior_rect, label_);

        // draw the arrows
	bool fill_down = wrap_ || (value_ > min_);
	bool fill_up = wrap_ || (value_ < max_);
        theme::user()->draw_arrow(width() - 15, 5, 12, surface, fill_up);
        theme::user()->draw_arrow(width() - 15, height()-10, 6, surface, fill_down);
    }
    int numeric_spinner::region(int x, int y)
    {
        if (x > x2()-15)
        {
            if (y > y1()+(height()/2)) return DOWN_ARROW;
            else return UP_ARROW;
        }
        return LABEL;
    }

    void scroll_bar::draw(surface::ptr surface, const rectangle& clip_rect)
    {
        INFO("scoll_bar::draw()");
        // translate model coordinates into scroll_bar coords
        rectangle outline_rect = rect(); outline_rect.position(0, 0);
        rectangle vis_rect = outline_rect; 
        INFO("  model begin: " << model_->begin() << ", vis_size" << model_->vis_size());
        if (width() > height())
        { // horizontal scrollbar
            // FIXME: the scrollbar move backwards!
            vis_rect.x1(width()*model_->begin()/model_->size());            
            vis_rect.x2(vis_rect.x1()+width()*model_->vis_size()/model_->size());            
        }
        else
        { // vertical scrollbar
            // FIXME: the scrollbar move backwards!
            vis_rect.y1(height()*model_->begin()/model_->size());            
            vis_rect.y2(vis_rect.y1() + height()*model_->vis_size()/model_->size());            
        }

        graphics_context::ptr gc = graphics_context::create();
        gc->line_color(color_manager::get()->get_color(
                    color_properties(outline_color_focused_str, surface)));
        surface->gc(gc);
        INFO("  drawing outline: " << outline_rect);
        surface->draw_rect(outline_rect);
        INFO("  drawing bar: " << vis_rect);
        surface->draw_rect(vis_rect);
    }
    
    void spinner::draw(surface::ptr surface, const rectangle& clip_rect)
    {
        rectangle interior_rect(3, 3, width()-6, height()-6);
        rectangle outline_rect(1, 1, width()-2, height()-2);
        rectangle pressed_rect = rect(); pressed_rect.position(0, 0);
        graphics_context::ptr gc = graphics_context::create();

        if(focused_)
        {
            gc->fill_color(color_manager::get()->get_color(
                        color_properties(fill_color_focused_str, surface)));
            if (hover_)
            {
                gc->line_color(color_manager::get()->get_color(
                            color_properties(outline_color_hover_str, surface)));
            }
            else
            {
                gc->line_color(color_manager::get()->get_color(
                            color_properties(outline_color_focused_str, surface)));
            }
        }
        else
        {
            gc->fill_color(color_manager::get()->get_color(
                        color_properties(fill_color_normal_str, surface)));
            if (hover_)
            {
                gc->line_color(color_manager::get()->get_color(
                            color_properties(outline_color_hover_str, surface)));
            }
            else
            {
                gc->line_color(color_manager::get()->get_color(
                            color_properties(outline_color_normal_str, surface)));
            }
        }

        gc->font_fill_color(color_manager::get()->get_color(
                    color_properties(font_color_normal_str, surface)));
        font::ptr Vera_18 = font_manager::get()->get_font(font_properties("Vera.ttf",18));
        gc->font(Vera_18);
        surface->gc(gc);

        // fill the spinner box
        surface->fill_rect(interior_rect);

        // outline the spinner box
        surface->draw_rect(outline_rect);
        if (pressed_)
            surface->draw_rect(pressed_rect);

        // draw the selected item's label
        // FIXME: this is major frellin' hideous stupid hackish kludge!
        surface->draw_text(interior_rect, items_[current_]->label());

        // draw the arrows
	bool fill_up = wrap_ || (current_ > 0);
	bool fill_down = wrap_ || (current_ < (int)items_.size()-1);
        theme::user()->draw_arrow(width()-15, 5, 12, surface, fill_up);
        theme::user()->draw_arrow(width()-15, height()-10, 6, surface, fill_down);
    }
    int spinner::region(int x, int y)
    {
        if (x > x2()-15)
        {
            if (y > y1()+(height()/2)) return DOWN_ARROW;
            else return UP_ARROW;
        }
        return LABEL;
    }

    void edit_box::draw(surface::ptr surface, const rectangle& clip_rect)
    {
        rectangle interior_rect(3, 3, width()-6, height()-6);
        rectangle outline_rect(1, 1, width()-2, height()-2);
        rectangle pressed_rect = rect(); pressed_rect.position(0, 0);

        graphics_context::ptr gc = graphics_context::create();

        // prepare the font
        font::ptr Vera_18 = font_manager::get()->get_font(font_properties("Vera.ttf",18));
        gc->font(Vera_18);
        gc->font_fill_color(color_manager::get()->get_color(
                    color_properties(font_color_normal_str, surface)));

        if (pressed_)
        {
            gc->fill_color(color_manager::get()->get_color(
                        color_properties(fill_color_pressed_str, surface)));
            gc->line_color(color_manager::get()->get_color(
                        color_properties(outline_color_pressed_str, surface)));
        }
        else if (focused_)
        {
            gc->fill_color(color_manager::get()->get_color(
                        color_properties(fill_color_focused_str, surface)));

            if (hover_)
            {
                gc->line_color(color_manager::get()->get_color(
                            color_properties(outline_color_hover_str, surface)));
            }
            else
            {
                gc->line_color(color_manager::get()->get_color(
                            color_properties(outline_color_focused_str, surface)));
            }
        }
        else
        {
            gc->fill_color(color_manager::get()->get_color(
                        color_properties(fill_color_normal_str, surface)));

            if (hover_)
            {
                gc->line_color(color_manager::get()->get_color(
                            color_properties(outline_color_hover_str, surface)));
            }
            else
            {
                gc->line_color(color_manager::get()->get_color(
                            color_properties(outline_color_normal_str, surface)));
            }
        }

        // FIXME: I think we may be off by one in the rect draw/fill routines
        // draw the label for all states
        surface->gc(gc);
        surface->fill_rect(interior_rect);

        if (pressed_) surface->draw_rect(pressed_rect);
        surface->draw_rect(outline_rect);

        // cursor and selection calculations
        int sel_min = MIN(selection_start_, selection_end_);
        std::wstring presel_str = text_.substr(0, sel_min);
        std::wstring sel_str = text_.substr(sel_min, abs(selection_end_-selection_start_));
        int sel_x = interior_rect.x1() + Vera_18->draw_len(presel_str);
        int sel_width = Vera_18->draw_len(sel_str);
        int cursor_x = (selection_start_ > selection_end_) ? sel_x : sel_x+sel_width;

        // draw the selection
        if (selection_start_ != selection_end_)
        {
            // yellow selection
            if (focused_)
                gc->fill_color(color_manager::get()->get_color(
                            color_properties(fill_color_normal_str, surface)));
            // light blue selection
            else
                gc->fill_color(color_manager::get()->get_color(
                            color_properties(fill_color_focused_str, surface)));

            surface->fill_rect(rectangle(sel_x, 3, sel_width, height()-6));
        }
        // draw the cursor if there is no selection and we are focused
        else if (focused_)
            surface->draw_line(cursor_x, 3, cursor_x, height()-3);
        
        // draw the string text_
        surface->draw_text(interior_rect, text_);
    }
    int edit_box::region(int x, int y)
    {
        font::ptr Vera_18 = font_manager::get()->get_font(font_properties("Vera.ttf",18));
        return Vera_18->chars_in_rect(rectangle(3, 0, x-x1(), y2()-y1()), text_);
    }

    void spreadsheet_cell::draw(surface::ptr surface, const rectangle& screen_position)
    {
        // FIXME: why is different than all the other widgets?
        if (screen_position.empty()) return;

        rectangle interior_rect(screen_position.x1()+3, screen_position.y1()+3, 
                screen_position.width()-6, screen_position.height()-6);
        rectangle outline_rect(screen_position.x1()+1, screen_position.y1()+1, 
                screen_position.width()-2, screen_position.height()-2);

        graphics_context::ptr gc = graphics_context::create();

        if(focused_)
        {
            gc->fill_color(color_manager::get()->get_color(
                        color_properties(fill_color_focused_str, surface)));
        }
        else
        {
            gc->fill_color(color_manager::get()->get_color(
                        color_properties(fill_color_normal_str, surface)));
        }
        gc->line_color(color_manager::get()->get_color(
                    color_properties(outline_color_normal_str, surface)));
        
        surface->fill_rect(interior_rect);
        surface->draw_rect(outline_rect);
    }
    void spreadsheet_cell_text::draw(surface::ptr surface, const rectangle& screen_position)
    {
        // FIXME: why is different than all the other widgets?
        if (screen_position.empty()) return;

        rectangle interior_rect(screen_position.x1()+3, screen_position.y1()+3, 
                screen_position.width()-6, screen_position.height()-6);
        rectangle outline_rect(screen_position.x1()+1, screen_position.y1()+1, 
                screen_position.width()-2, screen_position.height()-2);
        
        graphics_context::ptr gc = graphics_context::create();

        // prepare the font
        font::ptr the_font = font_manager::get()->get_font(font_properties("Vera.ttf", 10));
        
        gc->font(the_font);
        
        if (focused_)
        {
            gc->fill_color(color_manager::get()->get_color(
                        color_properties(fill_color_focused_str, surface)));
        }
        else
        {
            gc->fill_color(color_manager::get()->get_color(
                        color_properties(fill_color_normal_str, surface)));
        }
        gc->line_color(color_manager::get()->get_color(
                    color_properties(outline_color_normal_str, surface)));

        gc->font_outline_color(color_manager::get()->get_color(
                    color_properties(font_color_normal_str, surface)));
        gc->font_fill_color(color_manager::get()->get_color(
                    color_properties(font_color_normal_str, surface)));

        surface->gc(gc);

        surface->fill_rect(interior_rect);
        surface->draw_rect(outline_rect);
        surface->draw_text(interior_rect, text_);
    }

    void text_area::draw(surface::ptr surface, const rectangle& clip_rect)
    {
        rectangle interior_rect(3, 3, width()-6, height()-6);
        rectangle outline_rect(1, 1, width()-2, height()-2);
        rectangle pressed_rect = rect(); pressed_rect.position(0, 0);

        graphics_context::ptr gc = graphics_context::create();

        // prepare the font
        font::ptr Vera_14 = font_manager::get()->get_font(font_properties("Vera.ttf",14));
        gc->font(Vera_14);
        gc->font_fill_color(color_manager::get()->get_color(
                    color_properties(font_color_normal_str, surface)));

        if (pressed_)
        {
            gc->fill_color(color_manager::get()->get_color(
                        color_properties(fill_color_pressed_str, surface)));
            gc->line_color(color_manager::get()->get_color(
                        color_properties(outline_color_pressed_str, surface)));
        }
        else if (focused_)
        {
            gc->fill_color(color_manager::get()->get_color(
                        color_properties(fill_color_focused_str, surface)));

            if (hover_)
            {
                gc->line_color(color_manager::get()->get_color(
                            color_properties(outline_color_hover_str, surface)));
            }
            else
            {
                gc->line_color(color_manager::get()->get_color(
                            color_properties(outline_color_focused_str, surface)));
            }
        }
        else
        {
            gc->fill_color(color_manager::get()->get_color(
                        color_properties(fill_color_normal_str, surface)));

            if (hover_)
            {
                gc->line_color(color_manager::get()->get_color(
                            color_properties(outline_color_hover_str, surface)));
            }
            else
            {
                gc->line_color(color_manager::get()->get_color(
                            color_properties(outline_color_normal_str, surface)));
            }
        }

        // draw the label for all states
        surface->gc(gc);
        surface->fill_rect(interior_rect);

        if (pressed_) surface->draw_rect(pressed_rect);
        surface->draw_rect(outline_rect);

        // draw each line
        //variables to keep track of the lines
        int ypos = 3;
        bool cont = true;
        std::wstring line_str;
        std::wstring rest_of_text = text_;
        int line_width, nline_width, new_line;
        rectangle line_rect;
        int num_chars = 0;//counts the number of characters printed
        //variables for the selection/cursor
        int sel_min = MIN(selection_start_, selection_end_);
        int sel_max = MAX(selection_start_, selection_end_);
        int line_num = 0;
        while (cont) {
            line_rect = rectangle(3, ypos, width()-6, Vera_14->height()+3);
            //parse out the next line
            nline_width = rest_of_text.find(L'\n');
            line_width = Vera_14->chars_in_rect(line_rect, rest_of_text);
            if (nline_width != -1 && (nline_width <= line_width)) 
            {//if there is a new line in the line
                line_str = rest_of_text.substr(0, nline_width);

                rest_of_text = rest_of_text.substr(nline_width+1, 
                        rest_of_text.length()-nline_width-1);
                new_line = 1;//used to add in the new line character
            }
            else 
            {//no new line, just print it
                line_str = rest_of_text.substr(0, line_width);
                rest_of_text = rest_of_text.substr(line_width, rest_of_text.length()-line_width);
                new_line = 0;
            }

            //do cursor or selection
            if (selection_start_ == selection_end_ && focused_)
            {//nothing selected and we are focused -- print cursor
                if (selection_start_ >= num_chars && 
                        selection_start_ <= (num_chars+(int)line_str.length()))
                {//cursor is on this line
                    int chars_before_cursor = selection_start_ - num_chars;
                    int cursor_x = Vera_14->draw_len(line_str.substr(0,chars_before_cursor));
                    //draw cursor

                    if ( (chars_before_cursor != (int)line_str.length()) ||
                          (new_line) ||
                          ((num_chars+chars_before_cursor) == (int)text_.length()) ) 
                    {//if the cursor isn't at the end of the line
                     //or there is a new line
                     //or if it is at the very end of the text

                        surface->draw_line(line_rect.x1()+cursor_x, line_rect.y1()
                                ,line_rect.x1()+cursor_x,line_rect.y2());
                    }
                    line_ = line_num;//to remember where the cursor is

                }
            } else if (selection_start_ != selection_end_)
            {//draw selection
                //yellow selection
                if (focused_)
                    gc->fill_color(color_manager::get()->get_color(
                                color_properties(fill_color_normal_str, surface)));
                // light blue selection
                else
                    gc->fill_color(color_manager::get()->get_color(
                                color_properties(fill_color_focused_str, surface)));
                if ( (sel_min >= num_chars && sel_min <= (num_chars+(int)line_str.length()))
                        && (sel_max >= num_chars && sel_max <= (num_chars+(int)line_str.length())) )
                {//selection starts and ends on this line
                    int sel_start = sel_min - num_chars;
                    int sel_end = sel_max - num_chars;

                    int start_x = Vera_14->draw_len( line_str.substr(0, sel_start) );
                    int end_x = Vera_14->draw_len( line_str.substr(0, sel_end) );
                    rectangle sel_rect = rectangle(line_rect.x1()+start_x, line_rect.y1(), 
                            end_x-start_x, Vera_14->height()+3);
                    surface->fill_rect(sel_rect);
                    line_ = line_num;
                } 
                else if  ( sel_min >= num_chars && sel_min <= (num_chars+(int)line_str.length()) )
                {//selection starts on this line
                    int sel_start = sel_min - num_chars;
                    int start_x = Vera_14->draw_len ( line_str.substr(0, sel_start) );
                    int end_x = Vera_14->draw_len (line_str);//to end of text
                    rectangle sel_rect = rectangle(line_rect.x1()+start_x, line_rect.y1(), 
                            end_x-start_x, Vera_14->height()+3);
                    surface->fill_rect(sel_rect);
                    //alway use selection_end_ to determine current line
                    if (selection_end_ == sel_min) line_ = line_num;
                } 
                else if ( sel_max >= num_chars && sel_max <= (num_chars+(int)line_str.length()) )
                {//selection ends of this line
                    int sel_end = sel_max - num_chars;

                    int end_x = Vera_14->draw_len ( line_str.substr(0, sel_end) );
                    rectangle sel_rect = rectangle(line_rect.x1(), line_rect.y1(), end_x, 
                            Vera_14->height()+3);
                    surface->fill_rect(sel_rect);
                    if (selection_end_ ==  sel_max) line_ = line_num;
                } 
                else if ( sel_min < num_chars  && sel_max > num_chars+(int)line_str.length() )
                {//selection covers line
                    int end_x = Vera_14->draw_len(line_str);
                    rectangle sel_rect = rectangle(line_rect.x1(), line_rect.y1(), end_x, 
                            Vera_14->height()+3);
                    surface->fill_rect(sel_rect);
                }
            }
            num_chars += line_str.length()+new_line;
            //draw line
            surface->draw_text(line_rect, line_str );
            //move to next line
            ypos += Vera_14->height()+3;
            line_num++;
            //decide if you need to stop, past end of rectangle
            if (ypos+Vera_14->height()+3 >= interior_rect.y2())
                cont = false;
            //out of text
            if (rest_of_text.length() == 0 && !new_line)
            {//need to print the new line
                cont = false;
            }
        }
    }
    int text_area::region(int x, int y)
    {//finds where in the text the x,y is
        //create font
        font::ptr Vera_14 = font_manager::get()->get_font(font_properties("Vera.ttf",14));
        //pass through text to find the position
        int ypos = (y1()+3) + (Vera_14->height()+3);//the start of y plus a line
        bool cont = true;
        std::wstring line_str;
        std::wstring rest_of_text = text_;
        int line_width, nline_width, new_line;
        int num_chars = 0;
        while (cont) 
        {
            //parse out the next line
            nline_width = rest_of_text.find('\n');
            line_width = Vera_14->chars_in_rect(rectangle(3, 0, x2()-x1(), Vera_14->height()+6), rest_of_text);
            if (nline_width != -1 && (nline_width <= line_width)) 
            {//if there is a new line in the line
                line_str = rest_of_text.substr(0, nline_width);
                rest_of_text = rest_of_text.substr(nline_width+1, 
                        rest_of_text.length()-nline_width-1);
                new_line = 1;//used to add in the new line character to the total characters
            }
            else 
            {//no new line, just get the width
                line_str = rest_of_text.substr(0, line_width);
                rest_of_text = rest_of_text.substr(line_width, rest_of_text.length()-line_width);
                new_line = 0;
            }
            //if is on this line
            if (y <= ypos) {
                return num_chars + Vera_14->chars_in_rect(rectangle(3, 0, x-x1(), Vera_14->height()+6), line_str);
            }
            num_chars+=line_str.length()+new_line;
            //move to next line
            ypos += Vera_14->height()+3;
            //no more text
            if (rest_of_text.length() == 0)
                cont = false;
        }
        // trying to get the last character, just return the last character
        return text_.length();
    }
    int text_area::line_start_position(int line)
    {
        font::ptr Vera_14 = font_manager::get()->get_font(font_properties("Vera.ttf",14));
        //pass through text to find the position
        bool cont = true;
        std::wstring line_str;
        std::wstring rest_of_text = text_;
        int line_width, nline_width, new_line;
        int num_chars = 0;
        int line_num = 0; 
        while (cont) 
        {
            //parse out the next line
            nline_width = rest_of_text.find(L'\n');
            line_width = Vera_14->chars_in_rect(rectangle(3, 0, x2()-x1(), Vera_14->height()+6), rest_of_text);
            if (nline_width != -1 && (nline_width <= line_width)) 
            {//if there is a new line in the line
                line_str = rest_of_text.substr(0, nline_width);
                rest_of_text = rest_of_text.substr(nline_width+1, 
                        rest_of_text.length()-nline_width-1);
                new_line = 1;//used to add in the new line character to the total characters
            }
            else 
            {//no new line, just get the width
                line_str = rest_of_text.substr(0, line_width);
                rest_of_text = rest_of_text.substr(line_width, rest_of_text.length()-line_width);
                new_line = 0;
            }
            //if is on this line
            if (line_num == line)
                return num_chars;

            num_chars += line_str.length()+new_line;
            //move to next line
            line_num++;
            //stop if no more text
            if (rest_of_text.length() == 0)
                cont = false;
        }

        return text_.length();
    }
    int text_area::chars_in_line(int line)
    {
        font::ptr Vera_14 = font_manager::get()->get_font(font_properties("Vera.ttf",14));
        //pass through text to find the position
        bool cont = true;
        std::wstring line_str;
        std::wstring rest_of_text = text_;
        int line_width, nline_width, new_line;
        int num_chars = 0;
        int line_num = 0; 
        while (cont) 
        {
            //parse out the next line
            nline_width = rest_of_text.find(L'\n');
            line_width = Vera_14->chars_in_rect(rectangle(3, 0, x2()-x1(), Vera_14->height()+6), rest_of_text);
            if (nline_width != -1 && (nline_width < line_width)) 
            {//if there is a new line in the line
                line_str = rest_of_text.substr(0, nline_width);
                rest_of_text = rest_of_text.substr(nline_width+1, 
                        rest_of_text.length()-nline_width-1);
                new_line = 1;//used to add in the new line character to the total characters
            }
            else 
            {//no new line, just get the width
                line_str = rest_of_text.substr(0, line_width);
                rest_of_text = rest_of_text.substr(line_width, rest_of_text.length()-line_width);
                new_line = 0;
            }
            //if is on this line
            if (line_num == line)
                return line_str.length();

            num_chars+=line_str.length()+new_line;
            //move to next line
            line_num ++;
            //stop if no more text
            if (rest_of_text.length() == 0)
                cont = false;
        }

        return -1;

    }
 
    void viewport::draw(surface::ptr surface, const rectangle& clip_rect)
    {
        point scroll_offset(h_scroll()->begin(), v_scroll()->begin());
        rectangle t_rect = clip_rect;
        t_rect.position(t_rect.position() - scroll_offset);

        surface->offset(surface->offset() + scroll_offset);
        container::draw(surface, t_rect); // draw all the children
        surface->offset(surface->offset() - scroll_offset);
    }

}
