/******************************************************************************
 *    FILENAME: label.cpp
 * DESCRIPTION: Label widget implementation.
 *     AUTHORS: Darren Hart, Vernon Mauery, Marc Straemke 
 *  START DATE: 03/Mar/2003  LAST UPDATE: 14/May/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available
 *              with the source as license.txt or at 
 *              http://www.libstk.org/index.php?page=docs/license
 *****************************************************************************/

#include <iostream>
#include "libstk/label.h"
#include "libstk/widget.h"

using std::cout;
using std::endl;

namespace stk
{

    label::ptr label::create(container::ptr parent, std::wstring text, const rectangle& rect)
    {
        label::ptr new_label(new label(parent, text, rect));
        parent->add
        (new_label);
        return new_label;
    }

    label::label(boost::shared_ptr<container> parent, std::wstring text,
                 const rectangle& rect) : widget(parent, rect), text_(text)
    {
        cout << "label::label()" << endl;
        focusable_ = false;
    }

    label::~label()
    {}

}
