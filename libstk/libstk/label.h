/**************************************************************************************************
 *    FILENAME: label.h
 * DESCRIPTION: A simple label widget.
 *     AUTHORS: Darren Hart, Vernon Mauery, Marc Strämke
 *  START DATE: 03/Mar/2003  LAST UPDATE: 13/May/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_LABEL_H
#define STK_LABEL_H

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/signal.hpp>

#include <libstk/widget.h>
#include <libstk/container.h>

namespace stk
{
    class label : public widget
    {
    public:
        typedef boost::shared_ptr<label> ptr;
        typedef boost::weak_ptr<label> weak_ptr;

    private:
        std::wstring text_;

    protected:
        label(container::ptr parent, std::wstring text, const rectangle& rect);

    public:
        static label::ptr create(container::ptr parent, std::wstring text,
                                 const rectangle& rect);
        virtual ~label();

        std::wstring text() const
        {
            return text_;
        }
        void text(const std::wstring& t)
        {
            text_ = t;
        }

        virtual void draw(surface::ptr surface, const rectangle& clip_rect = rectangle());
    };

} // namespace stk

#endif
