/******************************************************************************
 *    FILENAME:  
 * DESCRIPTION: 
 *     AUTHORS: First Last, First Last, etc.
 *  START DATE: DD/MMM/YYYY  LAST UPDATE: DD/MMM/YYY
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
		h_scroll(scroll_modell::ptr(new scroll_modell));
		v_scroll(scroll_modell::ptr(new scroll_modell));
	}
			
	scroll_box::~scroll_box()
	{
	}
	
	
	scroll_modell::ptr scroll_box::h_scroll()
	{
		return h_scroll_;
	}
	void scroll_box::h_scroll(scroll_modell::ptr value)
	{
		h_scroll_con.disconnect();
		h_scroll_=value;
		h_scroll_con=value->on_change.connect(boost::bind(&scroll_box::redraw,this));		
		h_scroll_->size(rect_.width());
	}
			
	scroll_modell::ptr scroll_box::v_scroll()
	{
		return v_scroll_;
	}	
	void scroll_box::v_scroll(scroll_modell::ptr value)
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
