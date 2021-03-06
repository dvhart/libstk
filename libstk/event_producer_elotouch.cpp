/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: EloTouch event producer implementation.
 *     AUTHORS: Marc Str�mke
 *  START DATE: 2003/Jun/28
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Str�mke, Dirk H�rner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include <iostream>
#include <fstream>
#include "libstk/exceptions.h"
#include "libstk/event_producer_elotouch.h"
#include "libstk/mouse_event.h"
#include "libstk/logging.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

// baudrate settings are defined in <asm/termbits.h>, which is
//   included by <termios.h> 
#define BAUDRATE B9600            
// change this definition for the correct port 
#define _POSIX_SOURCE 1 // POSIX compliant source 

namespace
{
    
    void init_serial_port(int fd)
    {
        struct termios oldtio,newtio;
    
        tcgetattr(fd,&oldtio);
        bzero(&newtio, sizeof(newtio)); 
        
        newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
        newtio.c_iflag = 0;
         
        newtio.c_oflag = 0;

        newtio.c_lflag = 0;//ICANON;
         
         
        // initialize all control characters 
        // default values can be found in /usr/include/termios.h, and are given
        // in the comments, but we don't need them here
        
        newtio.c_cc[VINTR]    = 0;     /* Ctrl-c */ 
        newtio.c_cc[VQUIT]    = 0;     /* Ctrl-\ */
        newtio.c_cc[VERASE]   = 0;     /* del */
        newtio.c_cc[VKILL]    = 0;     /* @ */
        newtio.c_cc[VEOF]     = 0;     /* Ctrl-d */
        newtio.c_cc[VTIME]    = 0;     /* inter-character timer unused */
        newtio.c_cc[VMIN]     = 1;     /* blocking read until 1 character arrives */
        newtio.c_cc[VSWTC]    = 0;     /* '\0' */
        newtio.c_cc[VSTART]   = 0;     /* Ctrl-q */ 
        newtio.c_cc[VSTOP]    = 0;     /* Ctrl-s */
        newtio.c_cc[VSUSP]    = 0;     /* Ctrl-z */
        newtio.c_cc[VEOL]     = 0;     /* '\0' */
        newtio.c_cc[VREPRINT] = 0;     /* Ctrl-r */
        newtio.c_cc[VDISCARD] = 0;     /* Ctrl-u */
        newtio.c_cc[VWERASE]  = 0;     /* Ctrl-w */
        newtio.c_cc[VLNEXT]   = 0;     /* Ctrl-v */
        newtio.c_cc[VEOL2]    = 0;     /* '\0' */
        
        
        // now clean the serial line and activate the settings for the port
        tcflush(fd, TCIFLUSH);
        tcsetattr(fd,TCSANOW,&newtio);
    }

    
    void init_elotouch(int fd,bool stream)
    {
        char buf[10];
        memset(buf,'\0',10);
    
        buf[0]='U';
        buf[1]='M';
        buf[2]=0;
        if(stream)
            buf[3]=7;
        else
            buf[3]=5;
        buf[4]=0;
        buf[5]=0;
        buf[6]=0;
        buf[7]=0;
        buf[8]=0;
        buf[9]=0;
        write(fd,buf,10);
    }


}


namespace stk
{
/*    const int ELO_MIN_X = 400;
      const int ELO_MAX_X = 3670; */
    const int ELO_MIN_X = 3670;
    const int ELO_MAX_X = 400; 
    const int ELO_MIN_Y = 500;
    const int ELO_MAX_Y = 3540;
    
    event_producer_elotouch::ptr event_producer_elotouch::create(std::string devicename)
    {
        event_producer_elotouch::ptr new_event_producer_elotouch(new event_producer_elotouch(devicename));
        event_system::get()->add_producer(new_event_producer_elotouch);
        return new_event_producer_elotouch;
    }

    event_producer_elotouch::event_producer_elotouch(std::string devname)
        : just_untouched(false), raw_mode_(false)
    {
        calib_b=-3670;
        calib_a=1.0/(400-3670);
        calib_d=-500;
        calib_c=1.0/(3540-500);
        fd = open(devname.c_str(),O_RDWR | O_NOCTTY  | O_NDELAY);
        if(fd <0)
            ERROR("Couldnt open elotouch Device " << devname);
        init_serial_port(fd);
        init_elotouch(fd,false);
        
    }

