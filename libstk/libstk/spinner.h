/******************************************************************************
 *    FILENAME: spinner.h
 * DESCRIPTION: A list widget that displays only one value and wraps around as
 *              the user cycles through the list_items.
 *     AUTHORS: Darren Hart
 *  START DATE: 26/Apr/2003  LAST UPDATE: 13/May/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available
 *              with the source as license.txt or at 
 *              http://www.libstk.org/index.php?page=docs/license
 *****************************************************************************/

#ifndef STK_SPINNER_H
#define STK_SPINNER_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <libstk/rectangle.h>
#include <libstk/container.h>
#include <libstk/list.h>
#include <libstk/list_item.h>

namespace stk
{
    class spinner : public list
    {
    public:
        typedef boost::shared_ptr<spinner> ptr;
        typedef boost::weak_ptr<spinner> weak_ptr;

    private:

    protected:
        spinner(container::ptr parent, const rectangle& rect);
	/// Used to check if a positions lies on the scroll arrows.
	/// The event handler uses this method to check if the mouseclick should be
	/// used to scroll up or down
	bool arrow_clicked(int x,int y);
	

    public:
        static spinner::ptr create(container::ptr parent, const rectangle& rect);
        virtual ~spinner();
		
        /********** EVENT HANDLER INTERFACE **********/
        virtual void handle_event(event::ptr e);
        /********** END EVENT HANDLER INTERFACE **********/

        /********** DRAWABLE INTERFACE **********/
        virtual void draw(surface::ptr surface, const rectangle& clip_rect = rectangle());
        /********** END DRAWABLE INTERFACE **********/

        /********** PARENT INTERFACE **********/
        /// \todo this would have to return a this pointer!!! unless we make list_items a widget and spinner a container!!!
        //virtual widget::ptr focus_next();
        //virtual widget::ptr focus_prev();
        /********** END PARENT INTERFACE **********/

        /********** LIST INTERFACE **********/
        //virtual void add_item(list_item::ptr item);
        /********** END LIST INTERFACE **********/

        /********** SPINNER INTERFACE **********/
        /********** END SPINNER INTERFACE **********/
    };
}

#endif
