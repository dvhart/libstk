/***************************************************************************
                         surface.h  -  surface class header
                             -------------------
    begin                : Tue September 10 2002
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
	
surface::surface(SDL_Surface &new_surface) {
  _surface = &new_surface;
}

surface::surface(SDL_Rect &rect) {
  SDL_Surface *temp_surface;
  temp_surface = SDL_CreateRGBSurface(SDL_SRCALPHA, rect.w, rect.h,
				      32, RMASK, GMASK, BMASK, AMASK);
  _surface = SDL_DisplayFormatAlpha(temp_surface);
  SDL_FreeSurface(temp_surface);
  //SDL_SetAlpha(_surface, SDL_SRCALPHA | SDL_RLEACCEL, SDL_ALPHA_OPAQUE);
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

// FIXME: merge this, make it static, make the defines consts?
// I dunno, it's late and it works... but its ugly right here
// --dvhart
int direction(int x1, int y1, int x2, int y2)
{
#define LR    0x01       // left to right
#define RL    0x02       // right to left
#define UP    0x04       // up
#define DN    0x08       // down
#define S0    0x10       // gentle slope
#define S1    0x20       // steep slope
  
#define LRU_0 LR+UP+S0   // left to right, up, gentle slope
#define LRU_1 LR+UP+S1   // etc...
#define LRD_0 LR+DN+S0
#define LRD_1 LR+DN+S1
#define RLU_0 RL+UP+S0
#define RLU_1 RL+UP+S1
#define RLD_0 RL+DN+S0
#define RLD_1 RL+DN+S1
  
  int dir = 0;
  
  // determine slope (steep S1, gentle S0)
  int dy = ((y2 > y1) ? (y2 - y1) : (y1 - y2));
  int dx = ((x2 > x1) ? (x2 - x1) : (x1 - x2));
  if (dy != 0 && dx != 0)
    if (dy > dx) dir += S1;
    else if (dy <= dx) dir += S0;
  
  // determine verticle direction
  if (y1 < y2) dir += UP;
  else if (y1 > y2) dir += DN;
  
  // determine horizontal direction
  if (x1 < x2) dir += LR;
  else if(x1 > x2) dir += RL;
  
  return dir;
}

void surface ::draw_line(int x1, int y1, int x2, int y2, Uint32 color, /*style,width*/) 
{
  // determine the line direction	
  int dir = direction(x1, y1, x2, y2);
  int x = x1;
  int y = y1;

  switch(dir) {
  case LR:
    {
      for (x; x <= x2; x++) draw_pixel(x, y1, color);
      break;
    }
  case RL:
    {
      for (x; x >= x2; x--) draw_pixel(x, y1, color);
      break;
    }
  case UP:
    {
      for (y; y <= y2; y++) draw_pixel(x1, y, color);
      break;
    }
  case DN:
    {
      for (y; y >= y2; y--) draw_pixel(x1, y, color);
      break;
    }
  case LRU_0:
    {
      int dx       = x2-x1;
      int dy       = y2-y1;
      int d        = 2*dy-dx;
      int delta_e  = 2*dy;
      int delta_ne = 2*(dy-dx);

      draw_pixel(x, y, color);

      while (x < x2)
	{
	  if (d <= 0)
	    {
	      d += delta_e;
	      x++;
	    } 
	  else 
	    {
	      d += delta_ne;
	      x++;
	      y++;
	    }
	  draw_pixel(x, y, color);
	}
      break;
    }
  case LRU_1:
    {
      int dx       = x2-x1;
      int dy       = y2-y1;
      int d        = 2*dx-dy;
      int delta_e  = 2*dx;
      int delta_ne = 2*(dx-dy);

      draw_pixel(x, y, color);

      while (y < y2)
	{
	  if (d <= 0)
	    {
	      d += delta_e;
	      y++;
	    } 
	  else 
	    {
	      d += delta_ne;
	      y++;
	      x++;
	    }
	  draw_pixel(x, y, color);
	}
      break;
    }
  case LRD_0:
    {
      int dx       = x2-x1;
      int dy       = y1-y2;
      int d        = 2*dy-dx;
      int delta_e  = 2*dy;
      int delta_ne = 2*(dy-dx);

      draw_pixel(x, y, color);

      while (x < x2)
	{
	  if (d <= 0)
	    {
	      d += delta_e;
	      x++;
	    } 
	  else 
	    {
	      d += delta_ne;
	      x++;
	      y--;
	    }
	  draw_pixel(x, y, color);
	}
      break;
    }
  case LRD_1:
    {
      int dx       = x2-x1;
      int dy       = y1-y2;
      int d        = 2*dx-dy;
      int delta_e  = 2*dx;
      int delta_ne = 2*(dx-dy);

      draw_pixel(x, y, color);

      while (y > y2)
	{
	  if (d <= 0)
	    {
	      d += delta_e;
	      y--;
	    } 
	  else 
	    {
	      d += delta_ne;
	      x++;
	      y--;
	    }
	  draw_pixel(x, y, color);
	}
      break;

    }
  case RLU_0:
    {
      int dx       = x1-x2;
      int dy       = y2-y1;
      int d        = 2*dy-dx;
      int delta_e  = 2*dy;
      int delta_ne = 2*(dy-dx);

      draw_pixel(x, y, color);

      while (x > x2)
	{
	  if (d <= 0)
	    {
	      d += delta_e;
	      x--;
	    } 
	  else 
	    {
	      d += delta_ne;
	      x--;
	      y++;
	    }
	  draw_pixel(x, y, color);
	}
      break;
    }
  case RLU_1:
    {
      int dx       = x1-x2;
      int dy       = y2-y1;
      int d        = 2*dx-dy;
      int delta_e  = 2*dx;
      int delta_ne = 2*(dx-dy);

      draw_pixel(x, y, color);

      while (x > x2)
	{
	  if (d <= 0)
	    {
	      d += delta_e;
	      y++;
	    } 
	  else 
	    {
	      d += delta_ne;
	      x--;
	      y++;
	    }
	  draw_pixel(x, y, color);
	}
      break;
    }
  case RLD_0:
    {
      int dx       = x1-x2;
      int dy       = y1-y2;
      int d        = 2*dy-dx;
      int delta_e  = 2*dy;
      int delta_ne = 2*(dy-dx);

      draw_pixel(x, y, color);

      while (x > x2)
	{
	  if (d <= 0)
	    {
	      d += delta_e;
	      x--;
	    } 
	  else 
	    {
	      d += delta_ne;
	      x--;
	      y--;
	    }
	  draw_pixel(x, y, color);
	}
      break;

      break;
    }
  case RLD_1:
    {
      int dx       = x1-x2;
      int dy       = y1-y2;
      int d        = 2*dx-dy;
      int delta_e  = 2*dx;
      int delta_ne = 2*(dx-dy);

      draw_pixel(x, y, color);

      while (x > x2)
	{
	  if (d <= 0)
	    {
	      d += delta_e;
	      y--;
	    } 
	  else 
	    {
	      d += delta_ne;
	      x--;
	      y--;
	    }
	  draw_pixel(x, y, color);
	}

      break;
    }
  }

}

