#include "libstk/color_manager.h"
#include "libstk/graphics_context.h"
#include "libstk/exceptions.h"
#include "libstk/font_mngr.h"

#include "libstk/point.h"
#include "libstk/button.h"
#include "libstk/image_panel.h"
#include "libstk/label.h"
#include "libstk/list.h"
#include "libstk/list_item.h"
#include "libstk/progress.h"
#include "libstk/scroll_box.h"
#include "libstk/spinner.h"
#include "libstk/state.h"
#include "libstk/theme.h"
#include "libstk/tribal_theme.h"

// all draw routines go here with the exception of container as it isn't 
// themeable and only draws its children

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
		cout << "user_theme::user_theme()" << endl;
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
	}
	
	void user_theme::draw_arrow(int x, int y, int dir, surface::ptr surface)
	{
		switch (dir)
		{
			case 3:
			{
				std::vector<point> arrow_points;
				arrow_points.push_back(point(x, y));
				arrow_points.push_back(point(x + 5, y + 5));
				arrow_points.push_back(point(x, y + 10));
				surface->fill_poly_aa(arrow_points);
				break;
			}
			case 6:
			{
				std::vector<point> arrow_points;
				arrow_points.push_back(point(x, y));
				arrow_points.push_back(point(x + 10, y));
				arrow_points.push_back(point(x + 5, y + 5));
				surface->fill_poly_aa(arrow_points);
				break;
			}
			case 9:
			{
				std::vector<point> arrow_points;
				arrow_points.push_back(point(x + 5, y));
				arrow_points.push_back(point(x, y + 5));
				arrow_points.push_back(point(x + 5, y + 10));
				surface->fill_poly_aa(arrow_points);
				break;
			}
			case 12:
			{
				std::vector<point> arrow_points;
				arrow_points.push_back(point(x, y + 5));
				arrow_points.push_back(point(x + 10, y + 5));
				arrow_points.push_back(point(x + 5, y));
				surface->fill_poly_aa(arrow_points);
				break;
			}

			default:
				throw error_message_exception("user_theme::draw_arrow - invalid direction, use 3, 6, 9 or 12");
		}
	}
	// end tribal user theme
	
	// define the various widget draw routines
	void state::draw(surface::ptr surface, const rectangle& clip_rect)
	{
		surface->clip_rect(rect_);
		//cout << "state::draw()" << endl;
		graphics_context::ptr gc = graphics_context::create();
		gc->fill_color(color_manager::get_color(color_properties(fill_state_color_str, surface)));
		surface->gc(gc);
		surface->fill_rect(rect_);	
		container::draw(surface); // this will draw all the children - document this for theme howto
	}
	
	void button::draw(surface::ptr surface, const rectangle& clip_rect)
	{
		//cout << "button::draw()" << endl;
		
		surface->clip_rect(rect_);
		
		rectangle interior_rect(rect_.x1()+3, rect_.y1()+3, rect_.width()-6, rect_.height()-6);
		rectangle outline_rect(rect_.x1()+1, rect_.y1()+1, rect_.width()-2, rect_.height()-2);
		
		graphics_context::ptr gc = graphics_context::create();
		
		// prepare the font
		
		font::ptr the_font = font_manager::get()->get_font(font_properties("Arial.ttf",18));
		//stk::font::ptr the_font = font::create("Arial.ttf", 18);
		gc->font(the_font);
		
		if (pressed_)
		{
			gc->fill_color(color_manager::get_color(color_properties(fill_color_pressed_str, surface))); 
			gc->line_color(color_manager::get_color(color_properties(outline_color_pressed_str, surface))); 
			gc->font_fill_color(color_manager::get_color(color_properties(font_color_pressed_str, surface)));
		}
		else if(focused_)
		{
			gc->fill_color(color_manager::get_color(color_properties(fill_color_focused_str, surface)));
			if (hover_)
			{
				gc->line_color(color_manager::get_color(color_properties(outline_color_hover_str, surface))); 
				gc->font_fill_color(color_manager::get_color(color_properties(font_color_hover_str, surface)));
			}
			else 
			{
				gc->line_color(color_manager::get_color(color_properties(outline_color_focused_str, surface)));
				gc->font_fill_color(color_manager::get_color(color_properties(font_color_focused_str, surface)));
			}
		}
		else
		{
			gc->fill_color(color_manager::get_color(color_properties(fill_color_normal_str, surface)));
			if (hover_) 
			{
				gc->line_color(color_manager::get_color(color_properties(outline_color_hover_str, surface))); 
				gc->font_fill_color(color_manager::get_color(color_properties(font_color_hover_str, surface)));
			}
			else 
			{
				gc->line_color(color_manager::get_color(color_properties(outline_color_normal_str, surface)));
				gc->font_fill_color(color_manager::get_color(color_properties(font_color_normal_str, surface)));
			}
		}
		
		// FIXME: I think we may be off by one in the rect draw/fill routines
		// draw the label for all states
		surface->gc(gc);
		surface->fill_rect(interior_rect);
		if (pressed_) surface->draw_rect(rect_);
		surface->draw_rect(outline_rect);
		surface->draw_text(interior_rect, label_);
	}

	void progress::draw(surface::ptr surface, const rectangle& clip_rect)
	{
		//cout << "progress::draw()" << endl;
		graphics_context::ptr gc = graphics_context::create();
		gc->fill_color(color_manager::get_color(color_properties(fill_color_normal_str, surface)));
		gc->line_color(color_manager::get_color(color_properties(outline_color_normal_str, surface)));
		stk::font::ptr bob = font::create("Arial.ttf", 18);
		gc->font(bob);
		gc->font_fill_color(color_manager::get_color(color_properties(font_color_normal_str, surface)));
		surface->gc(gc);
		rectangle progress_rect(rect_.x1()+2, rect_.y1()+2, rect_.width()-4, rect_.height()-4);
		progress_rect.width((int)(progress_rect.width()*percent()));
		surface->fill_rect(progress_rect);
		surface->draw_rect(rect_);
		surface->draw_text(rect_, label_);
	}
	
	void label::draw(surface::ptr surface, const rectangle& clip_rect)
	{
		//cout << "label::draw()" << endl;
		graphics_context::ptr gc = graphics_context::create();
		gc->fill_color(color_manager::get_color(color_properties(fill_color_normal_str, surface)));
		gc->line_color(color_manager::get_color(color_properties(outline_color_normal_str, surface)));
		try
		{
			stk::font::ptr arial_18 = font::create("Arial.ttf", 18);
			gc->font(arial_18);
			gc->font_fill_color(color_manager::get_color(color_properties(font_color_normal_str, surface)));
			surface->gc(gc);
			surface->draw_text(rect_, text_);
		}
		catch (const exception& e)
		{
			cout << "theme::draw_label: error: " << e.what() << endl;
		}
	}

	void image_panel::draw(surface::ptr surface, const rectangle& clip_rect)
	{
		//cout << "image_panel::draw()" << endl;
		graphics_context::ptr gc = graphics_context::create();
		gc->line_color(color_manager::get_color(color_properties(outline_color_focused_str, surface)));
		surface->gc(gc);
		surface->clip_rect(clip_rect.empty() ? rect_ : clip_rect);
		//surface->draw_rect(rect_);
		//surface->draw_image(rect_.x1()+10, rect_.y1()+10, image_);
		surface->draw_image(rect_.x1(), rect_.y1(), image_);
	}
	
	void list::draw(surface::ptr surface, const rectangle& clip_rect)
	{
		//cout << "list::draw()" << endl;
		float sel;
		
		if (prev_selected_ != selected_)
		{
			sel = (float)(prev_selected_) + (float)(selected_ - prev_selected_)*((float)frame_/4.0);
			if (frame_ < 4) 
			{
				frame_++;
				redraw(rect_);
			}
			else 
			{
				frame_ = 0;
				prev_selected_ = selected_;
			}
		}
		else
		{
			sel = (float)selected_;
		}
		
		graphics_context::ptr gc = graphics_context::create();
		gc->line_color(color_manager::get_color(color_properties(outline_color_normal_str, surface)));
		if (focused_)
			gc->fill_color(color_manager::get_color(color_properties(fill_color_focused_str, surface)));
		else
			gc->fill_color(color_manager::get_color(color_properties(fill_color_normal_str, surface)));
		gc->font_fill_color(color_manager::get_color(color_properties(font_color_normal_str, surface)));
		stk::font::ptr arial_12 = font::create("Arial.ttf", 12);
		gc->font(arial_12);
		surface->gc(gc);
	
		// outline the list box
		surface->clip_rect(rect_);
		surface->draw_rect(rect_);
		
		// draw the highlighted selection
		rectangle cur_rect;
		cur_rect.x1(rect_.x1() + 2);
		cur_rect.width(rect_.width() - 4);
		cur_rect.y1(rect_.y1() + (int)(selected_*25) + 1);
		cur_rect.height(23);
		surface->fill_rect(cur_rect);
		
		// draw each item
		cur_rect.x1(rect_.x1() + 10);
		cur_rect.width(rect_.width() - 20);
		for (int i = 0; i < items_.size(); i++)
		{
			cur_rect.y1(rect_.y1() + i*25);
			cur_rect.height(25);
			items_[i]->rect(cur_rect);
			items_[i]->draw(surface);
		}
	}
	
	void list_item::draw(surface::ptr surface, const rectangle& clip_rect)
	{
		//cout << "list_item::draw()" << endl;
		// draw list is responsible for setting the graphics context!!
		surface->draw_text(rect_, label_);
	}

	void spinner::draw(surface::ptr surface, const rectangle& clip_rect)
	{
		cout << "spinner::draw()" << endl;
		rectangle interior_rect(rect_.x1()+3, rect_.y1()+3, rect_.width()-6, rect_.height()-6);
		rectangle outline_rect(rect_.x1()+1, rect_.y1()+1, rect_.width()-2, rect_.height()-2);
		graphics_context::ptr gc = graphics_context::create();

		if(focused_)
		{
			gc->fill_color(color_manager::get_color(color_properties(fill_color_focused_str, surface)));
			if (hover_)
			{
				gc->line_color(color_manager::get_color(color_properties(outline_color_hover_str, surface))); 
				gc->font_fill_color(color_manager::get_color(color_properties(font_color_hover_str, surface)));
			}
			else 
			{
				gc->line_color(color_manager::get_color(color_properties(outline_color_focused_str, surface)));
				gc->font_fill_color(color_manager::get_color(color_properties(font_color_focused_str, surface)));
			}
		}
		else
		{
			gc->fill_color(color_manager::get_color(color_properties(fill_color_normal_str, surface)));
			if (hover_) 
			{
				gc->line_color(color_manager::get_color(color_properties(outline_color_hover_str, surface))); 
				gc->font_fill_color(color_manager::get_color(color_properties(font_color_hover_str, surface)));
			}
			else 
			{
				gc->line_color(color_manager::get_color(color_properties(outline_color_normal_str, surface)));
				gc->font_fill_color(color_manager::get_color(color_properties(font_color_normal_str, surface)));
			}
		}

		gc->font_fill_color(color_manager::get_color(color_properties(font_color_normal_str, surface)));
		stk::font::ptr arial_18 = font::create("Arial.ttf", 18);
		gc->font(arial_18);
		surface->gc(gc);
	
		surface->clip_rect(rect_);
		
		// fill the spinner box
		surface->fill_rect(interior_rect); 
		
		// outline the spinner box
		surface->draw_rect(outline_rect);
		if (pressed_) surface->draw_rect(rect_);
		
		// draw the selected item
		items_[selected_]->rect(interior_rect);
		items_[selected_]->draw(surface);

		// draw the arrows
		theme::user()->draw_arrow(x2() - 15, y1() + 5, 12, surface);
		theme::user()->draw_arrow(x2() - 15, y2() - 10, 6, surface);
	}
	
	void scroll_box::draw(surface::ptr surface, const rectangle& clip_rect)
	{
		cout << "scroll_decorator::draw()" << endl;
		point scroll_offset;
		scroll_offset.x(h_scroll()->begin());
		scroll_offset.y(v_scroll()->begin());
		surface->offset()+=scroll_offset;
		
		if (children_.size() > 0)
		{
			if (clip_rect.empty())
			{
				rectangle temp_rect = rect_;
				children_[0]->draw(surface, temp_rect);
			}
			else
			{
				children_[0]->draw(surface, clip_rect);
			}
		}
		
		surface->offset()-=scroll_offset;
	}
	
}
