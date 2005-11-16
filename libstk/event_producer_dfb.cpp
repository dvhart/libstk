/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Directfb event producer backend.
 *     AUTHORS: Marc Strämke
 *  START DATE: 2004/Aug/21
 *
 *   COPYRIGHT: 2004 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include "event_producer_dfb.h"
#include "libstk/backend_dfb.h"
#include "libstk/mouse_event.h"
#include "libstk/logging.h"



namespace stk
{

    static keycode dfb_keycode_table[] =
    {
        key_null,
        key_a,
        key_b,
        key_c,
        key_d,
        key_e,
        key_f,
        key_g,
        key_h,
        key_i,
        key_j,
        key_k,
        key_l,
        key_m,
        key_n,
        key_o,
        key_p,
        key_q,
        key_r,
        key_s,
        key_t,
        key_u,
        key_v,
        key_w,
        key_x,
        key_y,
        key_z,
        key_0,
        key_1,
        key_2,
        key_3,
        key_4,
        key_5,
        key_6,
        key_7,
        key_8,
        key_9,
        key_f1,
        key_f2,
        key_f3,
        key_f4,
        key_f5,
        key_f6,
        key_f7,
        key_f8,
        key_f9,
        key_f10,
        key_f11,
        key_f12,
        key_leftshift,
        key_rightshift,
        key_leftcontrol,
        key_rightcontrol,
        key_leftsuper,
        key_rightsuper,
        key_null,               // HYPER_L
        key_null,               // HYPER_R
        key_capslock,
        key_numlock,
        key_scrolllock,
        key_escape,
        key_leftarrow,
        key_rightarrow,
        key_uparrow,
        key_downarrow,
        key_tab,
        key_enter,
        key_space,
        key_backspace,
        key_insert,
        key_delete,
        key_home,
        key_end,
        key_pageup,
        key_pagedown,
        key_print,
        key_pause
    };
    keycode get_keycode_from_dfb(int dfb_keycode)
    {
        switch(dfb_keycode)
        {
        case DIKI_A: 	
        case DIKI_B: 	
        case DIKI_C:
        case DIKI_D:
        case DIKI_E:
        case DIKI_F:	
 	case DIKI_G: 	
 	case DIKI_H: 	
 	case DIKI_I: 	
 	case DIKI_J: 	
 	case DIKI_K: 	
 	case DIKI_L: 	
 	case DIKI_M: 	
 	case DIKI_N: 	
 	case DIKI_O: 	
 	case DIKI_P: 	
 	case DIKI_Q: 	
 	case DIKI_R: 	
 	case DIKI_S: 	
 	case DIKI_T: 	
 	case DIKI_U: 	
 	case DIKI_V: 	
 	case DIKI_W: 	
 	case DIKI_X: 	
 	case DIKI_Y: 	
 	case DIKI_Z:
            //FIXME isnt handled right by widgets!
            //return static_cast<keycode>(key_a+(dfb_keycode-DIKI_A));
            return key_null;
            break;
 	case DIKI_0: 	
 	case DIKI_1: 	
 	case DIKI_2: 	
 	case DIKI_3: 	
 	case DIKI_4: 	
 	case DIKI_5: 	
 	case DIKI_6: 	
 	case DIKI_7: 	
 	case DIKI_8: 	
 	case DIKI_9:
            //return static_cast<keycode>(key_0+(dfb_keycode-DIKI_0));
            return key_null;
            break;
 	case DIKI_F1: 	
 	case DIKI_F2: 	
 	case DIKI_F3: 	
 	case DIKI_F4:	
 	case DIKI_F5:	
 	case DIKI_F6: 	
 	case DIKI_F7: 	
 	case DIKI_F8: 	
 	case DIKI_F9: 	
 	case DIKI_F10: 	
 	case DIKI_F11: 	
 	case DIKI_F12:
            return static_cast<keycode>(key_f1+(dfb_keycode-DIKI_F1));
            break;
/* 	DIKI_SHIFT_L 	
 	DIKI_SHIFT_R 	
 	DIKI_CONTROL_L 	
 	DIKI_CONTROL_R 	
 	DIKI_ALT_L 	
 	DIKI_ALT_R 	
 	DIKI_ALTGR 	
 	DIKI_META_L 	
 	DIKI_META_R 	
 	DIKI_SUPER_L 	
 	DIKI_SUPER_R 	
 	DIKI_HYPER_L 	
 	DIKI_HYPER_R 	
 	DIKI_CAPS_LOCK 	
 	DIKI_NUM_LOCK 	
 	DIKI_SCROLL_LOCK */	
 	case DIKI_ESCAPE:
            return key_escape;
            break;
        case DIKI_LEFT:
            return key_leftarrow;
            break;
 	case DIKI_RIGHT:
            return key_rightarrow;
            break;
 	case DIKI_UP:
            return key_uparrow;
            break;
 	case DIKI_DOWN:
            return key_downarrow;
            break;	
 	case DIKI_TAB:
            return key_tab;
            break;
 	case DIKI_ENTER:
            return key_enter;
            break;	
 	case DIKI_SPACE:
//            return key_space;
            return key_null;
            break;	
 	case DIKI_BACKSPACE:
            return key_backspace;
            break;
                  /*
 	DIKI_INSERT 	
 	DIKI_DELETE 	
 	DIKI_HOME 	
 	DIKI_END 	
 	DIKI_PAGE_UP 	
 	DIKI_PAGE_DOWN 	
 	DIKI_PRINT 	
 	DIKI_PAUSE 	
 	DIKI_QUOTE_LEFT 	TLDE
 	DIKI_MINUS_SIGN 	AE11
 	DIKI_EQUALS_SIGN 	AE12
 	DIKI_BRACKET_LEFT 	AD11
 	DIKI_BRACKET_RIGHT 	AD12
 	DIKI_BACKSLASH 	BKSL
 	DIKI_SEMICOLON 	AC10
 	DIKI_QUOTE_RIGHT 	AC11
 	DIKI_COMMA 	AB08
 	DIKI_PERIOD 	AB09
 	DIKI_SLASH 	AB10
 	DIKI_LESS_SIGN 	103rd
 	DIKI_KP_DIV 	
 	DIKI_KP_MULT 	
 	DIKI_KP_MINUS 	
 	DIKI_KP_PLUS 	
 	DIKI_KP_ENTER 	
 	DIKI_KP_SPACE 	
 	DIKI_KP_TAB 	
 	DIKI_KP_F1 	
 	DIKI_KP_F2 	
 	DIKI_KP_F3 	
 	DIKI_KP_F4 	
 	DIKI_KP_EQUAL 	
 	DIKI_KP_SEPARATOR 	
 	DIKI_KP_DECIMAL 	
 	DIKI_KP_0 	
 	DIKI_KP_1 	
 	DIKI_KP_2 	
 	DIKI_KP_3 	
 	DIKI_KP_4 	
 	DIKI_KP_5 	
 	DIKI_KP_6 	
 	DIKI_KP_7 	
 	DIKI_KP_8 	
 	DIKI_KP_9 	
 	DIKI_KEYDEF_END */	
        default:
            return key_null;
        };
    }
    
