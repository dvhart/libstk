#include "libstk/graphics_context.h"
#include "libstk/exceptions.h"
#include "libstk/font_mngr.h"

#include "libstk/button.h"
#include "libstk/image_panel.h"
#include "libstk/label.h"
#include "libstk/list.h"
#include "libstk/list_item.h"
#include "libstk/progress.h"
#include "libstk/spinner.h"
#include "libstk/state.h"


// all draw routines go here with the exception of container as it isn't 
// themeable and only draws its children

namespace stk
{
	// some tribal colors for this theme
	const std::string outline_color_normal_str  = "0xFFFFFFFF";
	const std::string outline_color_focused_str = "0xFFFFFFFF";
	const std::string outline_color_hover_str   = "0xFFFF00FF";
	const std::string outline_color_pressed_str = "0xFFFF00FF";

	const std::string fill_state_color_str      = "0x0C2C4CFF";
	const std::string fill_color_normal_str     = "0x185899FF";
	const std::string fill_color_focused_str    = "0x2893FFFF";
	const std::string fill_color_hover_str      = "0x185899FF";
	const std::string fill_color_pressed_str    = "0x2893FFFF";
	
	const std::string font_color_normal_str     = "0xFFFFFFFF";
	const std::string font_color_focused_str    = "0xFFFFFFFF";
	const std::string font_color_hover_str      = "0xFFFF00FF";
	const std::string font_color_pressed_str    = "0xFFFF00FF";

	// fixme:: we need a clean way to do colors now that we are not a class!!
	// prepare our colors
	/*
	const color outline_color_normal_  = surface->gen_color(outline_color_normal_str_);
	const color outline_color_focused_ = surface->gen_color(outline_color_focused_str_);
	const color outline_color_hover_   = surface->gen_color(outline_color_hover_str_);
	const color outline_color_pressed_ = surface->gen_color(outline_color_pressed_str_);

	const color fill_state_color_      = surface->gen_color(fill_state_color_str_);
	const color fill_color_normal_     = surface->gen_color(fill_color_normal_str_);
	const color fill_color_focused_    = surface->gen_color(fill_color_focused_str_);
	const color fill_color_hover_      = surface->gen_color(fill_color_hover_str_);
	const color fill_color_pressed_    = surface->gen_color(fill_color_pressed_str_);

	const color font_color_normal_     = surface->gen_color(font_color_normal_str_);
	const color font_color_focused_    = surface->gen_color(font_color_focused_str_);
	const color font_color_hover_      = surface->gen_color(font_color_hover_str_);
	const color font_color_pressed_    = surface->gen_color(font_color_pressed_str_);
	*/
	
	void state::draw(surface::ptr surface)
	{
		//cout << "state::draw()" << endl;
		graphics_context::ptr gc = graphics_context::create();
		gc->fill_color(surface->gen_color("0x0C2C4CFF")); 
		surface->gc(gc);
		surface->fill_rect(rect_);	
		container::draw(surface); // this will draw all the children - document this for theme howto
	}
	
