/**************************************************************************************************
 *    FILENAME: progress.h
 * DESCRIPTION: A progress bar widget with printf like label formatting.
 *     AUTHORS: Darren Hart, Marc Strämke
 *  START DATE: 22/Mar/2003  LAST UPDATE: 26/Jul/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_PROGRESS_H
#define STK_PROGRESS_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/signal.hpp>

#include <libstk/rectangle.h>
#include <libstk/widget.h>
#include <libstk/container.h>

namespace stk
{
    class progress : public widget
    {
    public:
        typedef boost::shared_ptr<progress> ptr;
        typedef boost::weak_ptr<progress> weak_ptr;

    private:

    protected:
        progress(container::ptr parent, const std::wstring& format_label, const rectangle& rect,
                 int range);

        /// rebuild the label to be displayed based on the current position and range
        void build_label();

        /// format_label defines the string displayed in the progress box
        /// \param %p percentage to two decimal places, i.e. 67.23%
        /// \param %d position in range, as an integer, i.e. 24
        /// \param %f percentage as a decimal to 4 digits, i.e. 0.6723%
        std::wstring format_label_;
        std::wstring label_;
        int range_;
        int position_;

    public:
        static progress::ptr create(container::ptr parent, const std::wstring& format_label,
                                    const rectangle& _rect, int range);
        virtual ~progress();


        /********** EVENT HANDLER INTERFACE **********/
        /********** END EVENT HANDLER INTERFACE **********/

        /********** DRAWABLE INTERFACE **********/
        virtual void draw(surface::ptr surface, const rectangle& clip_rect = rectangle());
        /********** END DRAWABLE INTERFACE **********/

        /********** COMPONENT INTERFACE **********/
        /********** END COMPONENT INTERFACE **********/

        /********** WIDGET INTERFACE **********/
        /********** END WIDGET INTERFACE **********/

        /********** PROGRESS INTERFACE **********/
        int range()
        {
            return range_;
        }
        void range(int val);
        int position()
        {
            return position_;
        }
        void position(int val);
        float percent()
        {
            return (float)position_/(float)range_;
        }
        void percent(float val);
        /********** END PROGRESS INTERFACE **********/

    };
}

#endif
