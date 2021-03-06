/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Class describing the modell (MVC term) behind scrolling 
 *     AUTHORS: Marc Str�mke, Darren Hart
 *  START DATE: 2003/May/13
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Str�mke, Dirk H�rner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_SCROLL_MODEL_H
#define STK_SCROLL_MODEL_H

#include <boost/smart_ptr.hpp>
#include <boost/signal.hpp>

namespace stk
{
    /// This class describes the modell or data associated with scrolling
    class scroll_model
    {
    public:
        typedef boost::shared_ptr<scroll_model> ptr;
        typedef boost::weak_ptr<scroll_model> weak_ptr;
        
    private:
        scroll_model();
        /// The Size of the Scrollable area
        int size_;
        /// The Begin of the Visible Area
        int begin_;
        /// the size of the visible region
        int vis_size_;

    public:
        static scroll_model::ptr create();
        int size() const;
        void size(int val);
        int begin() const;
        void begin(int val);
        int end() const;
        int vis_size() const;
        void vis_size(int val);

        boost::signal<void () > on_change;
    };

} // namespace stk

#endif
