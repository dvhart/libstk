#ifndef FONT_MNGR_H
#define FONT_MNGR_H

#include <boost/smart_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <string>
#include <map>
#include "libstk/font.h"

namespace stk
{
	
/// \todo This should be in font.h and be done by the font class 
struct font_properties
{
	std::string fontname;
	int height;			
	font_properties(std::string name, int height): fontname(name),height(height) {}
	
	bool operator<(const font_properties& rhs) const;
};



/// \brief Class for manaing Font Instances
/// This class manages all creation and retrieveal of font class instances
/// It does not hold a reference to the contained font, the user has to keep a reference
/// if he wants to avoid recreating the font everytime it is acessed (costly)
class font_manager
{
public:
	typedef boost::shared_ptr<font_manager> ptr;
	typedef boost::weak_ptr<font_manager> weak_ptr;
private:
	static font_manager::ptr instance;

	typedef std::map<font_properties,font::weak_ptr> Tfonts;
	Tfonts fonts;

	font_manager();
public:
	/// Get a font with the provided Properties
	font::ptr get_font(font_properties properties);
	/// Get the font manager instance-> only way of accessing the singleton
	static font_manager* get();
	~font_manager();
};

}

#endif
