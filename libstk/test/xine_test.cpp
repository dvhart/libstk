/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: An simple STK frontend to xine, demonstrating the overlay mechanism
 *     AUTHORS: Darren Hart
 *  START DATE: 2003/Jul/20
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include <iostream>
#include <SDL/SDL.h> // temporary: to get sdl overlay formats, remove once we define our own
#include <libstk/stk.h>

// backend specific includes (we select it dynamically, so include them all if built)
#ifdef HAVE_SDL
#include <libstk/event_producer_sdl.h>
#include <libstk/surface_sdl.h>
#endif
#ifdef HAVE_DFB
#include <libstk/surface_dfb.h>
// what would be a good default backend ?
#endif
#ifdef HAVE_FBDEV
#include <libstk/surface_fbdev.h>
// what would be a good default backend ?
#endif
// FIXME: should this be configurable also ?  --enable-elotouch ?
#include <libstk/event_producer_elotouch.h>

// xine headers
#include <xine.h>
#include <xine/xineutils.h>

using namespace stk;

void xine_event_listener(void *user_data, const xine_event_t *event) {
}

int main(int argc, char* argv[])
{
    int retval = 0;

    try
    {
#ifdef HAVE_LOGGING
        logger::get()->add_target(&cout, LL_Info);
#endif
        std::string surface_type, filename;
        if (argc < 3)
        {
            throw error_message_exception("Usage: xine_test sdl|dfb|fbdev filename");
        }
        else
        {
            surface_type = std::string(argv[1]);
            filename = std::string(argv[2]);
        }
        
        // select the surface and event system
        INFO("selecting surface and event system");
        surface::ptr screen;
        event_producer::ptr ep;
#ifdef HAVE_SDL
        if (surface_type == "sdl")
        {
            screen = surface_sdl::create(rectangle(0, 0, 640, 480));
            ep = event_producer_sdl::create();
        }
#endif
#ifdef HAVE_DFB
        else if (surface_type == "dfb")
        {
            screen = surface_dfb::create(rectangle(0, 0, 640, 480));
            // FIXME: use something a bit more generic for the test app
            ep = event_producer_elotouch::create("/dev/tts/0");
        }
#endif
#ifdef HAVE_FBDEV
#ifdef HAVE_SDL // needed for the event_system
        else if (surface_type == "fbdev")
        {
            screen = surface_fbdev::create(rectangle(0, 0, 1024, 768));
            ep = event_producer_sdl::create();
        }
#endif
#endif
        else
            throw error_message_exception("Unknown surface type");

        
        // create the application
        application::ptr app = application::create(screen);

        // create the main state and a button
        state::ptr main_state = state::create(app);
        button::ptr play_button = button::create(main_state, std::wstring(L"Play MRL"), 
                rectangle(10, 10, 100, 30));

        // create the xine panel
        //xine_panel::ptr xp = xine_panel::create(main_state, rectangle(310, 120, 320, 240));
        xine_panel::ptr xp = xine_panel::create(main_state, rectangle(50, 50, 540, 380));

        
        // start up xine
        static xine_t*             xine;
        static xine_stream_t*      stream;
        static xine_video_port_t*  vo_port;
        static xine_audio_port_t*  ao_port;
        static xine_event_queue_t* event_queue;

        xine = xine_new();
        xine_config_load(xine, "/home/dvhart/.xine/config"/*configfile path*/);
        xine_init(xine);

        // get the video and audio drivers
        INFO("creating the xine video port");
        vo_port = xine_open_video_driver(xine, "stk", XINE_VISUAL_TYPE_FB, (void *)xp.get());
        INFO("creating the xine audio port");
        ao_port = xine_open_audio_driver(xine , "auto", NULL);

        INFO("creating the xine stream");
        stream = xine_stream_new(xine, ao_port, vo_port);
        INFO("creating the xine event_queue");
        event_queue = xine_event_new_queue(stream);
        INFO("creating the xine event_listener");
        xine_event_create_listener_thread(event_queue, xine_event_listener, NULL);

        //xine_gui_send_vo_data(stream, XINE_GUI_SEND_DRAWABLE_CHANGED, (void *) window[fullscreen]);
        //xine_gui_send_vo_data(stream, XINE_GUI_SEND_VIDEOWIN_VISIBLE, (void *) 1);

        INFO("opening the stream");
        if((!xine_open(stream, filename.c_str())) || (!xine_play(stream, 0, 0))) {
            INFO("unable to open " << filename);
            return 1;
        }

        // add a timer (quit after 30 seconds)
        INFO("xine_test - creating timer to quit after 30 seconds");
        timer::ptr quit_timer = timer::create(30000, true); // every 20 seconds
        quit_timer->on_timer.connect( boost::bind(&stk::application::quit, app.get()));
        app->add_timer(quit_timer);

        INFO("running the libstk app");
        // run the program
        retval = app->run();

        // xine cleanup
        INFO("xine cleanup");
        xine_close(stream);
        xine_event_dispose_queue(event_queue);
        xine_dispose(stream);
        xine_close_audio_driver(xine, ao_port);  
        xine_close_video_driver(xine, vo_port);  
        xine_exit(xine);

        INFO("shared pointer automatic destuction follows:");
    }
    catch (const exception& e)
    {
        ERROR("Exception: " << e.what());
    }

    return retval;
}
