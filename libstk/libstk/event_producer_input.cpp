/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Linux input event producer implementation.
 *     AUTHORS: Darren Hart
 *  START DATE: 2004/Apr/22
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <iostream>

#include "libstk/exceptions.h"
#include "libstk/event_producer_input.h"
#include "libstk/key_event.h"
#include "libstk/mouse_event.h"
#include "libstk/logging.h"

namespace stk
{
    event_producer_input::ptr event_producer_input::create(const std::string& device_path)
    {
        event_producer_input::ptr new_event_producer_input(new event_producer_input(device_path));
        event_system::get()->add_producer(new_event_producer_input);
        return new_event_producer_input;
    }

    event_producer_input::event_producer_input(const std::string& device_path)
    {
        INFO("constructor");
        // attempt to open the specified device, throw exception on failure
        //device_.open(device_path.c_str(), std::ios::in);
        device_fd_ = open(device_path.c_str(), O_RDONLY | O_NONBLOCK);
        // FIXME: use an exception
        //if (!device_)
        if (device_fd_ == -1)
            throw std::string("Error: failed to open device: " + device_path);
    }

    event_producer_input::~event_producer_input()
    {
        INFO("destructor");
    }

    boost::shared_ptr<stk::event> event_producer_input::poll_event()
    {
        // enter the event loop
        input_event new_event;
        event::ptr event_ = event::create(event::none);

        // the event system only reads whole events at a time
        if (read(device_fd_, &new_event, sizeof(input_event)) == sizeof(input_event))
        {
            INFO("Read an event, type: 0x" << std::hex << new_event.type);
            switch(new_event.type)
            {
/* input event types
EV_SYN
EV_KEY
EV_REL
EV_ABS 
EV_MSC  
EV_LED   
EV_SND    
EV_REP     
EV_FF       
EV_PWR       
EV_FF_STATUS  
EV_MAX 
*/
            case EV_KEY:
                INFO("key event read");
                switch (new_event.value)
                {
                    case 0: // key_up
                        return input2stk_key_event(new_event, event::key_up);
                        break;
                    case 1: // key_down
                        return input2stk_key_event(new_event, event::key_down);
                        break;
                    case 2: // key_repeat
                        // FIXME: is this how we want libstk to handle key_repeat events?
                        return input2stk_key_event(new_event, event::key_up);
                        break;
                    //return key_event::ptr(new key_event(sdl2stk_key(new_event.key.keysym.sym), 
                    //            event::key_down, sdl2stk_mod(new_event.key.keysym.mod)));
                }
                break;
            /*
            case SDL_MOUSEBUTTONDOWN:
                return mouse_event::ptr(new mouse_event(new_event.button.x, new_event.button.y,
                            new_event.button.button, event::mouse_down));
                break;
            case SDL_MOUSEBUTTONUP:
                return mouse_event::ptr(new mouse_event(new_event.button.x, new_event.button.y,
                            new_event.button.button, event::mouse_up));
                break;
            case SDL_MOUSEMOTION:
                return mouse_event::ptr(new mouse_event(new_event.motion.x, new_event.motion.y,
                            -1, event::mouse_motion));
                break;
            case SDL_QUIT:
                return event::create(event::quit);
                break;
            */
            default:
                // throw something once we actually handle all SDL event types
                //WARN("unknown event type: " << std::hex << new_event.type);
                return event::create(event::unknown);
            }
        }
        return event_;
    }
    
    key_event::ptr event_producer_input::input2stk_key_event(const input_event& ev, 
            stk::event::event_type type)
    {
        keycode fn_code = (keycode)0; // FIXME: default to fn_none or something
        wchar_t character = 0;
        
        // f1 - f12
        if (ev.code >= KEY_F1 && ev.code <= KEY_F10)
        {
            fn_code = (keycode)(key_f1 + (keycode)(ev.code - KEY_F1));
        }
        else if (ev.code >= KEY_F11 && ev.code <= KEY_F12)
        {
            fn_code = (keycode)(key_f11 + (keycode)(ev.code - KEY_F11));
        }
        else
        {
            switch (ev.code)
            {
            // FIXME: fill this in

            // common utility keys 
            case KEY_ESC:
                fn_code = key_escape;
                break;
            case KEY_BACKSPACE:
                fn_code = key_backspace;
                break;
            case KEY_TAB:
                fn_code = key_tab;
                break;
            case KEY_ENTER:
                fn_code = key_enter;
                break;
            case KEY_SPACE:
                character = ' ';
                break;
            case KEY_DELETE:
                fn_code = key_delete;
                break;

            // arrow keys
            case KEY_UP:
                fn_code = key_uparrow;
                break;
            case KEY_LEFT:
                fn_code = key_leftarrow;
                break;
            case KEY_RIGHT:
                fn_code = key_rightarrow;
                break;
            case KEY_DOWN:
                fn_code = key_downarrow;
                break;

            // modifier keys
            case KEY_LEFTSHIFT:
                fn_code = key_leftshift;
                break;
            case KEY_RIGHTSHIFT:
                fn_code = key_rightshift;
                break;
            case KEY_LEFTCTRL:
                fn_code = key_leftcontrol;
                break;
            case KEY_RIGHTCTRL:
                fn_code = key_rightcontrol;
                break;
            case KEY_LEFTALT:
                fn_code = key_leftalt;
                break;
            case KEY_RIGHTALT:
                fn_code = key_rightalt;
                break;
            case KEY_LEFTMETA:
                fn_code = key_leftmeta;
                break;
            case KEY_RIGHTMETA:
                fn_code = key_rightmeta;
                break;
            }
        }
        
        // character = L"A";
        //INFO("Received Input Key value=" << std::hex << ev.code;
        //character = (wchar_t)keysym.unicode;
            
        if (fn_code == (keycode)0 && character == 0)
        {
            WARN("unknown key: " << std::hex << ev.code);
            fn_code = key_unknown;
        }
        return key_event::ptr(new key_event(fn_code, type, input2stk_mod(ev.code), character));
    }

    modcode event_producer_input::input2stk_mod(__u16 input_mod)
    {
        modcode modlist = mod_none;
        if (input_mod & KEY_NUMLOCK)    modlist = (modcode)(modlist | mod_numlock);
        if (input_mod & KEY_CAPSLOCK)   modlist = (modcode)(modlist | mod_capslock);
        if (input_mod & KEY_MODE)       modlist = (modcode)(modlist | mod_mode);
        if (input_mod & KEY_RIGHTCTRL)  modlist = (modcode)(modlist | mod_rightcontrol);
        if (input_mod & KEY_LEFTCTRL)   modlist = (modcode)(modlist | mod_leftcontrol);
        if (input_mod & KEY_RIGHTSHIFT) modlist = (modcode)(modlist | mod_rightshift);
        if (input_mod & KEY_LEFTSHIFT)  modlist = (modcode)(modlist | mod_leftshift);
        if (input_mod & KEY_RIGHTALT)   modlist = (modcode)(modlist | mod_rightalt);
        if (input_mod & KEY_LEFTALT)    modlist = (modcode)(modlist | mod_leftalt);
        if (input_mod & KEY_RIGHTMETA)  modlist = (modcode)(modlist | mod_rightmeta);
        if (input_mod & KEY_LEFTMETA)   modlist = (modcode)(modlist | mod_leftmeta);
        return modlist;
    }
    
} // namespace stk
