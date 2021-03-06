/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: sdl_data implementation 
 *     AUTHORS: Darren Hart 
 *  START DATE: 2003/May/28
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Str�mke, Dirk H�rner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include <stdlib.h>
#include <iostream>
#include <SDL/SDL.h>
#include "libstk/sdl_data.h"
#include "libstk/logging.h"

namespace stk
{
    sdl_data::weak_ptr sdl_data::instance_;

    sdl_data::ptr sdl_data::get()
    {
        sdl_data::ptr instance = instance_.lock(); 
        if (instance)
            return instance;
        instance.reset(new sdl_data());
        instance_ = instance;
        return instance;
    }

    sdl_data::sdl_data() : first_init_(true)
    {
    }

    sdl_data::~sdl_data()
    {
        INFO("destructor");
        INFO("use_count: " << instance_.lock().use_count());
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
