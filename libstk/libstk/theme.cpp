#include "theme.h"
#include "graphics_context.h"
#include "exceptions.h"

namespace stk
{
	theme::ptr theme::instance_;

	theme::ptr theme::create(stk::surface::ptr surface)
	{
		instance_ = theme::ptr(new theme(surface));	
		return instance_;
	}
	
	theme::theme(stk::surface::ptr surface) : surface_(surface)
	{
		cout << "theme::theme()" << endl;

		// prepare our colors
		outline_color_normal_  = surface->gen_color(outline_color_normal_str_);
		outline_color_focused_ = surface->gen_color(outline_color_focused_str_);
		outline_color_hover_   = surface->gen_color(outline_color_hover_str_);
		outline_color_active_  = surface->gen_color(outline_color_active_str_);

		fill_color_normal_     = surface->gen_color(fill_color_normal_str_);
		fill_color_focused_    = surface->gen_color(fill_color_focused_str_);
		fill_color_hover_      = surface->gen_color(fill_color_hover_str_);
		fill_color_active_     = surface->gen_color(fill_color_active_str_);

		font_color_normal_     = surface->gen_color(font_color_normal_str_);
		font_color_focused_    = surface->gen_color(font_color_focused_str_);
		font_color_hover_      = surface->gen_color(font_color_hover_str_);
		font_color_active_     = surface->gen_color(font_color_active_str_);
	}

	theme::~theme()
	{
	}

	theme::ptr theme::instance() 
	{
		if (!instance_) throw error_message_exception("theme::instance() - instance_ null, call theme::create() first");
		return instance_;
	}
	
	void theme::draw_state(const rectangle& rect)
	{
		//cout << "theme::draw_state()" << endl;
		graphics_context::ptr gc = graphics_context::create();
		gc->fill_color(surface_->gen_color("0x004E66FF")); 
		surface_->gc(gc);
		surface_->fill_rect(rect);	
	}
	
	void theme::draw_button(const rectangle& rect, const std::wstring& label,
		bool active, bool focused, bool hover)
	{
		rectangle interior_rect(rect.x1()+3, rect.y1()+3, rect.width()-5, rect.height()-4);
		rectangle outline_rect(rect.x1()+1, rect.y1()+1, rect.width()-2, rect.height()-1);
		
		//cout << "theme::draw_button()" << endl;
		graphics_context::ptr gc = graphics_context::create();
		
		// prepare the font
		stk::font::ptr the_font = font::create("Arial.ttf", 25);
		gc->font(the_font);
		
		if (active)
		{
			gc->fill_color(fill_color_active_); 
			gc->line_color(outline_color_active_); 
			gc->font_fill_color(font_color_active_);
		}
		else if(focused)
		{
			gc->fill_color(fill_color_focused_); 
			gc->line_color(outline_color_focused_); 
			gc->font_fill_color(font_color_focused_);
		}
		else if (hover)
		{
			gc->fill_color(fill_color_hover_); 
			gc->line_color(outline_color_hover_); 
			gc->font_fill_color(font_color_hover_);
		}
		else
		{
			gc->fill_color(fill_color_normal_); 
			gc->line_color(outline_color_normal_); 
			gc->font_fill_color(font_color_normal_);
		}
		
		// FIXME: I think we may be off by one in the rect draw/fill routines
		// draw the label for all states
		surface_->gc(gc);
		surface_->fill_rect(interior_rect);
		surface_->draw_rect(outline_rect);
		surface_->draw_text(rect, label);
	}

	void theme::draw_progress(const rectangle& rect, const std::wstring& label, float percent)
	{
		//cout << "theme::draw_progress()" << endl;
		graphics_context::ptr gc = graphics_context::create();
		gc->fill_color(fill_color_normal_); 
		gc->line_color(outline_color_normal_);
		stk::font::ptr bob = font::create("Arial.ttf", 25);
		gc->font(bob);
		gc->font_fill_color(font_color_normal_);
		surface_->gc(gc);
		rectangle progress_rect(rect.x1()+2, rect.y1()+2, rect.width()-4, rect.height()-3);
		progress_rect.width((int)(progress_rect.width()*percent));	
		surface_->fill_rect(progress_rect);
		surface_->draw_rect(rect);
		surface_->draw_text(rect, label);
	}
	
	void theme::draw_label(const rectangle& rect, const std::wstring& text)
	{
		//cout << "theme::draw_label()" << endl;
		graphics_context::ptr gc = graphics_context::create();
		gc->fill_color(fill_color_normal_); 
		gc->line_color(outline_color_normal_); 
		try
		{
			stk::font::ptr bob = font::create("Arial.ttf", 25);
			gc->font(bob);
			gc->font_fill_color(font_color_normal_);
			surface_->gc(gc);
			//surface_->draw_rect(rect);
			surface_->draw_text(rect, text);
		}
		catch (const exception& e)
		{
			cout << "theme::draw_label: error: " << e.what() << endl;
		}
	}
}