    event_producer_dfb::ptr event_producer_dfb::create()
    {
        event_producer_dfb::ptr new_event_producer(new event_producer_dfb());
        event_system::get()->add_producer(new_event_producer);
        return new_event_producer;
    }
    event_producer_dfb::event_producer_dfb()
    {
        backend_handle = backend_dfb::get();
        IDirectFB* dfb=backend_handle->get_interface();
        dfb->CreateInputEventBuffer(dfb, DICAPS_ALL, DFB_TRUE, &event_buffer);
    }
    event_producer_dfb::~event_producer_dfb()
    {
        event_buffer->Release(event_buffer);
    }
    event::ptr event_producer_dfb::poll_event()
    {
        static int axis_pos[32];
        event::ptr event_ = event::create(event::none);
        if(event_buffer->HasEvent(event_buffer)==DFB_OK)
        {
            DFBEvent event;
            DFBInputEvent& inputevent=event.input;
            event_buffer->GetEvent(event_buffer, &event);
            assert(event.clazz==DFEC_INPUT);
            switch(inputevent.type)
            { 
            case DIET_KEYPRESS:
                event_.reset(new key_event(get_keycode_from_dfb(inputevent.key_id),
                                           key_event::key_down,
                                           mod_none, inputevent.key_symbol));
                break;
            case DIET_KEYRELEASE:
                event_.reset(new key_event(get_keycode_from_dfb(inputevent.key_id),
                                           key_event::key_up,
                                           mod_none, inputevent.key_symbol));
                break;
            case DIET_BUTTONPRESS:
                event_.reset(new mouse_event(axis_pos[0], axis_pos[1], inputevent.button, event::mouse_down));
                break;
            case DIET_BUTTONRELEASE:
                event_.reset(new mouse_event(axis_pos[0], axis_pos[1], inputevent.button, event::mouse_up));
                break;
            case DIET_AXISMOTION:            
                switch(inputevent.axis)
                {
                case DIAI_X:
                    if(inputevent.flags&DIEF_AXISABS)
                        axis_pos[0]=inputevent.axisabs;
                    else
                        axis_pos[0]+=inputevent.axisrel;
                    if(axis_pos[0]<0)
                        axis_pos[0]=0;
                    break;
                case DIAI_Y:
                    if(inputevent.flags&DIEF_AXISABS)
                        axis_pos[1]=inputevent.axisabs;
                    else
                        axis_pos[1]+=inputevent.axisrel;
                    if(axis_pos[1]<0)
                        axis_pos[1]=0;
                    break;
                default:
                    axis_pos[inputevent.axis-DIAI_X]=inputevent.axisabs;
                }
                event_.reset(new mouse_event(axis_pos[0], axis_pos[1], 0, event::mouse_motion));
                break;
            }
        }
        return event_;
    }
}
