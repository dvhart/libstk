/***************************************************************************
                          stk_widget.h  -  description
                             -------------------
    begin                : Sat Apr 27 2002
    copyright            : (C) 2002 by Darren Hart
    email                : dvhart@byu.edu
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include <SDL/SDL.h>
#include  "surface.h"

using namespace stk;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
#define RMASK  0xff000000
#define GMASK  0x00ff0000
#define BMASK  0x0000ff00
#define AMASK  0x000000ff
#define RSHIFT 24
#define GSHIFT 16
#define BSHIFT 8
#define ASHIFT 0
#else
#define RMASK 0x000000ff
#define GMASK 0x0000ff00
#define BMASK 0x00ff0000
#define AMASK 0xff000000
#define RSHIFT 0
#define GSHIFT 8
#define BSHIFT 16
#define ASHIFT 24
#endif
	
surface::surface(SDL_Rect &rect) { 
  SDL_Surface *temp_surface;
  temp_surface = SDL_CreateRGBSurface(SDL_SRCALPHA, rect->w, rect->h,
				      32, RMASK, GMASK, BMASK, AMASK);
  _surface = SDL_DisplayFormatAlpha(temp_surface);
  SDL_FreeSurface(temp_surface);
  SDL_SetAlpha(bg_surface, SDL_SRCALPHA | SDL_RLEACCEL, SDL_ALPHA_OPAQUE);
}

surface::~surface() { 
  if (_surface) SDL_FreeSurface(_surface);
}
		
// non antialiased draw routines
void surface::draw_pixel(int x, int y, Uint32 color) {
  // DELETEME check for out of bounds
  if (x < 0 || y < 0 || x >= _surface->w || y >= _surface->h)
    return;
  
  switch (_surface->format->BytesPerPixel) {
  case 1:                                       // Assuming 8-bpp
    {
      Uint8 *bufp;
      bufp = (Uint8 *) _surface->pixels + y * _surface->pitch + x;
      *bufp = color;
    }
    break;
  case 2:                                       // Probably 15-bpp or 16-bpp
    {
      Uint16 *bufp;
      bufp = (Uint16 *) _surface->pixels + y * _surface->pitch / 2 + x;
      *bufp = color;
    }
    break;
  case 3:                                       // Slow 24-bpp mode, usually not used
    {
      Uint8 *bufp;
      bufp = (Uint8 *) _surface->pixels + y * _surface->pitch + x * 3;
      if (SDL_BYTEORDER == SDL_LIL_ENDIAN) {
        bufp[0] = color;
        bufp[1] = color >> 8;
        bufp[2] = color >> 16;
      } else {
        bufp[2] = color;
        bufp[1] = color >> 8;
        bufp[0] = color >> 16;
      }
    }
    break;
  case 4:                                       // Probably 32-bpp
    {
      Uint32 *bufp;
      bufp = (Uint32 *) _surface->pixels + y * _surface->pitch / 4 + x;
      *bufp = color;
    }
    break;
  }
}

int draw_line() {
}

int draw_rect() {
}

int draw_arc() {
}

int draw_ellipse() {
}

int draw_poly() {
}

// antialiased draw routines
int draw_line_aa() {
}

int draw_rect_aa() {
}

int draw_arc_aa() {
}

int draw_ellipse_aa() {
}

int draw_poly_aa() {
}

// non antialiased fill routines
int fill_rect() {
}

int fill_ellipse() {
}

// antialiased fill routines
int fill_rect_aa() {
}

int fill_ellipse_aa() {
}

void blit(surface &screen, SDL_Rect *rect = NULL) {
  // blit the local surface to the parent surface (screen)
  if (_surface && 
      SDL_BlitSurface(_surface, NULL, screen.get_surface(), rect) < 0) {
    cerr << "widget: Failed to blit bg_surface to screen" << endl;
  }
}

void surface::lock() {
  if (SDL_MUSTLOCK(_surface)) {
    if (SDL_LockSurface(_surface) < 0) {
      return;     // wtf ?
    }
  }
}

void surface::unlock() {
  if (SDL_MUSTLOCK(_surface)) {
    SDL_UnlockSurface(_surface);
  }
}
