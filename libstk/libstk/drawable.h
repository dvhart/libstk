/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Abstract drawable class, defining the interface for all 
 *              drawable Libstk objects such as widgets and list_items.
 *     AUTHORS: Darren Hart, Marc Strämke
 *  START DATE: 2003/Feb/22
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_DRAWABLE_H
#define STK_DRAWABLE_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <libstk/surface.h>
#include <libstk/rectangle.h>

namespace stk
{
    /// An abstract class defining the drawable interface.
    class drawable
    {
    public:
        typedef boost::shared_ptr<drawable> ptr;
        typedef boost::weak_ptr<drawable> weak_ptr;

    private:

    protected:

    public:
        drawable()
        { }
        
        virtual ~drawable()
        { }

        /// Retrieve the surface used in the application
        virtual boost::shared_ptr<stk::surface> surface() = 0;

        /// draw the current drawable on the passed surface
        /// fixme: do we need to pass surface if every drawable has a surface() method?
        virtual void draw(surface::ptr surface, const rectangle& clip_rect = rectangle())
        { }
        
        /// Redraw the area in rect the next time draw is called.
        /// See container for specifics regarding behavior.  The default
        /// implementation does nothing as leaf widgets redraw themselves
        /// entirely. 
        /// \param rect The area to be redrawn
        /// \param transform The coordinates need to be transformed
        /// \parma source The source fo the redraw upcall, used for optimization
        ///        NULL means unknown -> no optimization
        virtual void redraw(const rectangle& rect, drawable* source=NULL, bool transform=false)
        { }

    }; // class drawable
} // namespace stk

#endif
