/******************************************************************************
 *    FILENAME: event_producer_elotouch.cpp 
 * DESCRIPTION: EloTouch event producer implementation.
 *     AUTHORS: Marc Straemke
 *  START DATE: 28/Jun/2003  LAST UPDATE: 28/Jun/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available
 *              with the source as license.txt or at 
 *              http://www.libstk.org/index.php?page=docs/license
 *****************************************************************************/

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

using std::cout;
using std::endl;
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
         
        /* 
           initialize all control characters 
           default values can be found in /usr/include/termios.h, and are given
           in the comments, but we don't need them here
        */
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
        
        /* 
           now clean the modem line and activate the settings for the port
        */
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
    {
        fd = open(devname.c_str(),O_RDWR | O_NOCTTY  | O_NDELAY);
        if(fd <0)
            ERROR("Couldnt open elotouch Device " << devname);
        init_serial_port(fd);
        init_elotouch(fd,false);
        
    }

    event_producer_elotouch::~event_producer_elotouch()
    {
        close(fd);
        cout << "event_producer_sdl::~event_producer_sdl()" << endl;
/*
        for(int i=0;i<10;i++)
            std::cout << std::hex << (unsigned int)(unsigned char)buf[i] << "  ";
        std::cout << "\nType=" << buf[1] << " Lead=" << buf[0] << std::endl;
        if(buf[1]=='T')
        {
            if(buf[2]==1) std::cout << "Touch";
            else if(buf[2]==2) std::cout << "Stream";
            else if(buf[2]==4) std::cout << "Untouch";
            int x=*(short*)&(buf[3]);
            int y=*(short*)&(buf[5]);
            std::cout << "\tX=" << std::dec << x << " \tY=" << y << std::endl;
            }*/
    
    }

    void elo_scale_xy(int &x,int &y)
    {
        int x_org=x;
        int y_org=y;
        int width=ELO_MAX_X - ELO_MIN_X;
        int height=ELO_MAX_Y - ELO_MIN_Y;
        
        x=800*(x_org-ELO_MIN_X) / width;
        y=600*(y_org-ELO_MIN_Y) / height;
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
                elo_scale_xy(x,y);
                WARN ("Touch message received type=" << (unsigned int)message[2]);
                
                if(message[2]==1) //TOUCH
                {
                    event_.reset(new mouse_event(x,y,1,event::mouse_down));
                    INFO("Touch at X=" << x << " Y=" << y);
                }
                if(message[2]==4) //UNTOUCH
                {
                    event_.reset(new mouse_event(x,y,1,event::mouse_up));
                    INFO("Untouch at X=" << x << " Y=" << y);
                    just_untouched=true;
                }
            } // message type == T
        } // bufsize > 10
        return event_;
    }

} // namespace stk
