/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Xine media panel
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
        xine_panel(container::ptr parent, const rectangle& rect);
        xine_t*             xine_;
        xine_stream_t*      xine_stream_;
        xine_video_port_t*  xine_vo_port_;
        xine_audio_port_t*  xine_ao_port_;
        xine_event_queue_t* xine_event_queue_;

    public:
        static xine_panel::ptr create(container::ptr parent, const rectangle& _rect);
        ~xine_panel();
        void open(const std::string& filename);
        void play(int position, int millis);
        void pause();     
        void playpause(); 
        void speed(int val);
        int speed();
        void faster();
        void slower();
    };

} // namespace stk

#endif

