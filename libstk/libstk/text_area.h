/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: A text edit box widget.
 *     AUTHORS: Chris Slade
 *  START DATE: 2003/Oct/3
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_TEXT_AREA_H
#define STK_TEXT_AREA_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/signal.hpp>

#include <libstk/widget.h>
#include <libstk/container.h>

namespace stk
{
    class text_area : public widget
    {
    public:
        typedef boost::shared_ptr<text_area> ptr;
        typedef boost::weak_ptr<text_area> weak_ptr;

    private:
        /// return the character index under the x, y coordinate
        int region(int x, int y);

        std::wstring text_;
        int selection_start_;
        int selection_end_;
        bool pressed_; //used to tell if selecting

    protected:
        text_area(container::ptr parent, const std::wstring& text, const rectangle& rect);

    public:
        static text_area::ptr create(container::ptr parent, const std::wstring& text,
                const rectangle& rect);

        virtual ~text_area();

        std::wstring text() const
        {
            return text_;
        }
        void text(std::wstring& text)
        {
            text_ = text;
            redraw(rect());
        }

        // signals
        /// called when the text is changed
        boost::signal<bool (std::wstring), combiner::logical_or<bool> > on_change;

        virtual void draw(surface::ptr surface, const rectangle& clip_rect);
        virtual void handle_event(event::ptr e);
    };
}

#endif
