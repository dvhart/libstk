/******************************************************************************
 *    FILENAME: font_manager.h
 * DESCRIPTION: Classes to cache fonts by name and height.
 *     AUTHORS: Marc Straemke, Darren Hart 
 *  START DATE: 28/Mar/2003  LAST UPDATE: 13/May/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available
 *              with the source as license.txt or at 
 *              http://www.libstk.org/index.php?page=docs/license
 *****************************************************************************/

#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include <boost/smart_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <string>
#include <map>
#include <libstk/font.h>

namespace stk
{

    /// \todo This should be in font.h and be done by the font class
    struct font_properties
    {
        std::string fontname;
        int height;
        font_properties(std::string name, int height): fontname(name),height(height)
        {}

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
        /// FIXME: shouldn't font properties be a const reference ? --dvhart
        font::ptr get_font(font_properties properties);
        /// Get the font manager instance-> only way of accessing the singleton
        /// FIXME: why are we returning a pointer and not a shared_ptr ? --dvhart
        static font_manager* get
            ();
        ~font_manager();
    };

}

#endif
