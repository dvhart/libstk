/******************************************************************************
 *    FILENAME: scroll_box.cpp
 * DESCRIPTION: Scroll_box container implementation.
 *     AUTHORS: Darren Hart, Marc Straemke 
 *  START DATE: 29/Apr/2003  LAST UPDATE: 14/May/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available
 *              with the source as license.txt or at 
 *              http://www.libstk.org/index.php?page=docs/license
 *****************************************************************************/

#include "libstk/scroll_box.h"
#include "libstk/event.h"
#include "libstk/key_event.h"
#include <boost/bind.hpp>

namespace stk
{
	scroll_box::ptr scroll_box::create(container::ptr parent, 
			const rectangle& rect)
	{
		scroll_box::ptr new_scroll_box(
				new scroll_box(parent, rect));
		parent->add(new_scroll_box);
		return new_scroll_box;
	}
	
	scroll_box::scroll_box(container::ptr parent, 
			const rectangle& rect) : container(parent, rect)
	{
		focusable_ = true;
		h_scroll(scroll_model::ptr(new scroll_model));
		v_scroll(scroll_model::ptr(new scroll_model));
	}
			
	scroll_box::~scroll_box()
	{
	}
	
	
	scroll_model::ptr scroll_box::h_scroll()
	{
		return h_scroll_;
	}
	void scroll_box::h_scroll(scroll_model::ptr value)
	{
		h_scroll_con.disconnect();
		h_scroll_=value;
		h_scroll_con=value->on_change.connect(boost::bind(&scroll_box::redraw,this));		
		h_scroll_->size(rect_.width());
	}
			
	scroll_model::ptr scroll_box::v_scroll()
	{
		return v_scroll_;
	}	
	void scroll_box::v_scroll(scroll_model::ptr value)
	{
		v_scroll_con.disconnect();
		v_scroll_=value;
		v_scroll_con=value->on_change.connect(boost::bind(&scroll_box::redraw,this));
		v_scroll_->size(rect_.height());
	}
	void scroll_box::redraw()
	{
		container::redraw(rect_);
	}
	void scroll_box::handle_event(event::ptr e)
	{
		cout << "scroll_box::handle_event()" << endl;	
		widget::handle_event(e); 
	}

}