	void button::draw(surface::ptr surface)
	{
		//cout << "button::draw()" << endl;
		rectangle interior_rect(rect_.x1()+3, rect_.y1()+3, rect_.width()-6, rect_.height()-6);
		rectangle outline_rect(rect_.x1()+1, rect_.y1()+1, rect_.width()-2, rect_.height()-2);
		
		graphics_context::ptr gc = graphics_context::create();
		
		// prepare the font
		
		font::ptr the_font = font_manager::get()->get_font(font_properties("Arial.ttf",18));
		//stk::font::ptr the_font = font::create("Arial.ttf", 18);
		gc->font(the_font);
		
		if (pressed_)
		{
			gc->fill_color(surface->gen_color(fill_color_pressed_str)); 
			gc->line_color(surface->gen_color(outline_color_pressed_str)); 
			gc->font_fill_color(surface->gen_color(font_color_pressed_str));
		}
		else if(focused_)
		{
			gc->fill_color(surface->gen_color(fill_color_focused_str));
			if (hover_)
			{
				gc->line_color(surface->gen_color(outline_color_hover_str)); 
				gc->font_fill_color(surface->gen_color(font_color_hover_str));
			}
			else 
			{
				gc->line_color(surface->gen_color(outline_color_focused_str));
				gc->font_fill_color(surface->gen_color(font_color_focused_str));
			}
		}
		else
		{
			gc->fill_color(surface->gen_color(fill_color_normal_str));
			if (hover_) 
			{
				gc->line_color(surface->gen_color(outline_color_hover_str)); 
				gc->font_fill_color(surface->gen_color(font_color_hover_str));
			}
			else 
			{
				gc->line_color(surface->gen_color(outline_color_normal_str));
				gc->font_fill_color(surface->gen_color(font_color_normal_str));
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

	void progress::draw(surface::ptr surface)
	{
		//cout << "progress::draw()" << endl;
		graphics_context::ptr gc = graphics_context::create();
		gc->fill_color(surface->gen_color(fill_color_normal_str)); 
		gc->line_color(surface->gen_color(outline_color_normal_str));
		stk::font::ptr bob = font::create("Arial.ttf", 18);
		gc->font(bob);
		gc->font_fill_color(surface->gen_color(font_color_normal_str));
		surface->gc(gc);
		rectangle progress_rect(rect_.x1()+2, rect_.y1()+2, rect_.width()-4, rect_.height()-4);
		progress_rect.width((int)(progress_rect.width()*percent()));
		surface->fill_rect(progress_rect);
		surface->draw_rect(rect_);
		surface->draw_text(rect_, label_);
	}
	
	void label::draw(surface::ptr surface)
	{
		//cout << "label::draw()" << endl;
		graphics_context::ptr gc = graphics_context::create();
		gc->fill_color(surface->gen_color(fill_color_normal_str));
		gc->line_color(surface->gen_color(outline_color_normal_str));
		try
		{
			stk::font::ptr arial_18 = font::create("Arial.ttf", 18);
			gc->font(arial_18);
			gc->font_fill_color(surface->gen_color(font_color_normal_str));
			surface->gc(gc);
			surface->draw_text(rect_, text_);
		}
		catch (const exception& e)
		{
			cout << "theme::draw_label: error: " << e.what() << endl;
		}
	}

	void image_panel::draw(surface::ptr surface)
	{
		//cout << "image_panel::draw()" << endl;
		graphics_context::ptr gc = graphics_context::create();
		gc->line_color(surface->gen_color(outline_color_focused_str)); 
		surface->gc(gc);
		surface->clip_rect(rect_);
		surface->draw_rect(rect_);
		surface->draw_image(rect_.x1()+10, rect_.y1()+10, image_);
	}
	
	void list::draw(surface::ptr surface)
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
		gc->line_color(surface->gen_color(outline_color_normal_str));
		gc->fill_color(surface->gen_color(fill_color_focused_str));
		gc->font_fill_color(surface->gen_color(font_color_normal_str));
		stk::font::ptr arial_18 = font::create("Arial.ttf", 18);
		gc->font(arial_18);
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
	
	void list_item::draw(surface::ptr surface)
	{
		//cout << "list_item::draw()" << endl;
		// draw list is responsible for setting the graphics context!!
		surface->draw_text(rect_, label_);
	}

	void spinner::draw(surface::ptr surface)
	{
		cout << "spinner::draw()" << endl;
		rectangle interior_rect(rect_.x1()+3, rect_.y1()+3, rect_.width()-6, rect_.height()-6);
		rectangle outline_rect(rect_.x1()+1, rect_.y1()+1, rect_.width()-2, rect_.height()-2);
		graphics_context::ptr gc = graphics_context::create();

		if(focused_)
		{
			gc->fill_color(surface->gen_color(fill_color_focused_str));
			if (hover_)
			{
				gc->line_color(surface->gen_color(outline_color_hover_str)); 
				gc->font_fill_color(surface->gen_color(font_color_hover_str));
			}
			else 
			{
				gc->line_color(surface->gen_color(outline_color_focused_str));
				gc->font_fill_color(surface->gen_color(font_color_focused_str));
			}
		}
		else
		{
			gc->fill_color(surface->gen_color(fill_color_normal_str));
			if (hover_) 
			{
				gc->line_color(surface->gen_color(outline_color_hover_str)); 
				gc->font_fill_color(surface->gen_color(font_color_hover_str));
			}
			else 
			{
				gc->line_color(surface->gen_color(outline_color_normal_str));
				gc->font_fill_color(surface->gen_color(font_color_normal_str));
			}
		}

		gc->font_fill_color(surface->gen_color(font_color_normal_str));
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
	}
	
}
