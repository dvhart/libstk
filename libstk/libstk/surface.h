/***************************************************************************
	surface.cpp  -  surface class implementation
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


#ifndef STK_SURFACE_H
#define STK_SURFACE_H

#include "point.h"
#include "rectangle.h"
#include "graphics_context.h"
#include "stk_types.h"
#include <string>
#include <vector>

#define MIN(A,B) (((A)<(B))?(A):(B))

namespace stk
{
	// direction constants
	const int DOT   = 0x00;
	const int LR    = 0x01;
	const int RL    = 0x02;
	const int UP    = 0x04;
	const int DN    = 0x08;
	const int S0    = 0x10;
	const int S1    = 0x20;
	const int LRU_0 = LR+UP+S0;
	const int LRU_1 = LR+UP+S1;
	const int LRD_0 = LR+DN+S0;
	const int LRD_1 = LR+DN+S1;
	const int RLU_0 = RL+UP+S0;
	const int RLU_1 = RL+UP+S1;
	const int RLD_0 = RL+DN+S0;
	const int RLD_1 = RL+DN+S1;
	
	// opacity constants
	const int opaque      = 255;
	const int transparent = 0;
	
	// quadrant constants
	const int ur_quadrant = 1; // may not need these... maybe an enum...
	const int lr_quadrant = 2;
	const int ll_quadrant = 3;
	const int ul_quadrant = 4;

	static byte alpha_filter[256] =
	{
		#include "aa_filter_linear_m1.35.h"
	};

	class surface;

	class surface
	{
		protected:
			int direction(int x1, int y1, int x2, int y2);
			virtual void circle_points(int x, int y, int dx, int dy);
			virtual void ellipse_points(int x, int y, int dx, int dy);
			virtual void circle_points_aa(int x, int y, int dx, int dy);
			virtual void ellipse_points_aa(int x, int y, int dx, int dy);

			// color composition methods
			byte composite_a_over_b(byte Ca, byte Cb, float Aa, float Ab, float Ao)
			{
				// the alphas are floats in the range [0,1]
				// the colors are ints in the range [0,255]
				return (byte)((Aa*Ca + Ab*Cb*(1-Aa))/Ao);
			}

			byte composite_alpha(byte Aa, byte Ab)
			{
				// here the alphas are ints in the range [0,255]
				return ((Aa+Ab)-(Aa*Ab/255));
			}

			byte filter_aa(double distance) 
			{ 
				return alpha_filter[(int)(175*distance)]; 
			}		
			
			// member variables common to all surfaces
			rectangle rect_;      // defines its position on the screen, width and height
			byte alpha_;          // defines its opacity (255 is opaque)
			graphics_context gc_; // stores graphics settings used by draw routines

		public:
			surface() { };
			surface(const rectangle &rect) : rect_(rect) { };
			virtual ~surface() { };

			// property methods
			rectangle rect() const { return rect_; };
			byte alpha() const { return alpha_; };
			void alpha(byte a) { alpha_ = a; };
			graphics_context gc() const { return gc_; };
			void gc(graphics_context &new_gc) { gc_ = new_gc; }; 		

			// methods which MUST be implemented in derived classes
			virtual void draw_pixel(int x, int y, color clr) = 0;
			virtual void draw_pixel_aa(int x, int y, double distance, color clr) = 0;
			virtual color get_pixel(int x, int y) = 0;
			// format: "0xRRGGBBAA", 0-255, alpha 255 being opaque
			virtual color gen_color(const std::string &str_color) = 0;
			virtual color gen_color(byte r, byte g, byte b, byte a) = 0;
			virtual void lock(rectangle &rect, int flags, color** buf, int &stride) = 0;
			virtual void unlock() = 0;
			virtual void blit(surface &p_surface, rectangle &src_rect, 
					rectangle &dst_rect) = 0;

			/* A NOTE ON EFFICIENCY AND PERFORMANCE
			 * ----------------------------------
			 * The following draw routines have default implementations
			 * in this base class, HOWEVER, they use the virtual draw_pixel method
			 * which adds overhead for EVERY pixel drawn.  Efficient drawing
			 * requires that derived surface classes implement a private put_pixel()
			 * method and use it in virtual drawing routines (copy these and replace
			 * draw_pixel with your private put_pixel).  You should also implement
			 * a private pixel_at() method and use it in a private put_pixel_aa() 
			 * rather than using the the virtual get_pixel() (for the same reason).
			 * (this applies to circle/ellipse_points methods above as well)
			 */

			// non antialiased draw routines
			virtual void draw_line(int x1, int y1, int x2, int y2);
			virtual void draw_arc(const rectangle &rect, int quadrant);
			virtual void draw_arc(int x1, int y1, int x2, int y2, int quadrant);
			virtual void draw_rect(const rectangle &rect);
			virtual void draw_rect(int x1, int y1, int x2, int y2);
			virtual void draw_circle(int x, int y, int radius);
			virtual void draw_circle(const rectangle &rect);
			virtual void draw_ellipse(int x, int y, int a, int b);
			virtual void draw_ellipse(const rectangle &rect);
			virtual void draw_poly(const std::vector<point> points);
			virtual void draw_text(int x, int y, const std::string &text);

			// antialiased draw routines
			virtual void draw_line_aa(int x1, int y1, int x2, int y2);
			virtual void draw_arc_aa(const rectangle &rect, int quadrant);
			virtual void draw_arc_aa(int x1, int y1, int x2, int y2, int quadrant);
			virtual void draw_rect_aa(const rectangle &rect);
			virtual void draw_rect_aa(int x1, int y1, int x2, int y2);
			virtual void draw_circle_aa(int x, int y, int radius);
			virtual void draw_circle_aa(const rectangle &rect);
			virtual void draw_ellipse_aa(int x, int y, int a, int b);
			virtual void draw_ellipse_aa(const rectangle &rect);
			virtual void draw_poly_aa(const std::vector<point> points);

			// non antialiased fill routines
			virtual void fill_rect(int x1, int y1, int x2, int y2);
			virtual void fill_rect(const rectangle &rect);
			virtual void fill_circle(int x, int y, int radius);
			virtual void fill_circle(const rectangle &rect);
			virtual void fill_ellipse(int x, int y, int a, int b);
			virtual void fill_ellipse(const rectangle &rect);
			virtual void fill_poly(const std::vector<point> points);

			// antialiased fill routines
			virtual void fill_rect_aa(const rectangle &rect);
			virtual void fill_rect_aa(int x1, int y1, int x2, int y2);
			virtual void fill_circle_aa(int x, int y, int radius);
			virtual void fill_circle_aa(const rectangle &rect);
			virtual void fill_ellipse_aa(int x, int y, int a, int b);
			virtual void fill_ellipse_aa(const rectangle &rect);
			virtual void fill_poly_aa(const std::vector<point> points);
	};
} //end namespace stk

#endif
