/******************************************************************************
 *    FILENAME: scroll_model.cpp
 * DESCRIPTION: Scroll_model class implementation.
 *     AUTHORS: Marc Straemke, Darren Hart
 *  START DATE: 13/May/2003  LAST UPDATE: 14/May/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available
 *              with the source as license.txt or at 
 *              http://www.libstk.org/index.php?page=docs/license
 *****************************************************************************/
 
#include "scroll_model.h"

namespace stk
{

	scroll_model::scroll_model() : begin_(0),vis_size_(1),size_(1)
	{}
	
	int scroll_model::size()
	{
		return size_;
	}
	
	void scroll_model::size(int newsize)
	{
		size_=newsize;
		on_change();
	}
	
	int scroll_model::begin()
	{
		return begin_;
	}
	
	void scroll_model::begin(int value)
	{
		begin_=value;
		on_change();
	}
	
	int scroll_model::vis_size()
	{
		return vis_size_;
	}
	void scroll_model::vis_size(int newsize)
	{
		vis_size_=newsize;
		on_change();
	}
	
} // namespace stk
