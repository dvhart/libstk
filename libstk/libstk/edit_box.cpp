/******************************************************************************
 *    FILENAME: edit_box.cpp
 * DESCRIPTION: Edit box widget implementation
 *     AUTHORS: Dirk Hoerner
 *  START DATE: 23/Jun/2003  LAST UPDATE: 23/Jun/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available
 *              with the source as license.txt or at 
 *              http://www.libstk.org/index.php?page=docs/license
 *****************************************************************************/

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
        //cout << "button::handle_event()" << endl;
        switch (e->type())
        {
        case event::key_up:
        {
            // FIXME: where should default actions be taken care of ?
            // next and prev are currently in App, so activate (enter) should
            // probably be with them...
            key_event::ptr ke = boost::shared_static_cast<key_event>(e);
            switch ( ke->key() )
            {
            case key_enter:
                if (pressed_)
                {
                    cout << "edit_box::handle_event() - emitting signal on_release()" << endl;
                    pressed_ = false;
                    redraw(rect_);
                    on_release(text_);
                }
                return;
                break;
            case key_backspace:
                INFO("Backspace hit in edit_box");
                if(text_.size()>0)
                    text_.resize(text_.size()-1);
                redraw(rect_);
                return;
                break;
            }

            if( (ke->key() >= key_0 && ke->key() <= key_9) ||
                (ke->key() >= key_A && ke->key() <= key_Z) ||
                (ke->key() >= key_a && ke->key() <= key_z) )
            {
                INFO("Alphanum key hit in edit_box");
                text_+=(wchar_t)ke->key();
                redraw(rect_);
                return;
            }
            
            break; // key_up
        }
        case event::key_down:
        {
            key_event::ptr ke = boost::shared_static_cast<key_event>(e);
            switch ( ke->key() )
            {
            case key_enter:
                cout << "edit_box::handle_event() - emitting signal on_press()" << endl;
                pressed_ = true;
                redraw(rect_);
                on_press(text_);
                return;
                break;
            }
            break; // key_down
        }
        // FIXME: this stuff should be moved to application or state I think
        case event::mouse_motion:
        {
            break; // mouse_motion
        }
        case event::mouse_down:
        {
            mouse_event::ptr me = boost::shared_static_cast<mouse_event>(e);
            if (!pressed())
            {
                pressed_ = true;
                redraw(rect_);
                on_press(text_);
            }
            return;
            break; // mouse_down
        }
        case event::mouse_up:
        {
            mouse_event::ptr me = boost::shared_static_cast<mouse_event>(e);
            if (pressed())
            {
                pressed_ = false;
                redraw(rect_);
                on_release(text_);
            }
            return;
            break; // mouse_up
        }
        }
        // if we haven't handled it, try the defaults and possibly pass it up
        widget::handle_event(e);
    }

    /*
      switch (e->type())
      {
      case event::mouse
    */
}
