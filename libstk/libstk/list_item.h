/**************************************************************************************************
 *    FILENAME: list_item.h  
 * DESCRIPTION: A simple item for lists, can be derived to contain images, etc.
 *     AUTHORS: Darren Hart 
 *  START DATE: 03/Mar/2003  LAST UPDATE: 13/May/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef LIST_ITEM_H
#define LIST_ITEM_H

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <libstk/rectangle.h>
#include <libstk/event_handler.h>
#include <libstk/drawable.h>
#include <libstk/event.h>

namespace stk
{
    class list;

    class list_item : public event_handler, public drawable
    {
    public:
        typedef boost::weak_ptr<list_item> weak_ptr;
        typedef boost::shared_ptr<list_item> ptr;

    private:

    protected:
        boost::weak_ptr<list> parent_;
        std::wstring label_;
        std::string data_;
        rectangle rect_;
        list_item(boost::shared_ptr<list> parent, std::wstring label,
                  std::string data);

    public:
        static list_item::ptr create(boost::shared_ptr<list> parent,
                                     std::wstring label, std::string data);
        virtual ~list_item();

        /********** EVENT HANDLER INTERFACE **********/
        virtual void handle_event(event::ptr e);
        /********** END EVENT HANDLER INTERFACE **********/

        /********** DRAWABLE INTERFACE **********/
        virtual surface::ptr surface();
        virtual void draw(surface::ptr surface, const rectangle& clip_rect = rectangle());
        //virtual void redraw(const rectangle& rect);
        /********** END DRAWABLE INTERFACE **********/

        /********** LIST ITEM INTERFACE **********/
        std::wstring label() const
        {
            return label_;
        }
        void label(const std::wstring& label)
        {
            label_ = label;
        }
        std::string data() const
        {
            return data_;
        }
        void data(const std::string& data)
        {
            data_ = data;
        }
        rectangle rect() const
        {
            return rect_;
        }
        void rect(const rectangle& rect)
        {
            rect_ = rect;
        }
        /********** END LIST ITEM INTERFACE **********/
    };
}

#endif
