/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: A simple class to store YUV overlay_dfb data (memory, size, format)
 *     AUTHORS: Darren Hart
 *  START DATE: 2003/Jul/23
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_OVERLAY_DFB_H
#define STK_OVERLAY_DFB_H

#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <libstk/rectangle.h>
#include <libstk/surface.h>
#include <libstk/stk_types.h>
#include <libstk/overlay.h>
#include "libstk/backend_dfb.h"

using std::cout;
using std::endl;

// FIXME: do this differently (ie without macros!)
#define DFBCHECK(x...)                                         \
  {                                                            \
    DFBResult err = x;                                         \
							       \
    if (err != DFB_OK)                                         \
      {                                                        \
	ERROR("DFBCHECK FAILED");                              \
	DirectFBErrorFatal( #x, err );                         \
      }                                                        \
  }



namespace stk
{
    class overlay_dfb : public overlay
    {
    public:
        typedef boost::shared_ptr<overlay_dfb> ptr;
        typedef boost::weak_ptr<overlay_dfb> weak_ptr;

    private:
        IDirectFB* dfb_;
        IDirectFBDisplayLayer* layer_;
        IDirectFBSurface* surface_;
        int pitch_;
        byte* pixels_;
        int width_, height_, format_;
        bool locked_;

        overlay_dfb(int width, int height, int format) 
            : width_(width), height_(height), locked_(false), format_(format)
        {
            INFO("overlay_dfb::overlay_dfb()")
            
            dfb_ = backend_dfb::get()->get_interface();
            // FIXME: how do we know what layer we want? guessing 0.
            DFBCHECK(dfb_->GetDisplayLayer(dfb_, 0, &layer_));
            DFBCHECK(layer_->SetCooperativeLevel(layer_, DLSCL_ADMINISTRATIVE));
                
            DFBSurfaceDescription s_dsc;
            //DFBDisplayLayerConfig l_dsc;
            switch(format_) {
                case STK_FORMAT_YV12:
                    s_dsc.pixelformat = DSPF_YV12;
                    //l_dsc.pixelformat = DSPF_YV12;
                    break;
                case STK_FORMAT_YUY2:
                    s_dsc.pixelformat = DSPF_YUY2;
                    //l_dsc.pixelformat = DSPF_YUY2;
                    break;
                default:
                    ERROR("Error unknown image format (" << format << "), assuming YV12");
                    s_dsc.pixelformat = DSPF_YV12;
                    //l_dsc.pixelformat = DSPF_YV12;
            }

            s_dsc.flags = (DFBSurfaceDescriptionFlags)(DSDESC_CAPS | DSDESC_PIXELFORMAT | 
                    DSDESC_WIDTH | DSDESC_HEIGHT);
            s_dsc.caps =(DFBSurfaceCapabilities)(0);
            s_dsc.width = width;
            s_dsc.height = height;
            cout << "CREATE YUV SURFACE" << endl;    
            DFBCHECK(dfb_->CreateSurface(dfb_, &s_dsc, &surface_));

            /*
            l_dsc.flags = (DFBDisplayLayerConfigFlags)(DLCONF_WIDTH | DLCONF_HEIGHT | 
                    DLCONF_PIXELFORMAT | DLCONF_OPTIONS);
            l_dsc.width = width;
            l_dsc.height = height;
            l_dsc.options = (DFBDisplayLayerOptions)(0);
            */

            cout << "TESTING OVERLAY CONFIGURATION" << endl;
            /*
            DFBDisplayLayerConfigFlags failed;
            int ret = layer_->TestConfiguration(layer_, &l_dsc, &failed );
            if (ret == DFB_UNSUPPORTED) {
                throw error_message_exception("TestConfiguration failed while creating DFB Overlay");
            }
            cout << "SETTING LAYER CONFIGURATION" << endl;    
            DFBCHECK(layer_->SetConfiguration(layer_, &l_dsc));
            */
        }

    public:
        static overlay_dfb::ptr create(int width, int height, int format)
        {
            overlay_dfb::ptr new_overlay_dfb(new overlay_dfb(width, height, format));
            return new_overlay_dfb;
        }

        virtual ~overlay_dfb()
        {
            cout << "overlay_dfb::~overlay_dfb()" << endl;
            if (locked_) unlock();
            DFBCHECK(surface_->Release(surface_));
            surface_ = NULL;
        }

        // inline property methods
        virtual int width() const
        {
            return width_;
        }
        virtual int height() const
        {
            return height_;
        }
        virtual int format() const
        {
            return format_;
        }
        virtual int pitches(int index) const
        {
            int ret = 0;
            switch (format_)
            {
                case STK_FORMAT_YV12:
                    if (index == 0) ret = pitch_;
                    else ret = pitch_/2;
                    break;
                case STK_FORMAT_YUY2:
                    ret = 2*pitch_;
                    break;
                default:
                    // FIXME: throw something
                    break;
            }
            return ret;
        }
        
        virtual byte* pixels(int index) const
        {
            byte* ret = NULL;
            switch (format_)
            {
                case STK_FORMAT_YV12:
                    switch (index)
                    {
                        case 0:
                            ret = pixels_;
                            break;
                        case 1:
                            ret = pixels_ + (pitch_ * height_);
                            break;
                        case 2:
                            ret = pixels_ + (pitch_ * height_) + (pitch_ * height_ / 4);
                            break;
                        default:
                            // FIXME: throw something
                            break;
                    }
                    break;
                case STK_FORMAT_YUY2:
                    ret = pixels_;
                    break;
                default:
                    // FIXME: throw something
                    break;
            }
            return ret;
        }

        // methods
        virtual void lock()
        {
            DFBCHECK(surface_->Lock(surface_, DSLF_WRITE, (void**)(&pixels_), &pitch_));
            // FIXME: the dfb xine driver does this memset, do we want to ?
            memset(pixels_, 128, 6 * width_ * height_ / 4);
            locked_ = true;
        }
        virtual void unlock()
        {
            if (locked_)
            {
                surface_->Unlock(surface_);
                locked_ = false;
            }
        }

        virtual void display(const rectangle& rect)
        {
            cout << "DISPLAY" << endl;
            IDirectFBSurface *screen;
            DFBCHECK(layer_->GetSurface(layer_, &screen));
            DFBCHECK(screen->Blit(screen, surface_, NULL, rect.x1(), rect.y1()));
            //DFBCHECK(surface_->Flip(surface_, NULL, (DFBSurfaceFlipFlags)(0)));
        }
    };

} //end namespace stk

#endif
