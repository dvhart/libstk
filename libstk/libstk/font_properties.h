/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Classes to describe fonts by name, height, style, etc.
 *     AUTHORS: Marc Strämke, Darren Hart, Vernon Mauery 
 *  START DATE: 2003/Mar/28
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_FONT_PROPERTIES_H
#define STK_FONT_PROPERTIES_H

#include <boost/smart_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/optional.hpp>
#include <string>
#include <libstk/logging.h>

namespace stk
{

    struct font_properties
    {
        typedef enum { plain=0, italic, bold, bold_italic } font_style;

        boost::optional<std::string> fontname;
        boost::optional<int> height;
        boost::optional<font_style> style;
        boost::optional<float> rotation;

        font_properties() { }
        font_properties(std::string name, int height, font_style style, float rotation);
        void merge(const font_properties& rhs);

        bool operator<(const font_properties& rhs) const;
    };

} // namespace stk

#endif // STK_FONT_PROPERTIES_H
