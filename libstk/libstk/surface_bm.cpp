/******************************************************************************
 *    FILENAME: surface_bm.cpp
 * DESCRIPTION: A surface benchmark, evaluating the cost of virtual calls. 
 *     AUTHORS: Darren Hart, Vernon Mauery, Marc Straemke
 *  START DATE: 17/May/2003  LAST UPDATE: 17/May/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available
 *              with the source as license.txt or at 
 *              http://www.libstk.org/index.php?page=docs/license
 *****************************************************************************/

#include <ctime>
#include <sys/time.h>

#include <iostream>
#include <SDL/SDL.h>
#include "libstk/exceptions.h"
#include "libstk/surface.h"
#include "libstk/surface_sdl.h"
#include "libstk/color_manager.h"

using namespace stk;
using std::cout;
using std::endl;

#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240
#define FRAMES        240.0

int main(int argc, char* argv[])
{
	int retval = 0;

	try
	{
		// initialize sdl
		// FIXME: where should this be done ?, perhaps in an application factory?
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
			throw error_message_exception(std::string("Unable to init SDL: ") + std::string(SDL_GetError()));
		atexit(SDL_Quit);
			
		// create the surface
		cout << "surface_bm - creating surface" << endl;
		surface_sdl::ptr surface_ = surface_sdl::create(rectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
		surface_->clip_rect(rectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));

		color color_;
		struct timeval tv_start;
		struct timeval tv_end;
		
		// see how long it takes to draw 30 screens with draw_pixel
		double virt_dur = 0.0;
		for (int i = 0; i < FRAMES; i++)
		{
			color_ = surface_->gen_color((unsigned char)(i*(255.0/FRAMES)), 0, 0, 0xFF);
			//cout << "Drawing frame " << std::dec << i << " with " << std::hex << color_ << endl;
			gettimeofday(&tv_start, NULL);
			for (int x = 0; x < SCREEN_WIDTH; x++)
			{
				for (int y = 0; y < SCREEN_HEIGHT; y++)
				{
					surface_->draw_pixel(x, y, color_);
				}
			}
			gettimeofday(&tv_end, NULL);
			virt_dur += ((tv_end.tv_sec - tv_start.tv_sec) + 
				(double)((tv_end.tv_usec > tv_start.tv_usec) ? 
					(tv_end.tv_usec - tv_start.tv_usec) : 
					(1000000 - (tv_start.tv_usec - tv_end.tv_usec)))/1000000.0);
			surface_->update();
		}

		// see how long it takes to draw 30 screens with put_pixel
		double priv_dur = 0.0;
		for (int i = 0; i < FRAMES; i++)
		{
			color_ = surface_->gen_color(0, (unsigned char)(i*(255.0/FRAMES)), 0, 0xFF);
			//cout << "Drawing frame " << std::dec << i << " with " << std::hex << color_ << endl;
			gettimeofday(&tv_start, NULL);
			for (int x = 0; x < SCREEN_WIDTH; x++)
			{
				for (int y = 0; y < SCREEN_HEIGHT; y++)
				{
					surface_->put_pixel(x, y, color_);
				}
			}
			gettimeofday(&tv_end, NULL);
			priv_dur += ((tv_end.tv_sec - tv_start.tv_sec) + 
				(double)((tv_end.tv_usec > tv_start.tv_usec) ? 
					(tv_end.tv_usec - tv_start.tv_usec) : 
					(1000000 - (tv_start.tv_usec - tv_end.tv_usec)))/1000000.0);
			surface_->update();
		}
	
		
		cout << "Virtual Call Duration: " << std::dec << virt_dur << " Avg: " << virt_dur/FRAMES << endl;
		cout << "Private Call Duration: " << std::dec << priv_dur << " Avg: " << priv_dur/FRAMES << endl;
		cout << "Private Calls are " << std::dec << 100.0 * (1 - priv_dur / virt_dur) << "% faster" << endl;
		
	}
	catch (const exception& e)
	{
		cout << "Exception: " << e.what() << endl;
	}

	cout << "Quitting" << endl;
	SDL_Quit();
	return retval;
}
