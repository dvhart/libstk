/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: EloTouch event producer backend.
 *     AUTHORS: Marc Strämke, Darren Hart
 *  START DATE: 2003/Jun/28
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 **************************************************************************************************/

#ifndef STK_EVENT_PRODUCER_ELOTOUCH_H
#define STK_EVENT_PRODUCER_ELOTOUCH_H

#include <libstk/event_producer.h>
#include <libstk/event_system.h>
#include <string>
#include <queue>

namespace stk
{
    class event_producer_elotouch : public event_producer
    {
    public:
        typedef boost::shared_ptr<event_producer_elotouch> ptr;
    private:
        int fd;
        int touch_x,touch_y;    // Store last touchpos for untouch
        std::queue<unsigned char> buffer;
        bool just_untouched;
        bool raw_mode_;         // Send raw touchscreen coordinates?
        void elo_scale_xy(int &x,int &y) const;
    protected:
        event_producer_elotouch(std::string devicename);
        float calib_a,calib_b,calib_c,calib_d;            // Calibration parameters
    public:
        static event_producer_elotouch::ptr create(std::string devicename);
        virtual ~event_producer_elotouch();
        virtual event::ptr poll_event();
        void set_calib(const float a, const float b, const float c, const float d); // Set calibration values
        void get_calib(float &a, float &b, float &c, float &d) const; // Get calibration values
        void raw_mode(bool newval);
        bool raw_mode() const;
    };
}

#endif
