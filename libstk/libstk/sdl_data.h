/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: A singleton class used to handle SDL initialization.
 *     AUTHORS: Darren Hart 
 *  START DATE: 2003/May/28
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_SDL_DATA_H
#define STK_SDL_DATA_H

#include <boost/smart_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <libstk/exceptions.h>

#define SDL_MUTEX_LOCK sdl_data::get()->lock_mutex();
#define SDL_MUTEX_UNLOCK sdl_data::get()->unlock_mutex();

namespace stk
{
    class sdl_data
    {
    public:
        typedef boost::shared_ptr<sdl_data> ptr;
        typedef boost::weak_ptr<sdl_data> weak_ptr;

    private:
        /// Whether or not SDL_Init() has been called yet
        bool first_init_;
        /// Singleton instance
        static sdl_data::ptr instance_;
        /// A mutex used to prevent threads for accessing surface data concurrently
        SDL_mutex* mutex_; // really an ugly hack, but SDL isn't meant to be a production surface
        sdl_data();

    public:
        ~sdl_data();
        /// Get the singleton instance_
        static sdl_data::ptr get();
        /// Initialize SDL with SDL_Init if it hasn't been already
        void init();
        /// Return true if SDL_Init has been called, false otherwise
        bool first_init() const { return first_init_; }
        int lock_mutex()
        {
            int ret;
            if ((ret = SDL_mutexP(mutex_)) == -1)
                throw error_message_exception("sdl_data::lock_mutex() - SDL_mutexP() failed");
            return ret;
        }
        int unlock_mutex()
        {
            int ret;
            if ((ret = SDL_mutexV(mutex_)) == -1)
                throw error_message_exception("sdl_data::unlock_mutex() - SDL_mutexV() failed");
            return ret;
        }
    };
}

#endif
