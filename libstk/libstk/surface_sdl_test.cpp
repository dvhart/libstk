/*
 * A simple test of the surface_sdl backend surface class
 */

#include "surface_sdl.h"
#include "rectangle.h"
#include "exceptions.h"
#include <iostream>
#include <SDL/SDL.h>

using namespace stk;
using std::cout;
using std::endl;
using std::cerr;

int main()
{
	try
	{
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
			// we always pass SDL_INIT_TIMER since exec needs it
			std::cout << "app::init - Unable to init SDL: " << SDL_GetError() << std::endl;
			return 1;
		}
		// SDL cleanup on exit (ESC or window close)
		atexit(SDL_Quit);

		surface_sdl screen(*SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE | SDL_DOUBLEBUF));

		// prepare the screen fill color
		graphics_context gc;
		gc.fill_color(screen.gen_color("0xFF00FFFF")); 
		screen.gc(gc);

		// create a white surface with a blue circle and a red aa line
		rectangle my_rect(0, 0, 150, 150);
		surface_sdl my_surface(my_rect);
		gc.line_color(my_surface.gen_color("0x0000FFFF")); 
		gc.fill_color(my_surface.gen_color("0xFFFFFFFF")); 
		my_surface.gc(gc);
		my_surface.fill_rect(0, 0, 149, 149);
		my_surface.draw_circle(75, 75, 50);
		gc.line_color(my_surface.gen_color("0xFF0000FF")); 
		my_surface.gc(gc);
		my_surface.draw_line_aa(0, 0, 149, 149);
		// draw some arcs in a box
		my_surface.draw_arc(75, 75, 140, 140, ur_quadrant);
		my_surface.draw_arc(75, 75, 140, 140, lr_quadrant);
		my_surface.draw_arc(75, 75, 140, 140, ll_quadrant);
		my_surface.draw_arc(75, 75, 140, 140, ul_quadrant);
		// outline the box
		my_surface.draw_rect(0, 0, 149, 149); // fixme: should we draw INTERNAL to a passed rect ?

		bool done = false;
		SDL_Event event;
		SDL_Event new_event;
		int has_event;

		int xy = 0;
		
		// wait for close and move my_surface around the screen
		while (!done) {
			my_surface.x1(xy); my_surface.y1(xy);
			
			// handle any events
			if (has_event = SDL_PollEvent(&new_event)) {
				if (has_event) 
				{
					memcpy(&event, &new_event, sizeof(SDL_Event));
				} 

				if (event.type == SDL_QUIT) {
					done = 1;
				}
			}
			screen.fill_rect(0, 0, 639, 479);
			my_surface.blit(screen);
			screen.flip();
			// hop off the processor for a few ms
			//if (!SDL_PollEvent(NULL)) SDL_Delay(33);
			++xy %= 329;
		}
	}
	catch (error_message_exception e)
	{
		cout << "Exception caught: " << e.message() << endl;
	}
}

