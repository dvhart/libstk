/******************************************************************************
 *    FILENAME: sdl_data.cpp 
 * DESCRIPTION: sdl_data implementation 
 *     AUTHORS: Darren Hart 
 *  START DATE: 28/May/2003  LAST UPDATE: 28/May/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available
 *              with the source as license.txt or at 
 *              http://www.libstk.org/index.php?page=docs/license
 *****************************************************************************/

#include <iostream>
#include <SDL/SDL.h>
#include "libstk/sdl_data.h"
#include "libstk/logging.h"

namespace stk
{
    sdl_data::ptr sdl_data::instance_;

    sdl_data::ptr sdl_data::get()
    {
        if (instance_)
            return instance_;
        instance_.reset(new sdl_data());
        return instance_;
    }

    sdl_data::sdl_data() : first_init_(true)
    {
    }

    sdl_data::~sdl_data()
    {
        INFO("sdl_data::~sdl_data()");
        INFO("use_count: " << instance_.use_count());
        SDL_Quit();
    }

    void sdl_data::init()
    {
        if (first_init_)
        {
            INFO("Initializing SDL");
            if (SDL_Init(0) < 0) // FIXME: do we want to use the SDL_EVENT_THREAD flag here ? or joysticks ?
                throw error_message_exception(std::string("Unable to init SDL: ") +
                                              std::string(SDL_GetError()));
            first_init_ = false;
        }
    }

}
