/**************************************************************************************************
 *    FILENAME: xine_test.cpp
 * DESCRIPTION: An simple STK frontend to xine, demonstrating the overlay mechanism
 *     AUTHORS: Darren Hart
 *  START DATE: 20/Jul/2003  LAST UPDATE: 20/Jul/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include <iostream>
#include <SDL/SDL.h> // temporary: to get sdl overlay formats, remove once we define our own
#include <libstk/stk.h>

// backend specific headers
#include <libstk/event_producer_sdl.h>
#include <libstk/surface_sdl.h>

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
        // create the event producer
        // it is automatically registered with the primary event_system
        event_producer_sdl::ptr event_prod = event_producer_sdl::create();
        // create the primary surface
        surface::ptr screen = surface_sdl::create(rectangle(0, 0, 640, 480));
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
        cout << "creating the xine video port" << endl;
        vo_port = xine_open_video_driver(xine, "stk", XINE_VISUAL_TYPE_FB, (void *)xp.get());
        cout << "creating the xine audio port" << endl;
        ao_port = xine_open_audio_driver(xine , "auto", NULL);

        cout << "creating the xine stream" << endl;
        stream = xine_stream_new(xine, ao_port, vo_port);
        cout << "creating the xine event_queue" << endl;
        event_queue = xine_event_new_queue(stream);
        cout << "creating the xine event_listener" << endl;
        xine_event_create_listener_thread(event_queue, xine_event_listener, NULL);

        //xine_gui_send_vo_data(stream, XINE_GUI_SEND_DRAWABLE_CHANGED, (void *) window[fullscreen]);
        //xine_gui_send_vo_data(stream, XINE_GUI_SEND_VIDEOWIN_VISIBLE, (void *) 1);

        cout << "opening the stream" << endl;
        if((!xine_open(stream, "test_movie")) || (!xine_play(stream, 0, 0))) {
            cout << "unable to open test_movie" << endl;
            return 1;
        }

        cout << "running the libstk app" << endl;
        // run the program
        retval = app->run();

        // xine cleanup
        xine_close(stream);
        xine_event_dispose_queue(event_queue);
        xine_dispose(stream);
        xine_close_audio_driver(xine, ao_port);  
        xine_close_video_driver(xine, vo_port);  
        xine_exit(xine);
    }
    catch (const exception& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }

    return retval;
}
