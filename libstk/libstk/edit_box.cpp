/**************************************************************************************************
 *    FILENAME: edit_box.cpp
 * DESCRIPTION: Edit box widget implementation
 *     AUTHORS: Dirk Hoerner, Darren Hart
 *  START DATE: 23/Jun/2003  LAST UPDATE: 26/Jul/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include <iostream>

#include "libstk/edit_box.h"
#include "libstk/container.h"
#include "libstk/event.h"
#include "libstk/key_event.h"

using std::cout;
using std::endl;

namespace stk
{
    edit_box::ptr edit_box::create(container::ptr parent, const std::wstring& text, 
            const rectangle& rect)
    {
        edit_box::ptr new_edit_box(new edit_box(parent, text, rect));
        parent->add(new_edit_box);
        return new_edit_box;
    }

    edit_box::edit_box(container::ptr parent, const std::wstring& text, const rectangle& rect)
        :	widget(parent, rect), text_(text)
    {
        cout << "edit_box::edit_box()" << endl;
        focusable(true);
    }

    edit_box::~edit_box()
    {
        cout << "edit_box::~edit_box()" << endl;
    }

    // event handler
    void edit_box::handle_event(event::ptr e)
    {
        //cout << "edit_box::handle_event()" << endl;
        switch (e->type())
        {
        case event::key_down:
        {
            key_event::ptr ke = boost::shared_static_cast<key_event>(e);
            switch ( ke->key() )
            {
            case key_enter:
                cout << "edit_box::handle_event() - emitting signal on_confirm()" << endl;
                redraw(rect_);
                on_confirm(text_);
                return;
                break;
            case key_backspace:
                INFO("Backspace hit in edit_box");
                if (text_.size() > 0)
                    text_.resize(text_.size()-1);
                redraw(rect_);
                on_change(text_);
                return;
                break;
            }

            // FIXME: accept other non alphanumeric keys: []-=\;',./`
            // FIXME: how do we want to handle key modifiers ?
            if ( (ke->key() >= key_0 && ke->key() <= key_9) ||
                (ke->key() >= key_A && ke->key() <= key_Z) ||
                (ke->key() >= key_a && ke->key() <= key_z) )
            {
                INFO("Alphanum key hit in edit_box");
                // FIXME: replace a selection of text with the current keystroke
                text_ += (wchar_t)ke->key();
                redraw(rect_);
                on_change(text_);
                return;
            }
            else
                INFO("received unknown key: " << ke->key());
            
            break; // key_up
        }
        case event::mouse_down:
        {
            mouse_event::ptr me = boost::shared_static_cast<mouse_event>(e);
            // FIXME: reposition the curesor
            redraw(rect_);
            return;
            break; // mouse_down
        }
        }
        // if we haven't handled it, try the defaults and possibly pass it up
        widget::handle_event(e);
    }
}
