/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Xine media panel implementation
 *     AUTHORS: Darren Hart
 *  START DATE: 2003/Jul/21
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include <iostream>
#include "libstk/xine_panel.h"

void xine_event_listener(void *user_data, const xine_event_t *event) {
}

namespace stk
{
    xine_panel::ptr xine_panel::create(container::ptr parent, const rectangle& rect)
    {
        xine_panel::ptr new_xine_panel(new xine_panel(parent, rect));
        return new_xine_panel;
    }
  
    xine_panel::xine_panel(container::ptr parent, const rectangle& rect) :
        widget(parent, rect)
    {
        xine_ = xine_new();
        // FIXME: make this configurable or set it from the configure prefix
        xine_config_load(xine_, "/usr/local/share/libstk/xine_config");
        xine_init(xine_);

        // get the video and audio drivers
        INFO("creating the xine video port");
        xine_vo_port_ = xine_open_video_driver(xine_, "stk", XINE_VISUAL_TYPE_FB, (void *)this);
        INFO("creating the xine audio port");
        xine_ao_port_ = xine_open_audio_driver(xine_, "auto", NULL);
        //xine_gui_send_vo_data(stream, XINE_GUI_SEND_DRAWABLE_CHANGED, (void *) window[fullscreen]);
        //xine_gui_send_vo_data(stream, XINE_GUI_SEND_VIDEOWIN_VISIBLE, (void *) 1);


        INFO("creating the xine stream");
        xine_stream_ = xine_stream_new(xine_, xine_ao_port_, xine_vo_port_);
        INFO("creating the xine event_queue");
        xine_event_queue_ = xine_event_new_queue(xine_stream_);
        INFO("creating the xine event_listener");
        xine_event_create_listener_thread(xine_event_queue_, xine_event_listener, NULL);
    }
      
    xine_panel::~xine_panel()
    {
        INFO("xine cleanup");
        xine_close(xine_stream_);
        xine_event_dispose_queue(xine_event_queue_);
        xine_dispose(xine_stream_);
        xine_close_audio_driver(xine_, xine_ao_port_);
        xine_close_video_driver(xine_, xine_vo_port_);
        xine_exit(xine_);
    }

    void xine_panel::open(const std::string& stream_mrl)
    {
        if (!xine_open(xine_stream_, stream_mrl.c_str()))
        {
            INFO("xine_open failed to open stream mrl: " << stream_mrl);
        }
    }

    bool xine_panel::play(int position, int millis)
    {
        if (!xine_play(xine_stream_, position, millis))
        {
            INFO("xine_play failed");
            // throw an exception
        }
        return true;
    }

    bool xine_panel::pause()
    {
        if (xine_get_param(xine_stream_, XINE_PARAM_SPEED) != XINE_SPEED_PAUSE)
            xine_set_param(xine_stream_, XINE_PARAM_SPEED, XINE_SPEED_PAUSE);

        return true;
    }

    bool xine_panel::playpause()
    {
        if (xine_get_param(xine_stream_, XINE_PARAM_SPEED) != XINE_SPEED_PAUSE)
            xine_set_param(xine_stream_, XINE_PARAM_SPEED, XINE_SPEED_PAUSE);
        else
            xine_set_param(xine_stream_, XINE_PARAM_SPEED, XINE_SPEED_NORMAL);

        return true;
    }

    bool xine_panel::speed(int val)
    {
        INFO(__FUNCTION__ << " not implemented");
        switch(val)
        {
            case  XINE_SPEED_PAUSE:
            case  XINE_SPEED_SLOW_4:
            case  XINE_SPEED_SLOW_2:
            case  XINE_SPEED_NORMAL:
            case  XINE_SPEED_FAST_2:
            case  XINE_SPEED_FAST_4:
                xine_set_param(xine_stream_, XINE_PARAM_SPEED, val);
                break;
            default:
                INFO("undefined speed parameter: " << val);
        }
        return true;
    }

/*
#define XINE_SPEED_PAUSE                   0
#define XINE_SPEED_SLOW_4                  1
#define XINE_SPEED_SLOW_2                  2
#define XINE_SPEED_NORMAL                  4
#define XINE_SPEED_FAST_2                  8
#define XINE_SPEED_FAST_4                  16
*/


    int xine_panel::speed()
    {
        return xine_get_param(xine_stream_, XINE_PARAM_SPEED);
    }

    bool xine_panel::faster()
    {
        int speed = xine_get_param(xine_stream_, XINE_PARAM_SPEED) << 1;
        if (speed > XINE_SPEED_FAST_4) 
            speed = XINE_SPEED_FAST_4;
        else if (speed == XINE_SPEED_PAUSE) 
            speed = XINE_SPEED_SLOW_4;
        xine_set_param(xine_stream_, XINE_PARAM_SPEED, speed);
        return true;
    }

    bool xine_panel::slower()
    {
        int speed = xine_get_param(xine_stream_, XINE_PARAM_SPEED) >> 1;
        xine_set_param(xine_stream_, XINE_PARAM_SPEED, speed);
        return true;
    }

} 


