/***************************************************************************
	surface.cpp  -  surface class header
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

// ***********************************************************
// FIXME: we simply cast Uint32 sdl colors to color stk colors,
// we need to determine how exactly this mapping works
// ***********************************************************

#include "surface_sdl.h"
#include "surface.h"
#include <math.h>
#include <vector>
#include <list>
#include <iostream>
#include <SDL/SDL.h>

using std::cout;
using std::endl;
using std::cerr;

namespace stk
{
	surface_sdl::surface_sdl(SDL_Surface &new_surface)
	{
		sdl_surface_ = &new_surface;
		rect_.x1(0); rect_.y1(0);
		rect_.w(sdl_surface_->w);
		rect_.h(sdl_surface_->h);
	}

	surface_sdl::surface_sdl(rectangle &rect) : surface(rect)
	{
		SDL_Surface *temp_surface;
		temp_surface = SDL_CreateRGBSurface(SDL_SRCALPHA, rect.w(), rect.h(),
				32, RMASK, GMASK, BMASK, AMASK);
		sdl_surface_ = SDL_DisplayFormatAlpha(temp_surface);
		SDL_FreeSurface(temp_surface);
		SDL_SetAlpha(sdl_surface_, SDL_SRCALPHA | SDL_RLEACCEL, SDL_ALPHA_OPAQUE);
	}

	surface_sdl::~surface_sdl()
	{
		if (sdl_surface_)
			SDL_FreeSurface(sdl_surface_);
	}

	// methods which MUST be implemented in derived classes	
	void surface_sdl::draw_pixel(int x, int y, color clr) 
	{ 
		put_pixel(x, y, clr); 
	}

	void surface_sdl::draw_pixel_aa(int x, int y, double distance, color clr) 
	{ 
		put_pixel_aa(x, y, distance, clr); 
	}

	color surface_sdl::get_pixel(int x, int y)
	{
		return pixel_at(x, y);
	}
	
	color surface_sdl::gen_color(const std::string &str_color)
	{
		Uint8 r, g, b, a;
		Uint32 int_color = strtoll(str_color.c_str(), NULL, 16);
		Uint32 mapped_color;
		r = (int_color & RMASK) >> RSHIFT;
		g = (int_color & GMASK) >> GSHIFT;
		b = (int_color & BMASK) >> BSHIFT;
		a = (int_color & AMASK) >> ASHIFT;
		Uint32 sdl_color = gen_color(r, g, b, a);
		return (color)sdl_color;
	}

	color surface_sdl::gen_color(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
	{
		Uint32 sdl_color = SDL_MapRGBA(sdl_surface_->format, r, g, b, a);
		return (color)sdl_color;
	}

	void surface_sdl::lock(rectangle &rect, int flags, color** buf, int &stride)
	{
	}
	
	void surface_sdl::unlock()
	{
	}
	
	void surface_sdl::blit(surface &dst_surface, rectangle &src_rect, 
			rectangle &dst_rect)
	{
		// FIXME: how do we get at the sdl_surface of the target surface?
		// blit the local surface to the destination surface
		/*
		if (sdl_surface_ && SDL_BlitSurface(sdl_surface_, src_rect,
					dst_surface.sdl_surface(), dst_rect) < 0)
		{
			cerr << "widget: Failed to blit sdl_surface_ to screen" << endl;
		}
		*/
	}

	// optimized pixel routines
	void surface_sdl::put_pixel(int x, int y, color clr)
	{
		Uint32 sdl_color = (Uint32)clr;

		// DELETEME check for out of bounds
		if (x < 0 || y < 0 || x >= sdl_surface_->w || y >= sdl_surface_->h)
		{
			//throw std::string("surface::draw_pixel() - pixel coords out of bounds");
			cout << "surface::draw_pixel() - pixel coords out of bounds" << endl;
			return;
		}

		switch (sdl_surface_->format->BytesPerPixel)
		{
			case 1:                                       // Assuming 8-bpp
				{
					Uint8 *bufp;
					bufp = (Uint8 *) sdl_surface_->pixels + y * sdl_surface_->pitch + x;
					*bufp = sdl_color;
				}
				break;
			case 2:                                       // Probably 15-bpp or 16-bpp
				{
					Uint16 *bufp;
					bufp = (Uint16 *) sdl_surface_->pixels + y * sdl_surface_->pitch / 2 + x;
					*bufp = sdl_color;
				}
				break;
			case 3:                                       // Slow 24-bpp mode, usually not used
				{
					Uint8 *bufp;
					bufp = (Uint8 *) sdl_surface_->pixels + y * sdl_surface_->pitch + x * 3;
					if (SDL_BYTEORDER == SDL_LIL_ENDIAN)
					{
						bufp[0] = sdl_color;
						bufp[1] = sdl_color >> 8;
						bufp[2] = sdl_color >> 16;
					}
					else
					{
						bufp[2] = sdl_color;
						bufp[1] = sdl_color >> 8;
						bufp[0] = sdl_color >> 16;
					}
				}
				break;
			case 4:                                       // Probably 32-bpp
				{
					Uint32 *bufp;
					bufp = (Uint32 *) sdl_surface_->pixels + y * sdl_surface_->pitch / 4 + x;
					*bufp = sdl_color;
				}
				break;
		}
	}

	color surface_sdl::pixel_at(int x, int y)
	{
		// DELETEME check for out of bounds
		if (x < 0 || y < 0 || x >= sdl_surface_->w || y >= sdl_surface_->h)
		{
			//throw std::string("surface::draw_pixel() - pixel coords out of bounds");
			cout << "surface::get_pixel() - pixel coords out of bounds" << endl;
			return 0;
		}

		switch (sdl_surface_->format->BytesPerPixel)
		{
			case 1:                                       // Assuming 8-bpp
				{
					Uint8 *bufp;
					bufp = (Uint8 *) sdl_surface_->pixels + y * sdl_surface_->pitch + x;
					return (Uint32)(*bufp);
				}
				break;
			case 2:                                       // Probably 15-bpp or 16-bpp
				{
					Uint16 *bufp;
					bufp = (Uint16 *) sdl_surface_->pixels + y * sdl_surface_->pitch / 2 + x;
					return (Uint32)(*bufp);
				}
				break;
			case 3:                                       // Slow 24-bpp mode, usually not used
				{
					// FIXME
					Uint8 *bufp;
					bufp = (Uint8 *) sdl_surface_->pixels + y * sdl_surface_->pitch + x * 3;
					Uint32 sdl_color;
					if (SDL_BYTEORDER == SDL_LIL_ENDIAN)
					{
						sdl_color =  bufp[0];
						sdl_color += (bufp[1] >> 8);
						sdl_color += (bufp[2] >> 16);
					}
					else
					{
						sdl_color =  bufp[2];
						sdl_color += (bufp[1] >> 8);
						sdl_color += (bufp[0] >> 16);
					}
					return (color)sdl_color;
				}
				break;
			case 4:                                       // Probably 32-bpp
				{
					Uint32 *bufp;
					bufp = (Uint32 *) sdl_surface_->pixels + y * sdl_surface_->pitch / 4 + x;
					return (color)*bufp;
				}
				break;
		}
	}

	void surface_sdl::put_pixel_aa(int x, int y, double distance, color clr)
	{
		// FIXME: confirm the format of clr
		// will it always be in SDL color format?
		// or will it always be in some 32 bit stk RGBA format ??
		Uint32 color_a = (Uint32)clr;
		
		Uint8 red_a, green_a, blue_a, alpha_a;
		SDL_GetRGB(color_a, sdl_surface_->format, &red_a, &green_a, &blue_a);
		alpha_a = filter_aa((fabs(distance)));

		Uint8 red_b, green_b, blue_b, alpha_b;
		Uint32 color_b = get_pixel(x, y);
		SDL_GetRGBA(color_b, sdl_surface_->format, &red_b, &green_b, &blue_b, &alpha_b);

		Uint8 red_o, green_o, blue_o, alpha_o;
		alpha_o = composite_alpha(alpha_a, alpha_b);
		float alpha_a_f = alpha_a / 255.0;
		float alpha_b_f = alpha_b / 255.0;
		float alpha_o_f = alpha_o / 255.0;

		red_o   = composite_a_over_b(red_a, red_b, alpha_a_f, alpha_b_f, alpha_o_f);
		green_o = composite_a_over_b(green_a, green_b, alpha_a_f, alpha_b_f, alpha_o_f);
		blue_o  = composite_a_over_b(blue_a, blue_b, alpha_a_f, alpha_b_f, alpha_o_f);

		draw_pixel(x, y, gen_color(red_o, green_o, blue_o, alpha_o));
	}
	
	// optimized drawing routines
	// WRITEME...

	// optimied aa drawing routines
	// WRITEME...

	// optimized fill routines
	// WRITEME...

	// optimized aa fill routines
	// WRITEME...

} // end namespace stk
