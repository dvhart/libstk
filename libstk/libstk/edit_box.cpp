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
        focusable(true);
    }

    edit_box::~edit_box()
    {
    }

    // event handler
    void edit_box::handle_event(event::ptr e)
    {
        switch (e->type())
        {
        case event::key_down:
        {
            key_event::ptr ke = boost::shared_static_cast<key_event>(e);
            switch ( ke->key() )
            {
             case key_backspace:
                if (text_.size() > 0) text_.resize(text_.size()-1);
                redraw(rect_);
                on_change(text_);
                return;
                break;
             case key_delete:
                return;
                INFO("Delete not implemented");
                break;
             case key_enter:
                redraw(rect_);
                on_confirm(text_);
                return;
                break;
            }

            // handle ascii keys 
            if (ke->key() > key_backspace && ke->key() < key_delete)
            {
                INFO("ASCII key received");
                // FIXME: if text is selected, replace it with the current keystroke character 
                wchar_t new_char = (wchar_t)ke->key();
                INFO("Current modifier: " << std::hex << ke->modlist());
                if ((ke->modlist() & mod_shift) && (ke->key() >= key_a && ke->key() <= key_z)) 
                    new_char += (wchar_t)('A' - 'a');
                text_ += new_char;
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
