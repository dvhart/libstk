/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Default theme for Libstk.  Contains the user_theme class and 
 *              all themeable widget drawing routines.
 *     AUTHORS: Darren Hart, Marc Straemke
 *  START DATE: 2003/Apr/27
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *     license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include "libstk/color_manager.h"
#include "libstk/graphics_context.h"
#include "libstk/exceptions.h"
#include "libstk/font_manager.h"

#include "libstk/point.h"
#include "libstk/button.h"
#include "libstk/image_panel.h"
#include "libstk/label.h"
#include "libstk/list.h"
#include "libstk/list_item.h"
#include "libstk/numeric_spinner.h"
#include "libstk/progress.h"
#include "libstk/scroll_box.h"
#include "libstk/spinner.h"
#include "libstk/state.h"
#include "libstk/theme.h"
#include "libstk/tribal_theme.h"
#include "libstk/edit_box.h"

// all draw routines go here with the exception of container as it isn't
// themeable and only draws its children

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
        surface->clip_rect(clip_rect.empty() ? rect_ : clip_rect);
        graphics_context::ptr gc = graphics_context::create();
        gc->fill_color(color_manager::get()->get_color(
                    color_properties(fill_state_color_str, surface)));
        surface->gc(gc);
        surface->fill_rect(rect_);
        container::draw(surface); // this will draw all the children - document this for theme howto
    }

    void button::draw(surface::ptr surface, const rectangle& clip_rect)
    {
        surface->clip_rect(clip_rect.empty() ? rect_ : clip_rect);

        rectangle interior_rect(rect_.x1()+3, rect_.y1()+3, rect_.width()-6, rect_.height()-6);
        rectangle outline_rect(rect_.x1()+1, rect_.y1()+1, rect_.width()-2, rect_.height()-2);

        graphics_context::ptr gc = graphics_context::create();

        // prepare the font

        font::ptr the_font = font_manager::get()->get_font(font_properties("Arial.ttf",18));
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
            surface->draw_rect(rect_);
        surface->draw_rect(outline_rect);
        surface->draw_text(interior_rect, label_);
    }

    void progress::draw(surface::ptr surface, const rectangle& clip_rect)
    {
        surface->clip_rect(clip_rect.empty() ? rect_ : clip_rect);
        graphics_context::ptr gc = graphics_context::create();
        gc->fill_color(color_manager::get()->get_color(
                    color_properties(fill_color_normal_str, surface)));
        gc->line_color(color_manager::get()->get_color(
                    color_properties(outline_color_normal_str, surface)));
        font::ptr arial_18 = font_manager::get()->get_font(font_properties("Arial.ttf", 18));

        gc->font(arial_18);
        gc->font_fill_color(color_manager::get()->get_color(
                    color_properties(font_color_normal_str, surface)));
        surface->gc(gc);
        rectangle progress_rect(rect_.x1()+2, rect_.y1()+2, rect_.width()-4, rect_.height()-4);
        progress_rect.width((int)(progress_rect.width()*percent()));
        surface->fill_rect(progress_rect);
        surface->draw_rect(rect_);
        surface->draw_text(rect_, label_);
    }

    void label::draw(surface::ptr surface, const rectangle& clip_rect)
    {
        surface->clip_rect(clip_rect.empty() ? rect_ : clip_rect);
        graphics_context::ptr gc = graphics_context::create();
        gc->fill_color(color_manager::get()->get_color(
                    color_properties(fill_color_normal_str, surface)));
        gc->line_color(color_manager::get()->get_color(
                    color_properties(outline_color_normal_str, surface)));
        font::ptr arial_18 = font_manager::get()->get_font(font_properties("Arial.ttf", 18));
        gc->font(arial_18);
        gc->font_fill_color(color_manager::get()->get_color(
                    color_properties(font_color_normal_str, surface)));
        surface->gc(gc);
        surface->draw_text(rect_, text_);
    }

    void image_panel::draw(surface::ptr surface, const rectangle& clip_rect)
    {
        surface->clip_rect(clip_rect.empty() ? rect_ : clip_rect);
        graphics_context::ptr gc = graphics_context::create();
        gc->line_color(color_manager::get()->get_color(
                    color_properties(outline_color_focused_str, surface)));
        surface->gc(gc);
        //surface->draw_rect(rect_);
        //surface->draw_image(rect_.x1()+10, rect_.y1()+10, image_);
        surface->draw_image(rect_.x1(), rect_.y1(), image_);
    }

    void list::draw(surface::ptr surface, const rectangle& clip_rect)
    {
        surface->clip_rect(clip_rect.empty() ? rect_ : clip_rect);

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
        font::ptr arial_12 = font_manager::get()->get_font(font_properties("Arial.ttf", 12));
        gc->font(arial_12);
        surface->gc(gc);

        // adjust the list items using the vertical scroll_model
        surface->offset(surface->offset() - point(0, v_scroll_->begin()-rect_.y1()));

        // draw each item
        int y = 0;
        if (selected_ >= 0 && selected_ < items_.size()) items_[selected_]->selected(true);
        if (current_ >= 0 && current_ < items_.size()) items_[current_]->current(true);
        for (int i = 0; i < items_.size(); i++)
        {
            if (y+items_[i]->height() > v_scroll_->begin() && y < v_scroll_->end())
            {
                // FIXME: the clip_rect setting in list_item doesn't account for offset
                items_[i]->draw(surface, surface->clip_rect());
            }
            y += items_[i]->height();
        }
        if (selected_ >= 0 && selected_ < items_.size()) items_[selected_]->selected(false);
        if (current_ >= 0 && current_ < items_.size()) items_[current_]->current(false);
        
        surface->offset(surface->offset() + point(0, v_scroll_->begin()-rect_.y1()));

        // outline the list box
        surface->draw_rect(rect_);
    }

    void list_item::draw(surface::ptr surface, const rectangle& clip_rect)
    {
        // FIXME: the rect doesn't account for offset
        surface->clip_rect(clip_rect.empty() ? rect_ : clip_rect);
        
        // draw list is responsible for setting the graphics context!!
        // FIXME: it shouldn't be!
        
        rectangle label_rect(rect_.x1()+2, rect_.y1()+4, rect_.width()-4, rect_.height()-4);
        if (selected_) surface->fill_rect(rect_);
        if (current_) surface->draw_rect(rect_);
        surface->draw_text(label_rect, label_);
    }
    int list_item::height()
    {
        return 25;
    }

    void numeric_spinner::draw(surface::ptr surface, const rectangle& clip_rect)
    {
        surface->clip_rect(clip_rect.empty() ? rect_ : clip_rect);
        rectangle interior_rect(rect_.x1()+3, rect_.y1()+3, rect_.width()-6, rect_.height()-6);
        rectangle outline_rect(rect_.x1()+1, rect_.y1()+1, rect_.width()-2, rect_.height()-2);
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
        font::ptr arial_18 = font_manager::get()->get_font(font_properties("Arial.ttf",18));
        gc->font(arial_18);
        surface->gc(gc);

        // fill the spinner box
        surface->fill_rect(interior_rect);

        // outline the spinner box
        surface->draw_rect(outline_rect);
        if (pressed_)
            surface->draw_rect(rect_);

        // draw the current value
        // WRITEME
        surface->draw_text(interior_rect, label_);

        // draw the arrows
	bool fill_down = wrap_ || (value_ > min_);
	bool fill_up = wrap_ || (value_ < max_);
        theme::user()->draw_arrow(x2() - 15, y1() + 5, 12, surface, fill_up);
        theme::user()->draw_arrow(x2() - 15, y2() - 10, 6, surface, fill_down);
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


    
    void spinner::draw(surface::ptr surface, const rectangle& clip_rect)
    {
        surface->clip_rect(clip_rect.empty() ? rect_ : clip_rect);
        rectangle interior_rect(rect_.x1()+3, rect_.y1()+3, rect_.width()-6, rect_.height()-6);
        rectangle outline_rect(rect_.x1()+1, rect_.y1()+1, rect_.width()-2, rect_.height()-2);
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
        font::ptr arial_18 = font_manager::get()->get_font(font_properties("Arial.ttf",18));
        gc->font(arial_18);
        surface->gc(gc);

        // fill the spinner box
        surface->fill_rect(interior_rect);

        // outline the spinner box
        surface->draw_rect(outline_rect);
        if (pressed_)
            surface->draw_rect(rect_);

        // draw the selected item
        items_[selected_]->rect(interior_rect);
        items_[selected_]->draw(surface);

        // draw the arrows
	bool fill_up = wrap_ || (selected_ > 0);
	bool fill_down = wrap_ || (selected_ < items_.size()-1);
        theme::user()->draw_arrow(x2() - 15, y1() + 5, 12, surface, fill_up);
        theme::user()->draw_arrow(x2() - 15, y2() - 10, 6, surface, fill_down);
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
        surface->clip_rect(clip_rect.empty() ? rect_ : clip_rect);

        rectangle interior_rect(rect_.x1()+3, rect_.y1()+3, rect_.width()-6, rect_.height()-6);
        rectangle outline_rect(rect_.x1()+1, rect_.y1()+1, rect_.width()-2, rect_.height()-2);

        graphics_context::ptr gc = graphics_context::create();

        // prepare the font
        font::ptr arial_18 = font_manager::get()->get_font(font_properties("Arial.ttf",18));
        gc->font(arial_18);
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

        if (pressed_) surface->draw_rect(rect_);
        surface->draw_rect(outline_rect);

        // cursor and selection calculations
        int sel_min = MIN(selection_start_, selection_end_);
        std::wstring presel_str = text_.substr(0, sel_min);
        std::wstring sel_str = text_.substr(sel_min, abs(selection_end_-selection_start_));
        int sel_x = interior_rect.x1() + arial_18->draw_len(presel_str);
        int sel_width = arial_18->draw_len(sel_str);
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

            surface->fill_rect(rectangle(sel_x, rect_.y1()+3, sel_width, rect_.height()-6));
        }
        // draw the cursor if there is no selection and we are focused
        else if (focused_)
            surface->draw_line(cursor_x, rect_.y1()+3, cursor_x, rect_.y2()-3);
        
        // draw the string text_
        surface->draw_text(interior_rect, text_);
    }

}