    event_producer_elotouch::~event_producer_elotouch()
    {
        close(fd);
        INFO("event_producer_sdl::~event_producer_sdl()");
/*
        for(int i=0;i<10;i++)
            std::INFO(std::hex << (unsigned int)(unsigned char)buf[i] << "  ";
        std::INFO("\nType=" << buf[1] << " Lead=" << buf[0] << std::endl;
        if(buf[1]=='T')
        {
            if(buf[2]==1) std::INFO("Touch";
            else if(buf[2]==2) std::INFO("Stream";
            else if(buf[2]==4) std::INFO("Untouch";
            int x=*(short*)&(buf[3]);
            int y=*(short*)&(buf[5]);
            std::INFO("\tX=" << std::dec << x << " \tY=" << y << std::endl;
            }*/
    
    }

    void event_producer_elotouch::elo_scale_xy(int &x,int &y) const
    {
        INFO("Scaling (" << x << ";" << y << ") to");
        x=(calib_a*(x+calib_b))*800;
        y=(calib_c*(y+calib_d))*600;
        INFO("(" << x << ";" << y << ")");
/*        

        b=-3670;
        a=1/(400-3670);
        d=-3540;
        c=1/(500-3540);

        int x_org=x;
        int y_org=y;
        int width=ELO_MAX_X - ELO_MIN_X;
        int height=ELO_MAX_Y - ELO_MIN_Y;

        x=-0.236809f*x +884;    // Calibrated with ADS-TEC display
        y=0.190707f*y -91.4806f;// Calibrated
*/
        /*
        x=800*(x_org-ELO_MIN_X) / width;
        y=600*(y_org-ELO_MIN_Y) / height;*/
    }
    
    boost::shared_ptr<stk::event> event_producer_elotouch::poll_event()
    {
        event::ptr event_ = event::create(event::none);
        int res=1;
        unsigned char buf2;
        unsigned char message[10];
        while(res==1)
        {
            
            res=read(fd,&buf2,1);
            if(res==1)
                buffer.push(buf2);
        }
        while(!buffer.empty() &&buffer.front()!='U')
            buffer.pop();
        if(just_untouched)
        {
            just_untouched=false;
            event_.reset(new mouse_event(4000,4000,0,event::mouse_motion));
            return event_;
        }
        
        if(buffer.size()>=10)
        {
            for(int i=0;i<10;i++)
            {
                message[i]=buffer.front();
                buffer.pop();
            }
            if(message[1]=='T')
            {
                int x=*(short*)&(message[3]);
                int y=*(short*)&(message[5]);
                int prescalex=x;
                int prescaley=y;
                if(raw_mode_==false)
                    elo_scale_xy(x,y);
                //WARN ("Touch message received type=" << (unsigned int)message[2]);
                
                if(message[2]==1) //TOUCH
                {
                    event_.reset(new mouse_event(x,y,1,event::mouse_down));
                    touch_x=x;
                    touch_y=y;
                }
                if(message[2]==4) //UNTOUCH
                {
                    if(abs(x-touch_x)<30) // Snap to Touchpos if the deviation is small
                        x=touch_x;
                    if(abs(y-touch_y)<30)
                        y=touch_y;
                    event_.reset(new mouse_event(x,y,1,event::mouse_up));
                    just_untouched=true;
                }
            } // message type == T
        } // bufsize > 10
        return event_;
    }

    void event_producer_elotouch::set_calib(const float a, const float b, const float c, const float d)
    {
        calib_a=a;
        calib_b=b;
        calib_c=c;
        calib_d=d;
    }
    
    void event_producer_elotouch::get_calib(float &a,float &b, float &c, float &d) const 
    {
        a=calib_a;
        b=calib_b;
        c=calib_c;
        d=calib_d;
    }
    
    void event_producer_elotouch::raw_mode(bool newval)
    {
        raw_mode_=newval;
    }
    
    bool event_producer_elotouch::raw_mode() const
    {
        return raw_mode_;
    }

    
} // namespace stk
