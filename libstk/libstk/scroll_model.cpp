/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Scroll_model class implementation.
 *     AUTHORS: Marc Str�mke, Darren Hart
 *  START DATE: 2003/May/13
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Str�mke, Dirk H�rner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include "scroll_model.h"

namespace stk
{
    scroll_model::ptr scroll_model::create()
    {
        scroll_model::ptr new_scroll_model(new scroll_model());
        return new_scroll_model;
    }

    scroll_model::scroll_model() : size_(1),begin_(0),vis_size_(1)
    {}

    int scroll_model::size() const
    {
        return size_;
    }

    void scroll_model::size(int newsize)
    {
        size_=newsize;
        on_change();
    }

    int scroll_model::begin() const
    {
        return begin_;
    }

    void scroll_model::begin(int value)
    {
        begin_=value;
        on_change();
    }

    int scroll_model::end() const
    {
        return begin_ + vis_size_;
    }

    int scroll_model::vis_size() const
    {
        return vis_size_;
    }

    void scroll_model::vis_size(int newsize)
    {
        vis_size_=newsize;
        on_change();
    }

} // namespace stk
