/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: A text edit box widget.
 *     AUTHORS: Chris Slade
 *  START DATE: 2003/Oct/3
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Str�mke, Dirk H�rner
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
        

        //functions inplemented in text_area.cpp
        /// return the character index under the x, y coordinate
        int region(int x, int y);
        /// return the starting position of line
        int line_start_position(int line);
        /// return the number of characters in line
        int chars_in_line(int line);
        point draw_text(surface::ptr surface, const rectangle& text_rect, const rectangle& clip_rect);
        std::wstring next_line();

        //functions implemented in the theme
        font::ptr get_font();
        int text_width();
        int line_spacing();

        std::wstring text_;
        int selection_start_;
        int selection_end_;
        int pressed_;
        int line_; //used to store the line of the cursor

        //next_line state variables
        int new_line_;
        std::wstring rest_of_text_;

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
