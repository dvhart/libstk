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
		gc->fill_color(surface_->gen_color("0xFFFFFFFF")); 
		surface_->gc(gc);
		surface_->fill_rect(rect);	
	}
	
	void theme::draw_button(const rectangle& rect, const std::wstring& label,
		bool active, bool focused, bool hover)
	{
		//cout << "theme::draw_button()" << endl;
		graphics_context::ptr gc = graphics_context::create();
		if (active)
		{
			gc->fill_color(surface_->gen_color("0x00FF00FF")); 
			gc->line_color(surface_->gen_color("0x0000FFFF")); 
			surface_->gc(gc);
			surface_->fill_rect(rect);
			surface_->draw_rect(rect);
		}
		else if(focused)
		{
			gc->fill_color(surface_->gen_color("0xFF00FFFF")); 
			gc->line_color(surface_->gen_color("0x0000FFFF")); 
			surface_->gc(gc);
			surface_->fill_rect(rect);
			surface_->draw_rect(rect);
		}
		else if (hover)
		{
			gc->fill_color(surface_->gen_color("0xFF00FFFF")); 
			gc->line_color(surface_->gen_color("0x0000FFFF")); 
			surface_->gc(gc);
			surface_->fill_rect(rect);
			surface_->draw_rect(rect);
		}
		else
		{
			gc->fill_color(surface_->gen_color("0xFFFFFFFF")); 
			gc->line_color(surface_->gen_color("0x0000FFFF")); 
			surface_->gc(gc);
			surface_->fill_rect(rect);
			surface_->draw_rect(rect);
		}
		
		// draw the label for all states
		stk::font::ptr the_font = font::create("Arial.ttf", 25);
		gc->font(the_font);
		gc->font_fill_color(surface_->gen_color(127, 0, 80, 0xff));
		surface_->gc(gc);
		surface_->draw_text(rect, label);
	}

	void theme::draw_label(const rectangle& rect, const std::wstring& text)
	{
		//cout << "theme::draw_label()" << endl;
		graphics_context::ptr gc = graphics_context::create();
		gc->fill_color(surface_->gen_color("0xFF00FFFF")); 
		gc->line_color(surface_->gen_color("0x0000FFFF")); 
		try
		{
			stk::font::ptr bob = font::create("Arial.ttf", 25);
			gc->font(bob);
			gc->font_fill_color(surface_->gen_color(127, 0, 80, 0xff));
			surface_->gc(gc);
			surface_->draw_rect(rect);
			surface_->draw_text(rect, text);
		}
		catch (exception e)
		{
			cout << "theme::draw_label: error: " << e.to_string() << endl;
		}
	}
}
