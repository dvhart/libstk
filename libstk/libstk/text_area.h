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
#include <libstk/scrollable.h>
#include <libstk/scroll_model.h>
#include <libstk/widget.h>
#include <libstk/container.h>

namespace stk
{
    class text_area : public widget, public scrollable
    {
    public:
        typedef boost::shared_ptr<text_area> ptr;
        typedef boost::weak_ptr<text_area> weak_ptr;
        typedef std::pair<int, int> selection_pair;

    private:
        //functions inplemented in text_area.cpp
        /// return the character index under the x, y coordinate
        int region(int x, int y);
        /// return the starting position of line
        int line_start_position(int line);
        /// return the number of characters in line
        int chars_in_line(int line);
        /// returns the number of lines in text_
        int total_lines();
        /// returns the X,Y cordinates of the cursor (y is at the top of the line)
        void scroll();
        point cursor_position();

        /*
         * draws the text of the text box
         * surface: the surface to draw on
         * text_rect: rectangle of all the text
         * clip_rect: rectangle of area needed to be drawn
         * returns: the x,y point of where the cursor should be drawn.
         * The Y position is at the top of the line where the cursor is on.
         */
        point draw_text(surface::ptr surface, const rectangle& clip_rect);
        std::wstring next_line();

        //functions implemented in the theme
        font::ptr get_font();
        rectangle text_rectangle();
        int line_spacing();

        std::wstring text_;
        int selection_start_;
        int selection_end_;
        int pressed_;
        int line_; //used to store the line of the cursor
        bool line_wrap_;
        bool editable_;
        
        //next_line state variables
        int new_line_;
        std::wstring rest_of_text_;

    protected:
        text_area(const std::wstring& text, const rectangle& rect, bool line_wrap, bool editable);
        void resize();
        boost::signals::connection v_scroll_con_;
        boost::signals::connection h_scroll_con_;
        
    public:
        static text_area::ptr create(container::ptr parent, const std::wstring& text,
                const rectangle& rect, bool line_wrap, bool editable = true);
        virtual ~text_area();

        /***** TEXT AREA INTERFACE *****/
        std::wstring text() const
        {
            return text_;
        }
        void text(const std::wstring& text)
        {
            text_ = text;
            resize();
            scroll();
            redraw(widget::rect());
        }
        bool line_wrap()
        {
            return line_wrap_;
        }
        void line_wrap(bool wrap)
        {
            line_wrap_ = wrap;
        }
        bool editable()
        {
            return editable_;
        }
        void editable(bool edit);

        /*
         * sets the selection
         * start: the character position in the text of the start of the selection
         * end: the character position in the text of the end of text.
         * Set these two to the same value to have no selection.
         * 
         */
         void selection(int start, int end);

         /*
          * returns the start and end position of the selected text.
          * if they are equal then nothing is selected.
          * start is the first value in the pair.
          */
         text_area::selection_pair selection();

         /*
          * returns the selected text, or "" if nothing is selected. 
          */
         std::wstring selected_text();
        
         /***** TEXT AREA INTERFACE *****/

        // signals
        /// called when the text is changed
        boost::signal<bool (std::wstring), combiner::logical_or<bool> > on_change;
        /*** DRAWABLE INTERFACE ****/
        virtual void draw(surface::ptr surface, const rectangle& clip_rect);
        /**** END DRAWABLE INTERFACE ****/

        /**** EVENT HANDLER INTERFACE ****/
        virtual void handle_event(event::ptr e);
        /**** END EVENT HANDLER INTERFACE ****/

        /*** SCROLLABLE INTERFACE ****/
        virtual void v_scroll(scroll_model::ptr model);
        virtual void h_scroll(scroll_model::ptr model);
        /**** END SCROLLABLE INTERFACE ****/
    };
}

#endif
