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

#include "libstk/font_manager.h"

#include <iostream>
namespace stk
{


font_manager::ptr font_manager::instance_;
	
	
bool font_properties::operator<(const font_properties& rhs) const
{
	 return (height < rhs.height) && (fontname < rhs.fontname);
}
font_manager* font_manager::get()
{
	if(instance_)
		return instance_.get();
	instance_.reset(new font_manager());
	return instance_.get();
}
font_manager::font_manager()
{
	std::cout << "Font manager constructed\n";
}
font_manager::~font_manager()
{
	std::cout << "Font manager destructed\n";
}

//std::map<font_properties,font::weak_ptr> fonts;

font::ptr font_manager::get_font(font_properties properties)
{
	//std::cout << "get font called...\t";
	Tfonts::iterator font_iter=fonts.find(properties);
	if(font_iter==fonts.end())
	{
		//std::cout << "not font description yet...\t";
		font::ptr newfont(new font(properties.fontname,properties.height,0));
		fonts[properties]=newfont;
		//std::cout << "created\n";
		return newfont;
	}
	else
	{
		font::ptr font_ptr=font_iter->second;
		if(font_ptr)
		{
			//std::cout << "cached\n";
			return font_ptr;
		}
		//std::cout << "uncached...\t";
		font::ptr newfont(new font(properties.fontname,properties.height,0));
		fonts[properties]=newfont;
		//std::cout << "Created\n";
		return newfont;
	}
}

	
}
