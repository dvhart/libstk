/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: A templated list item providing access to and storage of a user data type
 *     AUTHORS: Darren Hart
 *  START DATE: 2003/Nov/21
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef LIST_ITEM_USERDATA
#define LIST_ITEM_USERDATA

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <libstk/list_template.h>
#include <libstk/list_item.h>

namespace stk
{
    template<class Tdata>
    class list_item_userdata : public list_item
    {
    public:
        typedef boost::shared_ptr<list_item_userdata<Tdata> > ptr;
        typedef boost::weak_ptr<list_item_userdata<Tdata> > weak_ptr;

    protected:
        list_item_userdata(std::wstring label) : list_item(label) { INFO("constructor"); }
        Tdata userdata_;

    public:
        static typename list_item_userdata<Tdata>::ptr create(
                typename list_template<list_item_userdata<Tdata> >::ptr parent, std::wstring label)
        {
            typename list_item_userdata<Tdata>::ptr new_list_item_userdata(
                    new list_item_userdata<Tdata>(label));
            parent->add_item(new_list_item_userdata);
            new_list_item_userdata->parent_ = parent;
            return new_list_item_userdata;
        }

        static typename list_item_userdata<Tdata>::ptr create(list::ptr parent, std::wstring label)
        {
            typename list_item_userdata<Tdata>::ptr new_list_item_userdata(
                    new list_item_userdata<Tdata>(label));
            parent->add_item(new_list_item_userdata);
            new_list_item_userdata->parent_ = parent;
            return new_list_item_userdata;
        }


        virtual ~list_item_userdata() { INFO("destructor"); }

        /********** LIST ITEM USERDATA INTERFACE **********/
        virtual Tdata userdata() { return userdata_; }
        virtual void userdata(Tdata data) { userdata_ = data; }
        /********** END LIST ITEM USERDATA INTERFACE **********/
    }; // class list_item_userdata
} // namespace stk

#endif

