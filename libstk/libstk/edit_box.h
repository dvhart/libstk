/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: An edit box widget.
 *     AUTHORS: Dirk Hörner, Darren Hart, Chris Slade
 *  START DATE: 2003/Jun/23
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_EDIT_BOX_H
#define STK_EDIT_BOX_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/signal.hpp>

#include <libstk/widget.h>
#include <libstk/container.h>

namespace stk
{
    class edit_box : public widget
    {
    public:
        typedef boost::shared_ptr<edit_box> ptr;
        typedef boost::weak_ptr<edit_box> weak_ptr;
        typedef std::pair<int, int> selection_pair;

    private:
        /// return the character index under the x, y coordinate
        int region(int x, int y);

        std::wstring text_;
        int selection_start_;
        int selection_end_;
        bool pressed_; //used to tell if selecting

    protected:
        edit_box(const std::wstring& text, const rectangle& rect);

    public:
        //static edit_box::ptr create(container::ptr parent, const rectangle& rect);
        static edit_box::ptr create(container::ptr parent, const std::wstring& text,
                const rectangle& rect);

        virtual ~edit_box();

        std::wstring text() const
        {
            return text_;
        }
        void text(std::wstring& text)
        {
            text_ = text;
            // Usually keeping the selection when putting a new text in is not a good idea
            selection_start_=0;
            selection_end_=0;
            redraw(rect());
        }

        // signals
        /// called when the text is changed
        boost::signal<bool (std::wstring), combiner::logical_or<bool> > on_change;
        /// called when the user presses enter in the box
        /// FIXME: what is a better name for this signal ?
        boost::signal<bool (std::wstring), combiner::logical_or<bool> > on_confirm;

        virtual void draw(surface::ptr surface, const rectangle& clip_rect);
        virtual void handle_event(event::ptr e);

        //SELECTION API
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
         edit_box::selection_pair selection();

         /*
          * returns the selected text, or "" if nothing is selected. 
          */
         std::wstring selected_text();

    };
}

#endif
