#include "theme.h"
#include "graphics_context.h"

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
		if (!instance_) throw std::string("theme::instance() - instance_ null, call theme::create() first");
		return instance_;
	}
	
	void theme::draw_state()
	{
		cout << "theme::draw_state()" << endl;
		graphics_context gc;
		gc.fill_color(surface_->gen_color("0xFFFFFFFF")); 
		surface_->gc(gc);
		surface_->fill_rect(surface_->rect());		
	}
	
	void theme::draw_button(rectangle& rect, bool active, bool focused, bool hover)
	{
		cout << "theme::draw_button()" << endl;
		graphics_context gc;
		if (active)
		{
			gc.fill_color(surface_->gen_color("0xFF00FFFF")); 
			gc.line_color(surface_->gen_color("0x0000FFFF")); 
			surface_->gc(gc);
			surface_->draw_rect(rect);
		}
		else if(focused)
		{
			gc.fill_color(surface_->gen_color("0xFF00FFFF")); 
			gc.line_color(surface_->gen_color("0x0000FFFF")); 
			surface_->gc(gc);
			surface_->draw_rect(rect);
		}
		else if (hover)
		{
			gc.fill_color(surface_->gen_color("0xFF00FFFF")); 
			gc.line_color(surface_->gen_color("0x0000FFFF")); 
			surface_->gc(gc);
			surface_->draw_rect(rect);
		}
		else
		{
			gc.fill_color(surface_->gen_color("0xFF00FFFF")); 
			gc.line_color(surface_->gen_color("0x0000FFFF")); 
			surface_->gc(gc);
			surface_->draw_rect(rect);
		}
	}
	
	void theme::draw_label()
	{
		cout << "theme::draw_label()" << endl;
	}
}
