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
	static byte alpha_filter[256] =
	{
		#include "aa_filter_linear_m1.35.h"
	};

	class surface;

	class surface
	{
		protected:
			// methods call draw_pixel and MUST be implemented in derived classes
			virtual void circle_points(int x, int y, int dx, int dy) = 0;
			virtual void ellipse_points(int x, int y, int dx, int dy) = 0;
			virtual void circle_points_aa(int x, int y, int dx, int dy) = 0;
			virtual void ellipse_points_aa(int x, int y, int dx, int dy) = 0;

			// member variables common to all surfaces
			rectangle rect_;      // defines its position on the screen, width and height
			byte alpha_;          // defines its opacity (255 is opaque)
			graphics_context gc_; // stores graphics settings used by draw routines

		public:
			surface(const rectangle &rect) : rect_(rect) { };
			virtual ~surface();

			// non antialiased draw routines
			// methods which must be implemented in the derived classes
			virtual void draw_line(int x1, int y1, int x2, int y2) = 0;
			virtual void draw_arc(rectangle &rect, int quadrant) = 0;
			virtual void draw_arc(int x1, int y1, int x2, int y2, int quadrant) = 0;
			// methods with a default implementation in this base class
			virtual void draw_rect(rectangle &rect);
			virtual void draw_rect(int x1, int y1, int x2, int y2);
			virtual void draw_circle(int x, int y, int radius);
			virtual void draw_ellipse(int x, int y, int a, int b);
			virtual void draw_poly(std::vector<point> points);

			// antialiased draw routines
			// methods which must be implemented in the derived classes
			virtual void draw_line_aa(int x1, int y1, int x2, int y2) = 0;
			virtual void draw_arc_aa() = 0;
			// methods with a default implementation in this base class
			virtual void draw_rect_aa();
			virtual void draw_ellipse_aa();
			virtual void draw_poly_aa(std::vector<point> points);

			// non antialiased fill routines
			virtual void fill_rect();
			virtual void fill_circle();
			virtual void fill_ellipse();
			virtual void fill_poly(std::vector<point> points);

			// antialiased fill routines
			virtual void fill_rect_aa();
			virtual void fill_circle_aa();
			virtual void fill_ellipse_aa();
			virtual void fill_poly_aa(std::vector<point> points);

			// util methods
			rectangle rect() const { return rect_; };
			byte alpha() const { return alpha_; };
			void alpha(byte a) { alpha_ = a; };
			graphics_context gc() const { return gc_; };
			void gc(graphics_conext &new_gc) { gc_ = new_gc; }; 
			
			// format: "0xRRGGBBAA", 0-255, alpha 255 being opaque
			virtual color gen_color(const std::string &str_color) = 0;
			virtual color gen_color(byte r, byte g, byte b, byte a) = 0;
			
			byte filter_aa(double distance) 
			{ 
				return alpha_filter[(int)(175*distance)]; 
			}
			
			virtual void lock();
			virtual void unlock();

			// FIXME
			// these need to be reworked
			virtual void blit(surface &p_surface, rectangle *src_rect = NULL,
					rectangle *dst_rect = NULL);
	};
} //end namespace stk

#endif