void surface ::draw_rect() {
}

void surface ::draw_arc() {
}

void surface ::draw_ellipse() {
}

void surface ::draw_poly() {
}

// antialiased draw routines
void surface ::draw_line_aa() {
}

void surface ::draw_rect_aa() {
}

void surface ::draw_arc_aa() {
}

void surface ::draw_ellipse_aa() {
}

void surface ::draw_poly_aa() {
}

// non antialiased fill routines
void surface ::fill_rect() {
}

void surface ::fill_ellipse() {
}

// antialiased fill routines
void surface ::fill_rect_aa() {
}

void surface ::fill_ellipse_aa() {
}


// utility methods
Uint32 surface::gen_color(const std::string &str_color) {
  Uint8 r, g, b, a;
  Uint32 int_color = strtoll(str_color.c_str(), NULL, 16);
  Uint32 mapped_color;
  r = (int_color & RMASK) >> RSHIFT;
  g = (int_color & GMASK) >> GSHIFT;
  b = (int_color & BMASK) >> BSHIFT;
  a = (int_color & AMASK) >> ASHIFT;
  return gen_color(r, g, b, a);
}

Uint32 surface::gen_color(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
  return SDL_MapRGBA(_surface->format, r, g, b, a);
}

void surface::blit(surface &screen, SDL_Rect *rect = NULL) {
  // blit the local surface to the parent surface (screen)
  if (_surface && 
      SDL_BlitSurface(_surface, NULL, screen.get_surface(), rect) < 0) {
    cerr << "widget: Failed to blit _surface to screen" << endl;
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
