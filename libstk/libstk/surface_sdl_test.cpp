/*
 * A simple test of the surface_sdl backend surface class
 */

#include "surface_sdl.h"
#include <iostream>
#include <SDL/SDL.h>

using namespace stk;
using std::cout;
using std::endl;
using std::cerr;

int main()
{
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
    // we always pass SDL_INIT_TIMER since exec needs it
    std::cout << "app::init - Unable to init SDL: " << SDL_GetError() << std::endl;
    return 1;
  }
  // SDL cleanup on exit (ESC or window close)
  atexit(SDL_Quit);
	 
	surface_sdl screen(*SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE | SDL_DOUBLEBUF));

	// prepare the drawing colors, etc
	graphics_context gc;
	gc.line_color(screen.gen_color("0xFF0000FF")); // draw read lines
	// ...
	screen.gc(gc);

	screen.draw_line(10, 10, 100, 100);
	
	bool done = false;
	SDL_Event event;
	SDL_Event new_event;
	int has_event;

	while (!done) {
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
		SDL_Flip(screen.sdl_surface());
		// hop off the processor for a few ms
		if (!SDL_PollEvent(NULL)) SDL_Delay(33);

	}
}

