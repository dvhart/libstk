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

#include "libstk/image_panel.h"
#include "libstk/image.h"

namespace stk
{
	image_panel::ptr image_panel::create(container::ptr parent, const rectangle& rect, 
			image::ptr img)
	{
		image_panel::ptr new_image_panel(new image_panel(parent, rect, img));
		parent->add(new_image_panel);
		return new_image_panel;
	}
	
	image_panel::image_panel(widget::ptr parent, const rectangle& rect, image::ptr img)
		: widget(parent, rect), image_(img)
	{
		cout << "image_panel::image_panel()" << endl;
	}
	
	image_panel::~image_panel()
	{
	}
	
}
