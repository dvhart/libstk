/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: xine media panel
 *     AUTHORS: Darren Hart
 *  START DATE: 2003/Jul/21
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_XINE_PANEL_H
#define STK_XINE_PANEL_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/signal.hpp>

#include <libstk/widget.h>
#include <libstk/container.h>

// xine headers
#include <xine.h>
#include <xine/xineutils.h>

namespace stk
{
    class xine_panel : public widget
    {
    public:
        typedef boost::shared_ptr<xine_panel> ptr;
        typedef boost::weak_ptr<xine_panel> weak_ptr;

    private:
        xine_panel(const rectangle& rect);
        /// get the audio/video ports and set up the event handler
        /// must be called after the parent has been set
        void init(const std::string& config);
        /// static wrapper that receives a xine_panel as user_data and then calls its listener
        static void event_listener_wrapper(void *user_data, const xine_event_t* xine_event);
        /// the actual event listener, called by xine_event_listener_wrapper
        void event_listener(const xine_event_t* xine_event);
        /// the xine backend, one per xine_panel so they can play at different speeds (ie pause)
        xine_t*             xine_;
        /// each xine panel has one stream, and therefore one audio and video port, and event queue
        xine_stream_t*      xine_stream_;
        xine_video_port_t*  xine_vo_port_;
        xine_audio_port_t*  xine_ao_port_;
        xine_event_queue_t* xine_event_queue_;
        /// the rectangle we return to from fullscreen mode
        rectangle restore_rect_;
        bool fullscreen_;

    public:
        static xine_panel::ptr create(container::ptr parent, const rectangle& _rect, 
                const std::string& config="/usr/local/share/libstk/xine_config"); // fixme: use PACKAGE_PREFIX ??
        ~xine_panel();

        /********** EVENT HANDLER INTERFACE **********/
        virtual void handle_event(event::ptr e);
        /********** END EVENT HANDLER INTERFACE **********/

        /********** XINE PANEL INTERFACE **********/
        void open(const std::string& filename);
        void play(int position, int millis);
        void pause();     
        void playpause(); 
        void speed(int val);
        int speed();
        void faster();
        void slower();
        void toggle_fullscreen();
        /********** END XINE PANEL INTERFACE **********/
    };

} // namespace stk

#endif

