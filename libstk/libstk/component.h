/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Libstk component class of the composite pattern, container.h
 *              defines the composite class.
 *     AUTHORS: Darren Hart, Marc Strämke
 *  START DATE: 2003/Apr/29
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_COMPONENT_H
#define STK_COMPONENT_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <libstk/event_handler.h>
#include <libstk/drawable.h>

namespace stk
{
    class widget;

    /// A class declaring the component interface.
    /// I think this class is superfluous and its interface should be moved
    /// down to widget. --dvhart
    class component : public event_handler, public drawable 
    {
    public:
        typedef boost::shared_ptr<component> ptr;
        typedef boost::weak_ptr<component> weak_ptr;

    private:

    protected:
        component()
        { }

    public:
        virtual ~component()
        { }


        /********** COMPONENT INTERFACE **********/
        /// Return the parent, widgets all return a valid pointer.
        /// Return an empty pointer here as not all derived classes have parents.
        virtual component::ptr parent()
        {
            return component::ptr();
        }
        /// Retrieve the next focusable widget.
        /// Implemented by container, here we just return an empty pointer.
        virtual boost::shared_ptr<widget> focus_next()
        {
            return boost::shared_ptr<widget>();
        }
        /// Retrieve the next focusable widget.
        /// Implemented by container, here we just return an empty pointer.
        virtual boost::shared_ptr<widget> focus_prev()
        {
            return boost::shared_ptr<widget>();
        }
        /// Add a child.
        /// This does nothing by default and is overridden in container.
        virtual void add
            (boost::shared_ptr<widget> item)
        { }
        /// Remove a child
        /// This does nothing by default and is overridden in container.
        virtual void remove
            (boost::shared_ptr<widget> item)
        { }
        /********** END COMPONENT INTERFACE **********/

    }
    ; // class parent
} // namespace stk

#endif
