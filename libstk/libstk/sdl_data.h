/**************************************************************************************************
 *    FILENAME: sdl_data.h (FIXME: new name welcome!) 
 * DESCRIPTION: A singleton class used to handle SDL initialization.
 *     AUTHORS: Darren Hart 
 *  START DATE: 28/May/2003  LAST UPDATE: 26/Jul/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_SDL_DATA_H
#define STK_SDL_DATA_H

#include <boost/smart_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <libstk/exceptions.h>

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
        sdl_data();

    public:
        ~sdl_data();
        /// Get the singleton instance_
        static sdl_data::ptr get
            ();
        /// Initialize SDL with SDL_Init if it hasn't been already
        void init();
        /// Return true if SDL_Init has been called, false otherwise
        bool first_init() const
        {
            return first_init_;
        }
    };
}

#endif
