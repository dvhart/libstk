/******************************************************************************
 *    FILENAME: event_producer_sdl.cpp 
 * DESCRIPTION: SDL event producer implementation.
 *     AUTHORS: Darren Hart, Marc Straemke
 *  START DATE: 22/Feb/2003  LAST UPDATE: 27/Jul/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available
 *              with the source as license.txt or at 
 *              http://www.libstk.org/index.php?page=docs/license
 *****************************************************************************/

#include <SDL/SDL.h>
#include <iostream>
#include "libstk/exceptions.h"
#include "libstk/event_producer_sdl.h"
#include "libstk/key_event.h"
#include "libstk/mouse_event.h"
#include "libstk/sdl_data.h"
#include "libstk/logging.h"

/* FIXME: SDL reports the non-shifted keysym when shift is held down.  Are we using SDL
 * wrong or do we need to deal with that here, or possibly in the app ?
 * i.e. 3 and Shift-3 both return SDLK_3, rather than SDLK_3 and SDLK_HASH respectively.
 */
namespace stk
{
    event_producer_sdl::ptr event_producer_sdl::create()
    {
        event_producer_sdl::ptr new_event_producer_sdl(new event_producer_sdl());
        event_system::get()->add_producer(new_event_producer_sdl);
        return new_event_producer_sdl;
    }

    event_producer_sdl::event_producer_sdl()
    {
        // make sure SDL has been initialized
        sdl_data_ = sdl_data::get(); // reference counting
        sdl_data_->init();
    }

    event_producer_sdl::~event_producer_sdl()
    {
    }

    boost::shared_ptr<stk::event> event_producer_sdl::poll_event()
    {
        // enter the event loop
        SDL_Event new_event;
        event::ptr event_ = event::create(event::none);
        if (SDL_PollEvent(&new_event))
        {
            switch(new_event.type)
            {
            case SDL_KEYDOWN:
            {
                return key_event::ptr(new key_event(sdl2stk_key(new_event.key.keysym.sym), 
                            event::key_down, sdl2stk_mod(new_event.key.keysym.mod)));
                break;
            }
            case SDL_KEYUP:
            {
                return key_event::ptr(new key_event(sdl2stk_key(new_event.key.keysym.sym),
                            event::key_up, sdl2stk_mod(new_event.key.keysym.mod)));
                break;
            }
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
            default:
                // throw something
                WARN("unknown event type");
                return event::create(event::unknown);
            }
        }
        return event_;
    }
    
    keycode event_producer_sdl::sdl2stk_key(SDLKey sdl_key)
    {
        INFO("Received SDL Key: " << std::hex << sdl_key);
        keycode key;

        // FIXME: fill in the rest of the keys 
        
        // handle key ranges
        // a - z
        if (sdl_key >= SDLK_a && sdl_key <= SDLK_z)
        {
            key = (keycode)(key_a + (keycode)(sdl_key - SDLK_a));
            return key;
        }
        // 0 - 9
        if (sdl_key >= SDLK_0 && sdl_key <= SDLK_9)
        {
            key = (keycode)(key_0 + (keycode)(sdl_key - SDLK_0));
            return key;
        }
        // keypad 0 - keypad 9
        if (sdl_key >= SDLK_KP0 && sdl_key <= SDLK_KP9)
        {
            // FIXME: use stk keypad codes (don't exist atm)
            key = (keycode)(key_0 + (keycode)(sdl_key - SDLK_KP0));
            return key;
        }
        // f1 - f12
        if (sdl_key >= SDLK_F1 && sdl_key <= SDLK_F12)
        {
            key = (keycode)(key_f1 + (keycode)(sdl_key - SDLK_F1));
            return key;
        }
 
        
        switch (sdl_key)
        {
        // FIXME: fill this in, take advantage of ascii codes if possible
        case SDLK_BACKSPACE:
            key = key_backspace;
            break;
        case SDLK_TAB:
            key = key_tab;
            break;
        case SDLK_RETURN:
            key = key_enter;
            break;
        case SDLK_ESCAPE:
            key = key_escape;
            break;
        case SDLK_SPACE:
            key = key_space;
            break;
        case SDLK_DELETE:
            key = key_delete;
            break;

        case SDLK_LEFT:
            key = key_leftarrow;
            break;
        case SDLK_RIGHT:
            key = key_rightarrow;
            break;
        case SDLK_UP:
            key = key_uparrow;
            break;
        case SDLK_DOWN:
            key = key_downarrow;
            break;
        default:
            WARN("unknown key: " << std::hex << sdl_key);
            key = key_unknown;
            break;
        }
        return key;
    }

    modcode event_producer_sdl::sdl2stk_mod(SDLMod sdl_mod)
    {
        modcode modlist = mod_none;
        if (sdl_mod & KMOD_NUM)    modlist = (modcode)(modlist | mod_numlock);
        if (sdl_mod & KMOD_CAPS)   modlist = (modcode)(modlist | mod_capslock);
        if (sdl_mod & KMOD_MODE)   modlist = (modcode)(modlist | mod_mode);
        if (sdl_mod & KMOD_LCTRL)  modlist = (modcode)(modlist | mod_leftcontrol);
        if (sdl_mod & KMOD_RCTRL)  modlist = (modcode)(modlist | mod_rightcontrol);
        if (sdl_mod & KMOD_RSHIFT) modlist = (modcode)(modlist | mod_rightshift);
        if (sdl_mod & KMOD_LSHIFT) modlist = (modcode)(modlist | mod_leftshift);
        if (sdl_mod & KMOD_RALT)   modlist = (modcode)(modlist | mod_rightalt);
        if (sdl_mod & KMOD_LALT)   modlist = (modcode)(modlist | mod_leftalt);
        if (sdl_mod & KMOD_RMETA)  modlist = (modcode)(modlist | mod_rightmeta);
        if (sdl_mod & KMOD_LMETA)  modlist = (modcode)(modlist | mod_leftmeta);
        return modlist;
    }
    
} // namespace stk
