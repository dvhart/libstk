/*************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: The primary container widget, managed by application. 
 *     AUTHORS: Darren Hart, Marc Strämke
 *  START DATE: 2003/Nov/29
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_STATE_H
#define STK_STATE_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <libstk/application.h>
#include <libstk/container.h>
#include <libstk/image.h>
#include <libstk/widget.h>

namespace stk
{
    class state : public container
    {
    public:
        typedef boost::shared_ptr<state> ptr;
        typedef boost::weak_ptr<state> weak_ptr;

    private:
        /// redeclare this method as private as a state's parent cannot be changed
        virtual void parent(boost::shared_ptr<container> parent) { }

    protected:
        image::ptr bg_image_;
        state(application::ptr parent);

    public:
        virtual ~state();
        static state::ptr create(application::ptr parent);
        static state::ptr create(application::ptr parent, std::string bg_image_path);

        /********** EVENT HANDLER INTERFACE **********/
        virtual void handle_event(event::ptr e);
        /********** END EVENT HANDLER INTERFACE **********/

        /********** DRAWABLE INTERFACE **********/
        //virtual surface::ptr surface();
        virtual void draw(surface::ptr surface, const rectangle& clip_rect = rectangle());
        //virtual void redraw(bool val, const rectangle& rect);
        /********** END DRAWABLE INTERFACE **********/

        /********** COMPONENT INTERFACE **********/
        // virtual void parent(boost::shared_ptr<container> parent); // see private above
        /********** END COMPONENT INTERFACE **********/

        /********** CONTAINER INTERFACE **********/
        /********** END CONTAINER INTERFACE **********/

        /********** STATE INTERFACE **********/
        // FIXME: these two should probably be moved up to container
        virtual widget::ptr focus_first();
        virtual widget::ptr focus_last();
        boost::signal<bool (), combiner::logical_or<bool> > on_enter;
        boost::signal<bool (), combiner::logical_or<bool> > on_leave;
        /********** END STATE INTERFACE **********/
    };
}

#endif
