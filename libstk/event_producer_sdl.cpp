/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: SDL event producer implementation.
 *     AUTHORS: Darren Hart, Marc Strämke
 *  START DATE: 2003/Feb/22
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include <SDL/SDL.h>
#include <iostream>
#include "libstk/exceptions.h"
#include "libstk/event_producer_sdl.h"
#include "libstk/key_event.h"
#include "libstk/mouse_event.h"
#include "libstk/sdl_data.h"
#include "libstk/logging.h"

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
        INFO("Enabling unicode");
        SDL_EnableUNICODE(1);
    }

    event_producer_sdl::~event_producer_sdl()
    {
        INFO("destructor");
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
                return sdl2stk_key_event(new_event.key.keysym, event::key_down);
                //return key_event::ptr(new key_event(sdl2stk_key(new_event.key.keysym.sym), 
                //            event::key_down, sdl2stk_mod(new_event.key.keysym.mod)));
                break;
            case SDL_KEYUP:
                return sdl2stk_key_event(new_event.key.keysym, event::key_up);
                //return key_event::ptr(new key_event(sdl2stk_key(new_event.key.keysym.sym),
                //           event::key_up, sdl2stk_mod(new_event.key.keysym.mod)));
                break;
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
                // throw something once we actually handle all SDL event types
                //WARN("unknown event type: " << std::hex << new_event.type);
                return event::create(event::unknown);
            }
        }
        return event_;
    }
    
    key_event::ptr event_producer_sdl::sdl2stk_key_event(SDL_keysym keysym, event::event_type type)
    {
        keycode fn_code = (keycode)0; // FIXME: default to fn_none or something
        wchar_t character = 0;
        
        // f1 - f12
        if (keysym.sym >= SDLK_F1 && keysym.sym <= SDLK_F12)
        {
            fn_code = (keycode)(key_f1 + (keycode)(keysym.sym - SDLK_F1));
        }
        else
        {
            switch (keysym.sym)
            {
            // FIXME: fill this in

            // common utility keys 
            case SDLK_BACKSPACE:
                fn_code = key_backspace;
                break;
            case SDLK_TAB:
                fn_code = key_tab;
                break;
            case SDLK_RETURN:
                fn_code = key_enter;
                break;
            case SDLK_ESCAPE:
                fn_code = key_escape;
                break;
            case SDLK_SPACE:
                character = ' ';
                break;
            case SDLK_DELETE:
                fn_code = key_delete;
                break;

            // arrow keys
            case SDLK_LEFT:
                fn_code = key_leftarrow;
                break;
            case SDLK_RIGHT:
                fn_code = key_rightarrow;
                break;
            case SDLK_UP:
                fn_code = key_uparrow;
                break;
            case SDLK_DOWN:
                fn_code = key_downarrow;
                break;

            // modifier keys
            case SDLK_RSHIFT:
                fn_code = key_rightshift;
                break;
            case SDLK_LSHIFT:
                fn_code = key_leftshift;
                break;
            case SDLK_RCTRL:
                fn_code = key_rightcontrol;
                break;
            case SDLK_LCTRL:
                fn_code = key_leftcontrol;
                break;
            case SDLK_RALT:
                fn_code = key_rightalt;
                break;
            case SDLK_LALT:
                fn_code = key_leftalt;
                break;
            case SDLK_RMETA:
                fn_code = key_rightmeta;
                break;
            case SDLK_LMETA:
                fn_code = key_leftmeta;
                break;
            case SDLK_RSUPER:
                fn_code = key_rightsuper;
                break;
            case SDLK_LSUPER:
                fn_code = key_leftsuper;
                break;
            }
        }
        
        INFO("Received SDL Key sym=" << std::hex << keysym.sym 
                << " unicode: 0x" << std::hex << keysym.unicode);
        character = (wchar_t)keysym.unicode;
            
        if (fn_code == (keycode)0 && character == 0)
        {
            WARN("unknown key: " << std::hex << keysym.sym);
            fn_code = key_unknown;
        }
        return key_event::ptr(new key_event(fn_code, type, sdl2stk_mod(keysym.mod), character));
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
