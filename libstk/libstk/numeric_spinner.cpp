/**************************************************************************************************
 *    FILENAME: numeric_spinner.cpp
 * DESCRIPTION: numeric_spinner widget implementation.
 *     AUTHORS: Darren Hart
 *  START DATE: 14/Jul/2003  LAST UPDATE: 15/Jul/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *     license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include <sstream>
#include "libstk/numeric_spinner.h"
#include "libstk/event.h"
#include "libstk/key_event.h"
#include "libstk/mouse_event.h"
#include "libstk/keycode.h"

namespace stk
{
    numeric_spinner::ptr numeric_spinner::create(container::ptr parent, const rectangle& rect,
            double min, double max, double increment, int precision, bool wrap)
    {
        numeric_spinner::ptr new_numeric_spinner(new numeric_spinner(parent, rect, 
                    min, max, increment, precision, wrap));
        parent->add(new_numeric_spinner);
        return new_numeric_spinner;
    }

    numeric_spinner::numeric_spinner(container::ptr parent, const rectangle& rect,
            double min, double max, double increment, int precision, bool wrap) : 
        widget(parent, rect), min_(min), max_(max), increment_(increment), value_(min), 
    precision_(precision), wrap_(wrap)
    {
        cout << "numeric_spinner::numeric_spinner()" << endl;
        focusable(true);
        build_label();
    }

    numeric_spinner::~numeric_spinner()
    {}

    void numeric_spinner::handle_event(event::ptr e)
    {
        switch (e->type())
        {
            case event::key_down:
                {
                    key_event::ptr ke = boost::shared_static_cast<key_event>(e);
                    switch ( ke->fn_key() )
                    {
                        case key_enter:
                        case key_uparrow:
                            value_ = value_+increment_;
                            if (value_ > max_)
                            {	
                                if (wrap_) value_ = min_;
                                else value_ = max_;
                            }
                            build_label();
                            on_change();
                            redraw(rect_);
                            return;
                            break;
                        case key_downarrow:
                            value_ = value_-increment_;
                            if (value_ < min_) 
                            {
                                if (wrap_) value_ = max_;
                                else value_ = min_;
                            }
                            build_label();
                            on_change();
                            redraw(rect_);
                            return;
                            break;
                    }
                    break;
                }
            case event::mouse_up:
                {
                    mouse_event::ptr me = boost::shared_static_cast<mouse_event>(e);
                    if (region(me->x(), me->y()) == DOWN_ARROW)
                    {
                        value_ = value_-increment_;
                        if (value_ < min_) 
                        {
                            if (wrap_) value_ = max_;
                            else value_ = min_;
                        }
                    }
                    else
                    {
                        value_ = value_+increment_;
                        if (value_ > max_)
                        {	
                            if (wrap_) value_ = min_;
                            else value_ = max_;
                        }
                    }
                    build_label();
                    on_change();
                    redraw(rect_);
                    return;
                }
        }
        widget::handle_event(e);
    }

    void numeric_spinner::build_label()
    {
        // FIXME: format value_str_ to value_ with precision_ digits after the decimal
        std::wstringstream num_stream;

        /*if (precision_ > 0)*/ num_stream.setf(std::ios::showpoint);
        num_stream.precision(precision_);
        num_stream << value_;
        label_ = num_stream.str();
    }

    void numeric_spinner::precision(int val)
    {
        if (val >= 0)
        {
            precision_ = val;
            build_label();
        }
        else
            throw error_message_exception("cannot set precision < 0");
    }

    void numeric_spinner::min(double val)
    {
        min_ = val;
        if (min_ > max_) max_ = min_;
        if (value_ < min_) value_ = min_;
        build_label();
    }

    void numeric_spinner::max(double val)
    {
        max_ = val;
        if (max_ < min_) min_ = max_;
        if (value_ > max_) value_ = max_;
        build_label();
    }

    void numeric_spinner::increment(double val)
    {
        increment_ = val;
        // FIXME: consider assuring that value_ is min_ + N*increment_
    }
}
