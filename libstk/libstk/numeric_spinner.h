/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: A list widget that displays only one value and wraps around as
 *              the user cycles through the list_items.
 *     AUTHORS: Darren Hart
 *  START DATE: 2003/Jul/14
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_NUMERIC_SPINNER_H
#define STK_NUMERIC_SPINNER_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <libstk/rectangle.h>
#include <libstk/container.h>

namespace stk
{
    class numeric_spinner : public widget
    {
        public:
            typedef boost::shared_ptr<numeric_spinner> ptr;
            typedef boost::weak_ptr<numeric_spinner> weak_ptr;

        private:
            /// Region codes
            enum regions_ { DOWN_ARROW, UP_ARROW, LABEL };
            /// Format the label_ string according to value_ and precision_
            void build_label();
            /// Return a region code for an x,y coordinate (defined by the theme).
            /// If the x,y coordinate is over the up arrow, then UP_ARROW is returned.
            int region(int x, int y);

        protected:
            numeric_spinner(container::ptr parent, const rectangle& rect, 
                    double min, double max, double increment, int precision, bool wrap);
            /// the minimum value
            double min_;
            /// the maximum value
            double max_;
            /// the amount to increment value_ when changing it
            double increment_;
            /// the value stored in the numeric_spinner
            double value_;
            /// the string formatted from value_ and displayed by draw()
            std::wstring label_;
            /// number of decimal places to display (BROKEN)
            int precision_; 
            /// wrap around from first to last and last to first values
            bool wrap_;


        public:
            static numeric_spinner::ptr create(container::ptr parent, const rectangle& rect, 
                    double min, double max, double increment, int precision, bool wrap);
            virtual ~numeric_spinner();

            /********** EVENT HANDLER INTERFACE **********/
            virtual void handle_event(event::ptr e);
            /********** END EVENT HANDLER INTERFACE **********/

            /********** DRAWABLE INTERFACE **********/
            virtual void draw(surface::ptr surface, const rectangle& clip_rect = rectangle());
            /********** END DRAWABLE INTERFACE **********/

            /********** COMPONENT INTERFACE **********/
            /********** END COMPONENT INTERFACE **********/

            /********** NUMERIC SPINNER INTERFACE **********/
            int precision() const { return precision_; }
            void precision(int val);
            double value() const { return value_; }
            double min() const { return min_; }
            void min(double val);
            double max() const { return max_; }
            void max(double val);
            double increment() const { return increment_; }
            void increment(double val);
            bool wrap() const { return wrap_; }
            void wrap(bool val) { wrap_ = val; redraw(rect()); }

            // numeric_spinner signals
            boost::signal<bool (), combiner::logical_or<bool> > on_change;
            /********** END NUMERIC SPINNER INTERFACE **********/
    };
}

#endif
