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
#include <boost/thread/recursive_mutex.hpp>
#include <libstk/exceptions.h>

#define SDL_MUTEX_LOCK   boost::recursive_mutex::scoped_lock scoped_lock(sdl_data::get()->mutex);//INFO("SDL_MUTEX_LOCK");
#define SDL_MUTEX_UNLOCK //INFO("SDL_MUTEX_UNLOCK"); 

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
        sdl_data();

    public:
        ~sdl_data();
        /// Get the singleton instance_
        static sdl_data::ptr get();
        /// Initialize SDL with SDL_Init if it hasn't been already
        void init();
        /// Return true if SDL_Init has been called, false otherwise
        bool first_init() const { return first_init_; }
        boost::recursive_mutex mutex;
    };
}

#endif
