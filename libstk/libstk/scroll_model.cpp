/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Scroll_model class implementation.
 *     AUTHORS: Marc Strämke, Darren Hart
 *  START DATE: 2003/May/13
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include "libstk/logging.h"
#include "libstk/stk_types.h"
#include "libstk/scroll_model.h"

namespace stk
{
    scroll_model::ptr scroll_model::create()
    {
        scroll_model::ptr new_scroll_model(new scroll_model());
        return new_scroll_model;
    }

    scroll_model::scroll_model() : size_(1), begin_(0), vis_size_(1)
    {}

    int scroll_model::size() const
    {
        return size_;
    }

    void scroll_model::size(int val)
    {
        if (size_ != val)
        {
            INFO("size: " << size_ << " <-- " << val);
            size_ = val;
            begin_ = MAX(0, MIN(begin_, size_-vis_size_));
            on_change();
        }
    }

    int scroll_model::begin() const
    {
        return begin_;
    }

    void scroll_model::begin(int val)
    {
        if (begin_ != val)
        {
            INFO("begin: " << begin_ << " <-- " << val);
            begin_ = MAX(0, MIN(val, size_-vis_size_));
            INFO("  calling on_change()");
            on_change();
            INFO("  done");
        }
    }

    int scroll_model::end() const
    {
        return begin_ + vis_size_;
    }

    int scroll_model::vis_size() const
    {
        return vis_size_;
    }

    void scroll_model::vis_size(int val)
    {
        if (vis_size_ != val)
        {
            INFO("vis_size: " << vis_size_ << " <-- " << val);
            vis_size_ = val;
            begin_ = MAX(0, MIN(begin_, size_-vis_size_));
            on_change();
        }
    }

} // namespace stk
