/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Progress widget implementation.
 *     AUTHORS: Darren Hart, Marc Strämke
 *  START DATE: 2003/Mar/22
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include <iostream>
#include <sstream>
#include "libstk/progress.h"
#include "libstk/widget.h"

namespace stk
{

    progress::ptr progress::create(container::ptr parent, const std::wstring& format_label,
                                   const rectangle& rect, int range)
    {
        progress::ptr new_progress(new progress(format_label, rect, range));
        new_progress->parent(parent);
        return new_progress;
    }

    progress::progress(const std::wstring& format_label, const rectangle& rect, int range) : 
        widget(rect), format_label_(format_label), range_(range)
    {
        INFO("constructor");
        focusable_ = false;
        build_label();
    }

    progress::~progress()
    {
        INFO("destructor");
    }

    void progress::range(int val)
    {
        range_ = MAX(0, val);
        build_label();
        redraw(rect());
    }

    void progress::fill(int val)
    {
        fill_ = MAX(0, MIN(range_, val));
        build_label();
        redraw(rect());
    }

    void progress::percent(float val)
    {
        fill_ = MAX(0, (int)(range_*MIN(1.0, val)));
        build_label();
        redraw(rect());
    }

    void progress::build_label()
    {
        // %p, percent_*100 to two decimal places, followed by %
        // %d, fill
        // %f, percent_ to four decimal places

        std::wstringstream num_stream;
        label_ = format_label_;

        // get the percent string
        num_stream.setf(std::ios::showpoint);
        num_stream.setf(std::ios::fixed);
        num_stream.precision(2);
        num_stream << percent()*100 << "%";
        std::wstring percent_str = num_stream.str();

        // get the decimal string
        num_stream.str(L"");
        num_stream.unsetf(std::ios::fixed);
        num_stream.precision(4);
        num_stream << percent();
        std::wstring decimal_str = num_stream.str();

        // get the fill string
        num_stream.str(L"");
        num_stream.unsetf(std::ios::showpoint);
        num_stream << fill_;
        std::wstring fill_str = num_stream.str();

        int index;
        while ((index = label_.find(L"%p")) >= 0)
            label_.replace(index, 2, percent_str);
        while ((index = label_.find(L"%d")) >= 0)
            label_.replace(index, 2, fill_str);
        while ((index = label_.find(L"%f")) >= 0)
            label_.replace(index, 2, decimal_str);
    }

}
