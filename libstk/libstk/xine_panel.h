/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Xine media panel
 *     AUTHORS: Darren Hart
 *  START DATE: 2003/Jul/21
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_XINE_PANEL_H
#define STK_XINE_PANEL_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/signal.hpp>

#include <libstk/widget.h>
#include <libstk/container.h>

namespace stk
{
    class xine_panel : public widget
    {
    public:
        typedef boost::shared_ptr<xine_panel> ptr;
        typedef boost::weak_ptr<xine_panel> weak_ptr;

    private:

    protected:
        xine_panel(container::ptr parent, const rectangle& rect);

    public:
        static xine_panel::ptr create(container::ptr parent, const rectangle& _rect);
        virtual ~xine_panel();

    };

} // namespace stk

#endif

