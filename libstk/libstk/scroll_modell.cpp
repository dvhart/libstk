/******************************************************************************
 *    FILENAME: scroll_modell.cpp
 * DESCRIPTION: Scroll_modell class implementation.
 *     AUTHORS: Marc Straemke, Darren Hart
 *  START DATE: 13/May/2003  LAST UPDATE: 14/May/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available
 *              with the source as license.txt or at 
 *              http://www.libstk.org/index.php?page=docs/license
 *****************************************************************************/
 
#include "scroll_modell.h"

namespace stk
{

	scroll_modell::scroll_modell() : begin_(0),end_(1),size_(1)
	{}
	
	int scroll_modell::size()
	{
		return size_;
	}
	
	void scroll_modell::size(int newsize)
	{
		size_=newsize;
		on_change();
	}
	
	int scroll_modell::begin()
	{
		return begin_;
	}
	
	void scroll_modell::begin(int value)
	{
		begin_=value;
		on_change();
	}
	
	int scroll_modell::end()
	{
		return end_;
	}
	
	void scroll_modell::end(int value)
	{
		end_=value;
		on_change();
	}
	
} // namespace stk
