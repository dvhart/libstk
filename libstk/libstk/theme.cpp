#include "theme.h"
#include "graphics_context.h"

namespace stk
{
	theme* theme::instance_ = 0;
	void theme::create(stk::surface::ptr surface)
	{
		instance_ = new theme(surface);
	}
	
	theme::theme(stk::surface::ptr surface) : surface_(surface)
	{
		cout << "theme::theme()" << endl;
	}

	theme* theme::instance() 
	{
		if (!instance_) throw std::string("theme::instance() - instance_ null, call theme::create() first");
		return instance_;
	}
	
	theme::~theme()
	{
	}

	
	void theme::draw_state()
	{
		graphics_context gc;
		gc.fill_color(surface_->gen_color("0xFFFFFFFF")); 
		surface_->gc(gc);
		surface_->fill_rect(surface_->rect());		
	}
	
	void theme::draw_button(rectangle& rect)
	{
		graphics_context gc;
		gc.fill_color(surface_->gen_color("0xFF00FFFF")); 
		gc.line_color(surface_->gen_color("0x0000FFFF")); 
		surface_->gc(gc);
		surface_->draw_rect(rect);
	}
	
	void theme::draw_label()
	{
	}
}
