/**************************************************************************************************
 *    FILENAME: overlay_sdl.h
 * DESCRIPTION: A simple class to store YUV overlay_sdl data (memory, size, format)
 *              backends.
 *     AUTHORS: Darren Hart
 *  START DATE: 20/Jul/2003  LAST UPDATE: 20/Jul/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_OVERLAY_SDL_H
#define STK_OVERLAY_SDL_H

#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <libstk/rectangle.h>
#include <libstk/surface.h>
#include <libstk/stk_types.h>
#include <libstk/overlay.h>

#include <SDL/SDL.h>

using std::cout;
using std::endl;

namespace stk
{
    class overlay_sdl : public overlay
    {
    public:
        typedef boost::shared_ptr<overlay_sdl> ptr;
        typedef boost::weak_ptr<overlay_sdl> weak_ptr;

    private:

    protected:
        overlay_sdl(int width, int height, int format, SDL_Surface* sdl_surface)
        {
            cout << "overlay_sdl::overlay_sdl(rect, format, surface)" << endl;
            overlay_impl_ = SDL_CreateYUVOverlay(width, height, format, sdl_surface);
        }

        // temp data (same as SDL structure)
        SDL_Overlay* overlay_impl_;

    public:
        static overlay_sdl::ptr create(int width, int height, int format, SDL_Surface* sdl_surface)
        {
            overlay_sdl::ptr new_overlay_sdl(new overlay_sdl(width, height, format, sdl_surface));
            return new_overlay_sdl;
        }

        virtual ~overlay_sdl()
        {
            cout << "overlay_sdl::~overlay_sdl()" << endl;
            SDL_FreeYUVOverlay(overlay_impl_);
        }

        // inline property methods
        virtual int width() const
        {
            return overlay_impl_->w;
        }
        virtual int height() const
        {
            return overlay_impl_->h;
        }
        virtual int format() const
        {
            return overlay_impl_->format;
        }
        virtual int pitches(int index) const
        {
            return overlay_impl_->pitches[index];
        }
        virtual byte* pixels(int index) const
        {
            return overlay_impl_->pixels[index];
        }

        // methods
        virtual void lock()
        {
            SDL_LockYUVOverlay(overlay_impl_);
        }
        virtual void unlock()
        {
            SDL_UnlockYUVOverlay(overlay_impl_);
        }
        virtual void display(const rectangle& rect)
        {
            SDL_Rect sdl_rect = { rect.x1(), rect.y1(), rect.width(), rect.height() };
            SDL_DisplayYUVOverlay(overlay_impl_, &sdl_rect);
        }
    };

} //end namespace stk

#endif