/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Button widget.
 *     AUTHORS: Darren Hart
 *  START DATE: 2003/Mar/06
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_BUTTON_H
#define STK_BUTTON_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/signal.hpp>

#include <libstk/widget.h>
#include <libstk/container.h>

namespace stk
{
    class button : public widget
    {
    public:
        typedef boost::shared_ptr<button> ptr;
        typedef boost::weak_ptr<button> weak_ptr;

    private:

    protected:
        button(std::wstring label, const rectangle& rect);
        /// Specifies to Label of the Button (FIXME better text?)
        std::wstring label_;

    public:
        static button::ptr create(container::ptr parent, const std::wstring label,
                const rectangle& rect);
        virtual ~button();
        
        /********** EVENT HANDLER INTERFACE **********/
        virtual void handle_event(event::ptr e);
        /********** END EVENT HANDLER INTERFACE **********/

        /********** DRAWABLE INTERFACE **********/
        virtual void draw(surface::ptr surface, const rectangle& clip_rect = rectangle());
        /********** END DRAWABLE INTERFACE **********/

        /********** COMPONENT INTERFACE **********/
        /********** END COMPONENT INTERFACE **********/

        /********** BUTTON INTERFACE **********/
        /// set the property label (#label_)
        void label(std::wstring newlabel);
        /// get the property label (#label_)
        std::wstring label();
        // button signals
        boost::signal<bool (), combiner::logical_or<bool> > on_press;
        boost::signal<bool (), combiner::logical_or<bool> > on_release;
        /********** END BUTTON INTERFACE **********/
    };

} // namespace stk

#endif
