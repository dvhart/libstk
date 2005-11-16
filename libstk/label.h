/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: A simple label widget.
 *     AUTHORS: Darren Hart, Vernon Mauery, Marc Strämke
 *  START DATE: 2003/Mar/03
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_LABEL_H
#define STK_LABEL_H

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/signal.hpp>
#include <boost/optional.hpp>

#include <libstk/widget.h>
#include <libstk/container.h>
#include <libstk/font_properties.h>

namespace stk
{
    class label : public widget
    {
    public:
        typedef boost::shared_ptr<label> ptr;
        typedef boost::weak_ptr<label> weak_ptr;

    private:
        std::wstring text_;
        font_properties font_props_;
        en_horizontal_alignment horizontal_alignment_;
    protected:
        label(std::wstring text, const rectangle& rect);

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
            redraw(rect());
        }

        /// Read the value Horizontal Alignment Property 
        en_horizontal_alignment horizontal_alignment() const
        {
            return horizontal_alignment_;
        }
        /// Write/Set the value Horizontal Alignment Property 
        void horizontal_alignment(en_horizontal_alignment new_alignment) 
        {
            horizontal_alignment_=new_alignment;
            redraw(rect());
        }
        

        /// Get and set the font properties object, any of the properties may be unspecified
        font_properties font_props() { return font_props_; }
        void font_props(font_properties props)
        {
            font_props_ = props;
            redraw(rect());
        }
        
        virtual void draw(surface::ptr surface, const rectangle& clip_rect = rectangle());
    };

} // namespace stk

#endif
