/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: A simple abstract base class to store YUV overlay data 
 *              (memory, size, format)
 *              backends.
 *     AUTHORS: Darren Hart
 *  START DATE: 2003/Jul/20
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_OVERLAY_H
#define STK_OVERLAY_H

#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <libstk/rectangle.h>
#include <libstk/surface.h>
#include <libstk/stk_types.h>

// supported pixel formats
#define STK_FORMAT_YV12 0x32315659 
#define STK_FORMAT_YUY2 0x32595559

using std::cout;
using std::endl;

namespace stk
{
    class overlay
    {
    public:
        typedef boost::shared_ptr<overlay> ptr;
        typedef boost::weak_ptr<overlay> weak_ptr;

    private:

    protected:
        overlay()
        {
            cout << "overlay::overlay()" << endl;
        }

    public:
        virtual ~overlay()
        {
            cout << "overlay::~overlay()" << endl;
        }

        // inline property methods
        virtual int width() const = 0;
        virtual int height() const = 0;
        virtual int format() const = 0;
        virtual int pitches(int index) const = 0;
        virtual byte* pixels(int index) const = 0;

        // methods
        virtual void lock() = 0;
        virtual void unlock() = 0;
        virtual void display(const rectangle& rect) = 0;
    };

} //end namespace stk

#endif
