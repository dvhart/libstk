/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: xine media panel implementation
 *     AUTHORS: Darren Hart
 *  START DATE: 2003/Jul/21
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Str�mke, Dirk H�rner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include <iostream>
#include "libstk/xine_panel.h"
#include "libstk/key_event.h"
#include "libstk/mouse_event.h"
#include "libstk/logging.h"
#include "libstk/color_manager.h" // delete if we move draw to tribal_theme

namespace stk
{
    xine_panel::ptr xine_panel::create(container::ptr parent, const rectangle& rect, 
            const std::string& config)
    {
        xine_panel::ptr new_xine_panel(new xine_panel(rect));
        new_xine_panel->parent(parent);
        new_xine_panel->init(config);
        return new_xine_panel;
    }
  
    xine_panel::xine_panel(const rectangle& rect) : widget(rect), fullscreen_(false)
    {
        INFO("constructor");
        focusable_ = true;        
    }
      
    xine_panel::~xine_panel()
    {
        INFO("destructor");
        xine_close(xine_stream_);
        xine_event_dispose_queue(xine_event_queue_);
        xine_dispose(xine_stream_);
        xine_close_audio_driver(xine_, xine_ao_port_);
        xine_close_video_driver(xine_, xine_vo_port_);
        xine_exit(xine_);
    }

    // FIXME: move this into create, no need for another routine
    void xine_panel::init(const std::string& config)
    {
        INFO("initializing the xine engine");
        xine_ = xine_new();
        xine_config_load(xine_, config.c_str());
        xine_init(xine_);

        INFO("creating the xine stream, event queue, event_listener, audio, and video ports");
        xine_ao_port_ = xine_open_audio_driver(xine_, "auto", NULL);
        xine_vo_port_ = xine_open_video_driver(xine_, "stk", XINE_VISUAL_TYPE_FB, (void*)this);
        xine_stream_ = xine_stream_new(xine_, xine_ao_port_, xine_vo_port_);

        xine_event_queue_ = xine_event_new_queue(xine_stream_);
        xine_event_create_listener_thread(xine_event_queue_, &event_listener_wrapper, (void*)this);
        xine_gui_send_vo_data(xine_stream_, XINE_GUI_SEND_DRAWABLE_CHANGED, (void*)this);
        xine_gui_send_vo_data(xine_stream_, XINE_GUI_SEND_VIDEOWIN_VISIBLE, (void*)1);
    }

    void xine_panel::handle_event(event::ptr e)
    {
        switch(e->type())
        {
        // don't let widget::handle_event call redraw 
        // (it causes our parent to draw over our video output)
        case event::focus:
            focused_ = true;
            return;
            break;
        case event::un_focus:
            focused_ = false;
            return;
            break;
        case event::mouse_enter:
            hover_ = true;
            return;
            break;
        case event::mouse_leave:
            hover_ = false;
            return;
            break;
        case event::key_up:
        {
            key_event::ptr ke = boost::shared_static_cast<key_event>(e);
            switch ( ke->fn_key() )
            {
            case key_enter:
                INFO("enter released");
                return;
                break;
            }
            break; // key_up
        }
        case event::key_down:
        {
            key_event::ptr ke = boost::shared_static_cast<key_event>(e);
            switch (ke->fn_key())
            {
            // FIXME: use a method to send these events to the xine engine
            case key_enter:
                xine_event_t xine_event;
                xine_event.type = XINE_EVENT_INPUT_SELECT;
                xine_event.data_length = 0;
                xine_event.data = NULL;
                xine_event.stream = xine_stream_;
                gettimeofday(&xine_event.tv, NULL);
                xine_event_send(xine_stream_, &xine_event);
                return;
                break;
            case key_uparrow:
            {
                xine_event_t xine_event;
                xine_event.type = XINE_EVENT_INPUT_UP;
                xine_event.data_length = 0;
                xine_event.data = NULL;
                xine_event.stream = xine_stream_;
                gettimeofday(&xine_event.tv, NULL);
                xine_event_send(xine_stream_, &xine_event);
                return;
                break;
            }
            case key_downarrow:
            {
                xine_event_t xine_event;
                xine_event.type = XINE_EVENT_INPUT_DOWN;
                xine_event.data_length = 0;
                xine_event.data = NULL;
                xine_event.stream = xine_stream_;
                gettimeofday(&xine_event.tv, NULL);
                xine_event_send(xine_stream_, &xine_event);
                return;
                break;
            }
            case key_rightarrow:
            {
                xine_event_t xine_event;
                xine_event.type = XINE_EVENT_INPUT_RIGHT;
                xine_event.data_length = 0;
                xine_event.data = NULL;
                xine_event.stream = xine_stream_;
                gettimeofday(&xine_event.tv, NULL);
                xine_event_send(xine_stream_, &xine_event);
                return;
                break;
            }
            case key_leftarrow:
            {
                xine_event_t xine_event;
                xine_event.type = XINE_EVENT_INPUT_LEFT;
                xine_event.data_length = 0;
                xine_event.data = NULL;
                xine_event.stream = xine_stream_;
                gettimeofday(&xine_event.tv, NULL);
                xine_event_send(xine_stream_, &xine_event);
                return;
                break;
            }
            case key_backspace: // FIXME: use F, or something else...
            {
                toggle_fullscreen();
                return;
                break;
            }
            } // end switch (ke->fn_key())
            break; // key_down
        }
        case event::mouse_down:
        {
            mouse_event::ptr me = boost::shared_static_cast<mouse_event>(e);
            INFO("mouse button " << me->button() << " pressed");
            return;
            break; // mouse_down
        }
        case event::mouse_up:
        {
            mouse_event::ptr me = boost::shared_static_cast<mouse_event>(e);
            INFO("mouse button " << me->button() << " released");
            return;
            break; // mouse_up
        }
        } // end switch
        // if we haven't handled it, try the defaults and possibly pass it up
        widget::handle_event(e);
    }

