/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Basic list widget templated with list_item
 *     AUTHORS: Darren Hart
 *  START DATE: 2003/Nov/11
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_LIST_H
#define STK_LIST_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/bind.hpp>

#include <libstk/list_template.h>
#include <libstk/list_item.h>

namespace stk
{
    class list : public list_template<list_item::ptr>
    {
        public:
            typedef boost::shared_ptr<list> ptr;
            typedef boost::weak_ptr<list> weak_ptr;

        protected:
            list(const rectangle& rect) : list_template<list_item::ptr>(rect) { }

        public:
            static list::ptr create(container::ptr parent, const rectangle& rect)
            {
                list::ptr new_list(new list(rect));
                new_list->on_resize.connect(boost::function<bool()>(
                            (boost::bind(&scrollable::update_vis_sizes, new_list.get(), 
                                         boost::bind(&rectangle::height, new_list.get()), 
                                         boost::bind(&rectangle::width, new_list.get())), true)));
                new_list->parent(parent);
                return new_list;
            }
            virtual ~list() { }

            /********** DRAWABLE INTERFACE **********/
            virtual void draw(surface::ptr surface, const rectangle& clip_rect = rectangle());
            /********** END DRAWABLE INTERFACE **********/
    };
}

#endif
