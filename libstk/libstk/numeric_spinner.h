/**************************************************************************************************
 *    FILENAME: numeric_spinner.h
 * DESCRIPTION: A list widget that displays only one value and wraps around as
 *              the user cycles through the list_items.
 *     AUTHORS: Darren Hart
 *  START DATE: 14/Jul/2003  LAST UPDATE: 14/Jul/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
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
                double min, double max, double increment, int precision);
        double min_, max_, increment_, value_;
        std::wstring label_;
        int precision_; 
	

    public:
        static numeric_spinner::ptr create(container::ptr parent, const rectangle& rect, 
                double min, double max, double increment, int precision);
        virtual ~numeric_spinner();
        	
        /********** EVENT HANDLER INTERFACE **********/
        virtual void handle_event(event::ptr e);
        /********** END EVENT HANDLER INTERFACE **********/

        /********** DRAWABLE INTERFACE **********/
        virtual void draw(surface::ptr surface, const rectangle& clip_rect = rectangle());
        /********** END DRAWABLE INTERFACE **********/

        /********** NUMERIC SPINNER INTERFACE **********/
        int precision() const { return precision_; }
        void precision(const int val);
        double value() const { return value_; }
        double min() const { return min_; }
        void min(const double val);
        double max() const { return max_; }
        void max(const double val);
        double increment() const { return increment_; }
        void increment(const double val);
        
        // numeric_spinner signals
        boost::signal<bool (), combiner::logical_or<bool> > on_change;
        /********** END NUMERIC SPINNER INTERFACE **********/
    };
}

#endif
