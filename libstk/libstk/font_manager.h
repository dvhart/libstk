/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Classes to cache fonts by name and height.
 *     AUTHORS: Marc Strämke, Darren Hart 
 *  START DATE: 2003/Mar/28
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_FONT_MANAGER_H
#define STK_FONT_MANAGER_H

#include <boost/smart_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <string>
#include <map>
#include <libstk/font.h>
#include <libstk/font_properties.h>

namespace stk
{

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
        /// fixme: make this a static class or use the namespace idiom, there is no
        /// need for it to be singleton (see color_manager)
        static font_manager::ptr instance_;

        /// FIXME: both Tfonts and fonts are names contrary to coding standards
        /// i.e. "names must be descriptive" and "All private variables use _ as a suffix"
        /// how about Tfont_map and font_map_ ? --dvhart
        typedef std::map<font_properties,font::ptr> Tfonts;
        Tfonts fonts;
        font_manager();
        
    public:
        /// Get a font with the provided Properties
        font::ptr get_font(const font_properties& properties);
        /// Get the font manager instance-> only way of accessing the singleton
        static font_manager::ptr get();
        ~font_manager();
    };

}

#endif
