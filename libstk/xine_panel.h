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
        xine_panel(const rectangle& rect = rectangle());
        /// static wrapper that receives a xine_panel as user_data and then calls its listener
        static void event_listener_wrapper(void *user_data, const xine_event_t* xine_event);
        /// the actual event listener, called by xine_event_listener_wrapper
        void event_listener(const xine_event_t* xine_event);
        /// tell xine the surface has changed, bound to the on_resize signal :: FIXME we shouldn't use a public signal ??
        void drawable_changed();
        /// the xine backend, one per xine_panel so they can play at different speeds (ie pause)
        xine_t*             xine_;
        /// each xine panel has one stream, and therefore one audio and video port, and event queue
        xine_stream_t*      xine_stream_;
        xine_video_port_t*  xine_vo_port_;
        xine_audio_port_t*  xine_ao_port_;
        xine_event_queue_t* xine_event_queue_;
        xine_post_t*        xine_post_;

        /// the rectangle we return to from fullscreen mode
        rectangle restore_rect_;
        bool fullscreen_;

    public:
        static xine_panel::ptr create(container::ptr parent, const rectangle& _rect, 
                const std::string& config = "/usr/local/share/libstk/xine_config", // fixme: use PACKAGE_PREFIX ??
                const std::string& audio_driver = "auto");
        ~xine_panel();

        /********** EVENT HANDLER INTERFACE **********/
        virtual void handle_event(event::ptr e);
        /********** END EVENT HANDLER INTERFACE **********/

        /********** DRAWABLE INTERFACE **********/
        virtual void draw(surface::ptr surface, const rectangle& clip_rect = rectangle());
        /********** END DRAWABLE INTERFACE **********/

        /********** WIDGET INTERFACE **********/
        /********** END WIDGET INTERFACE **********/

        /********** XINE PANEL INTERFACE **********/
        void open(const std::string& filename);
        /// play the current stream from position (0-65535) or time millis
        /// position is used if both != 0
        void play(int position, int millis);
        /// pause the playback of the current stream
        void pause();     
        /// toggle play/pause state
        void playpause(); 
        /// stop playback (doesn't destroy stream_)
        void stop(); 
        /// close the stream, open must be called prior to play
        void close(); 
        void speed(int val);
        int speed();
        void faster();
        void slower();
        // FIXME: clean this up; maybe delete the toggle_fullscreen method
        bool fullscreen() { return fullscreen_; }
        void fullscreen(bool val) { if (val != fullscreen_) toggle_fullscreen(); }
        void toggle_fullscreen();
        bool visualization(const std::string& name);
        /// Send a xine event to the current stream (for more customizable frontends)
        void send_event(xine_event_t* xe);
        /// Get xine engine status, see xine.h for a list of defined status values
        int status();
        /// Get and set xine engine parameters, see xine.h for a list of defined param's and ranges
        int parameter(int param);
        void parameter(int param, int value);
        /// Get xine stream info, see xine.h for a list of defined stream infos and ranges
        int stream_info(int info);
        /// Get xine stream meta info, see xine.h for a list of defined stream meta infos
        std::string meta_info(int info);
        /// Get stream position information.  (depends on stream support)
        ///\param stream_pos the positon in the stream out of 65535
        ///\param ms_pos the position in milliseconds
        ///\param ms_len the length of the stream in milliseconds
        void position(int* stream_pos, int* ms_pos, int* ms_len);
        // xine_panel signals
        boost::signal<bool (), combiner::logical_or<bool> > on_playback_finished;
        /********** END XINE PANEL INTERFACE **********/
    };

} // namespace stk

#endif