    // FIXME: consider moving this to tribal_theme?
    void xine_panel::draw(surface::ptr surface, const rectangle& clip_rect)
    {
        graphics_context::ptr gc = graphics_context::create();
        gc->fill_color(color_manager::get()->get_color(color_properties("0x00000000", surface)));
        surface->gc(gc);
        surface->fill_rect(clip_rect);

        // FIXME: send an expose event to the xine object
    }

    void xine_panel::event_listener_wrapper(void *user_data, const xine_event_t *xine_event) 
    {
        xine_panel* xp = reinterpret_cast<xine_panel*>(user_data);
        xp->event_listener(xine_event);
    }

    void xine_panel::event_listener(const xine_event_t* xine_event)
    {
        // FIXME: get a list of events and create corresponding signals...
        switch(xine_event->type) 
        { 
        case XINE_EVENT_UI_PLAYBACK_FINISHED:
            //running = 0;
            INFO("XINE_EVENT_UI_PLAYBACK_FINISHED");
            on_playback_finished();
            break;
        case XINE_EVENT_PROGRESS:
        {
            xine_progress_data_t* pevent = (xine_progress_data_t*)xine_event->data;
            INFO("XINE_EVENT_PROGRESS - " << pevent->description << "[" << pevent->percent << "%]");
            break;
        }
        } // end switch
    }

    void xine_panel::open(const std::string& stream_mrl)
    {
        if (!xine_open(xine_stream_, stream_mrl.c_str()))
        {
            INFO("xine_open failed to open stream mrl: " << stream_mrl);
        }
    }

    void xine_panel::play(int position, int millis)
    {
        if (!xine_play(xine_stream_, position, millis))
        {
            INFO("xine_play failed");
            // throw an exception
        }
    }

    void xine_panel::pause()
    {
        if (xine_get_param(xine_stream_, XINE_PARAM_SPEED) != XINE_SPEED_PAUSE)
            xine_set_param(xine_stream_, XINE_PARAM_SPEED, XINE_SPEED_PAUSE);
    }

    void xine_panel::playpause()
    {
        if (xine_get_param(xine_stream_, XINE_PARAM_SPEED) != XINE_SPEED_PAUSE)
            xine_set_param(xine_stream_, XINE_PARAM_SPEED, XINE_SPEED_PAUSE);
        else
            xine_set_param(xine_stream_, XINE_PARAM_SPEED, XINE_SPEED_NORMAL);
    }

    void xine_panel::speed(int val)
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

    void xine_panel::faster()
    {
        int speed = xine_get_param(xine_stream_, XINE_PARAM_SPEED) << 1;
        if (speed > XINE_SPEED_FAST_4) 
            speed = XINE_SPEED_FAST_4;
        else if (speed == XINE_SPEED_PAUSE) 
            speed = XINE_SPEED_SLOW_4;
        xine_set_param(xine_stream_, XINE_PARAM_SPEED, speed);
    }

    void xine_panel::slower()
    {
        int speed = xine_get_param(xine_stream_, XINE_PARAM_SPEED) >> 1;
        xine_set_param(xine_stream_, XINE_PARAM_SPEED, speed);
    }

    void xine_panel::toggle_fullscreen()
    {
        if (fullscreen_)
        {
            INFO("restore from fullscreen");
            rect(restore_rect_);
            redraw(surface()->rect()); 
        }
        else
        {
            INFO("going full screen");
            restore_rect_ = rect_;
            rect(surface()->rect());
        }
        fullscreen_ = !fullscreen_;
        INFO("sending gui drawable changed");
        // FIXME: these are deprecated, use: xine_port_send_gui_data(...)
        xine_gui_send_vo_data(xine_stream_, XINE_GUI_SEND_DRAWABLE_CHANGED, (void*)this);
        xine_gui_send_vo_data(xine_stream_, XINE_GUI_SEND_VIDEOWIN_VISIBLE, (void*)1);
    }

    bool xine_panel::visualization(const std::string& name)
    {
        // HACK: testing post plugins
        INFO("Available Visualizations: ");
        int i = 0;
        while (xine_list_post_plugins_typed(xine_, XINE_POST_TYPE_AUDIO_VISUALIZATION)[i])
        {
            INFO(" "<<xine_list_post_plugins_typed(xine_, XINE_POST_TYPE_AUDIO_VISUALIZATION)[i++]);
        }
        // END HACK
        if (post = xine_post_init(xine_, name.c_str(), 0, &xine_ao_port_, &xine_vo_port_))
        {
            xine_post_wire_audio_port(xine_get_audio_source(xine_stream_), post->audio_input[0]);
            INFO("Post plugin " << name << " loaded");
            return true;
        }
        ERROR("Failed to load post plugin " << name << " using none");
        // FIXME: unwire any existing connection ?
        return false;
    }

    void xine_panel::send_event(xine_event_t* xe)
    {
        xine_event_send(xine_stream_, xe);
    }
} 


