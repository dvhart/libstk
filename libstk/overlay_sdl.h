/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: A simple class to store YUV overlay_sdl data (memory, size, format)
 *              backends.
 *     AUTHORS: Darren Hart
 *  START DATE: 2003/Jul/20
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Str�mke, Dirk H�rner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_OVERLAY_SDL_H
#define STK_OVERLAY_SDL_H

#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <libstk/rectangle.h>
#include <libstk/surface.h>
#include <libstk/stk_types.h>
#include <libstk/overlay.h>

#include <SDL/SDL.h>

namespace stk
{
    class overlay_sdl : public overlay
    {
    public:
        typedef boost::shared_ptr<overlay_sdl> ptr;
        typedef boost::weak_ptr<overlay_sdl> weak_ptr;

    private:
        SDL_Overlay* overlay_impl_;

        overlay_sdl(int width, int height, int format, SDL_Surface* sdl_surface)
        {
            SDL_MUTEX_LOCK;
            overlay_impl_ = SDL_CreateYUVOverlay(width, height, format, sdl_surface);
            if (!overlay_impl_)
                ERROR("SDL_CreateYUVOverlay failed");
            SDL_MUTEX_UNLOCK;
        }

    public:
        static overlay_sdl::ptr create(int width, int height, int format, SDL_Surface* sdl_surface)
        {
            overlay_sdl::ptr new_overlay_sdl(new overlay_sdl(width, height, format, sdl_surface));
            return new_overlay_sdl;
        }

        virtual ~overlay_sdl()
        {
            //INFO("destructor");
            SDL_MUTEX_LOCK;
            SDL_FreeYUVOverlay(overlay_impl_);
            SDL_MUTEX_UNLOCK;
        }

        // inline property methods
        virtual int width() const { return overlay_impl_->w; }
        virtual int height() const { return overlay_impl_->h; }
        virtual int format() const { return overlay_impl_->format; }
        virtual int pitches(int index) const { return overlay_impl_->pitches[index]; }
        virtual byte* pixels(int index) const { return overlay_impl_->pixels[index]; }

        // methods
        virtual void lock() 
        { 
            SDL_MUTEX_LOCK;
            SDL_LockYUVOverlay(overlay_impl_); 
            SDL_MUTEX_UNLOCK;
        }
        virtual void unlock() 
        { 
            SDL_MUTEX_LOCK;
            SDL_UnlockYUVOverlay(overlay_impl_); 
            SDL_MUTEX_UNLOCK;
        }
        virtual void display(const rectangle& rect)
        {
            SDL_MUTEX_LOCK;
            SDL_Rect sdl_rect = { rect.x1(), rect.y1(), rect.width(), rect.height() };
            SDL_DisplayYUVOverlay(overlay_impl_, &sdl_rect);
            SDL_MUTEX_UNLOCK;
        }
    };

} //end namespace stk

#endif
