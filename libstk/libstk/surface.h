/***************************************************************************
 *                                                                         *
 * 	surface.h - surface base class header                                  *
 * 	---------------------------------------------------------------------  *
 * 	begin                : Tue September 10 2002                           *
 * 	copyright            : (C) 2002 by Darren Hart                         *
 * 	email                : dvhart@byu.edu                                  *
 * 	                                                                       *
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the LGPL Limited General Public License.        *
 *                                                                         *
 ***************************************************************************/


#ifndef STK_SURFACE_H
#define STK_SURFACE_H

#include <iostream>
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "point.h"
#include "rectangle.h"
#include "graphics_context.h"
#include "stk.h"

using std::cout;
using std::endl;

/* stk::surface provides an interface to be implemented by all backend 
 * surface implementations.  In general, one creates a screen from one of
 * the derived surface classes (ie surface_sdl or surface_dfb) and several
 * other surfaces of the same type.  Each surface may be drawn on 
 * individually.  When drawing is complete, each surface should be "blitted"
 * to the screen (or a "lower" surface) until everything has been blitted
 * to the screen.  The screen is then "flipped" and the updates appears on
 * the screen.  All drawing characteristics are determined from the
 * graphics_context member variable "gc_".
 */

namespace stk
{
	class image;

	// direction constants (used in the draw_line routines)
	const int DOT   = 0x00;
	const int LR    = 0x01;      // left to right horizontal
	const int RL    = 0x02;      // right to left horizontal
	const int UP    = 0x04;      // bottom to top verticle
	const int DN    = 0x08;      // top to bottom verticle
	const int S0    = 0x10;      // shallow slope (< 1.0)
	const int S1    = 0x20;      // steep slot    (>= 1.0)
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

	// static antialiased alpha falloff filter
	static byte alpha_filter[256] =
	{
		#include "aa_filter.h"
	};

	class surface
	{
		public:
			typedef boost::shared_ptr<surface> ptr;
			typedef boost::weak_ptr<surface> weak_ptr;
	
		private:
			
		protected:
			surface() { cout << "surface::surface()" << endl; }
			surface(const rectangle &rect) : rect_(rect) { cout << "surface::surface(rectangle)" << endl; }

			/* direction() returns one of the direction constants defined above.  
			 * Line drawing routines use it to determine which of the twelve 
			 * directions to use when drawing the line.  This is needed for both
			 * accuracy and performance.
			 */
			int direction(int x1, int y1, int x2, int y2);

			/* The following methods are used by the various draw_circle and
			 * draw_ellipse routines.  They take advantage of circles' eight way
			 * symmetry and ellipses' four way summetry.
			 */
			virtual void circle_points(int x, int y, int dx, int dy);
			virtual void ellipse_points(int x, int y, int dx, int dy);
			virtual void circle_points_aa(int x, int y, int dx, int dy);
			virtual void ellipse_points_aa(int x, int y, int dx, int dy);

			/* These composite methods are used to determine the resulting color
			 * of a pixel when drawing in antialiased mode.  By using the a over b
			 * composite model combined with the alpha composition, intermediate
			 * pixel values can be determined (we can render top down instead of
			 * bottom up).
			 */
			byte composite_a_over_b(byte Ca, byte Cb, float Aa, float Ab, float Ao)
			{
				// the alphas are floats in the range [0,1]
				// the colors are bytes in the range [0,255]
				return (byte)((Aa*Ca + Ab*Cb*(1-Aa))/Ao);
			}
			byte composite_alpha(byte Aa, byte Ab)
			{
				// here the alphas are bytes in the range [0,255]
				return ((Aa+Ab)-(Aa*Ab/255));
			}

			/* Index the static antialiased alpha filter.  Returns an alpha value
			 * from [0-255], 255 being opaque.
			 */
			byte filter_aa(double distance) 
			{ 
				return alpha_filter[(int)(175*distance)]; 
			}		
			
			// member variables common to all surfaces
			rectangle rect_;      // position on the screen, width and height
			byte alpha_;          // opacity (255 is opaque)
			graphics_context::ptr gc_; // stores graphics settings used by draw routines

		public:
			virtual ~surface() { cout << "surface::~surface()" << endl; }

			// inline property methods
			rectangle rect() const { return rect_; }
			// FIXME: we may not need alpha now that we only have one surface..
			// it may be nice for special cases where we need another surface
			// need to think on it -- dvhart
			byte alpha() const { return alpha_; }
			void alpha(byte a) { alpha_ = a; }
			// FIXME: should we be using smart pointers here with gcs and rects ?
			graphics_context::ptr gc() const { return gc_; }
			void gc(graphics_context::ptr new_gc) { gc_ = new_gc; }
			void x1(int x) { rect_.x1(x); }
			void y1(int y) { rect_.y1(y); }

			/* The following methods MUST be implemented in derived classes.  They 
			 * are not purely virtual here since we must be able to instantiate a 
			 * surface class in certain situations.
			 */
			virtual void draw_pixel(int x, int y, color clr) { }
			virtual void draw_pixel_aa(int x, int y, double distance, color clr) { }
			virtual void draw_pixel_aa(int x, int y, unsigned char alpha_a, color clr) { }
			virtual color read_pixel(int x, int y) const { }
			// format: "0xRRGGBBAA", [0-255], alpha 255 being opaque
			virtual color gen_color(const std::string& str_color) const { }
			virtual color gen_color(byte r, byte g, byte b, byte a) const { }
			virtual void lock(rectangle &rect, int flags, color** buf, int &stride) { }
			virtual void unlock() { }
			virtual void blit(surface &dst_surface) { }
			virtual void update(const rectangle& u_rect=rectangle()) { }

			/* A NOTE ON EFFICIENCY AND PERFORMANCE
			 * ----------------------------------
			 * The following draw routines have default implementations
			 * in this base class, HOWEVER, they use the virtual draw_pixel method
			 * which adds overhead for EVERY pixel drawn.  Efficient drawing
			 * requires that derived surface classes implement a private put_pixel()
			 * method and use it in virtual drawing routines (copy these and replace
			 * draw_pixel with your private put_pixel).  You should also implement
			 * a private get_pixel() method and use it in a private put_pixel_aa() 
			 * rather than using the the virtual read_pixel() (for the same reason).
			 * (this applies to circle/ellipse_points methods above as well)
			 */

			// non antialiased draw routines
			virtual void draw_line(int x1, int y1, int x2, int y2);
			/* draw_arc() routines draw an arc with the rectangle passed or defined
			 * and uses quadrant to determine how the arc gets drawn.  quadrant is
			 * one of (ur)|(lr)|(ll)|(ul)_quadrant as defined above and the arc is
			 * drawn as that portion of the quarter ellipse inscribed within the
			 * rectangle.
			 */
			virtual void draw_arc(const rectangle& rect, int quadrant);
			virtual void draw_arc(int x1, int y1, int x2, int y2, int quadrant);
			virtual void draw_rect(const rectangle& rect);
			virtual void draw_rect(int x1, int y1, int x2, int y2);
			virtual void draw_circle(int x, int y, int radius);
			virtual void draw_circle(const rectangle& rect);
			virtual void draw_ellipse(int x, int y, int a, int b);
			virtual void draw_ellipse(const rectangle& rect);
			/* draw_poly() receives a vector of points (at least 2) and connects
			 * sequential points and finally the last to the first.
			 */
			virtual void draw_poly(const std::vector<point> points);
			// define the text interface... should we pass a rect and
			// clip to that?  --vhmauery
			// no, the clip rect should be set by the theme before drawing 
			//   we still have to add surface::slip(const rectangle& clip_rect) to surface...
			virtual void draw_text(const rectangle& rect, const std::wstring &text, int kerning_mode=0);

			// antialiased draw routines
			virtual void draw_line_aa(int x1, int y1, int x2, int y2);
			virtual void draw_arc_aa(const rectangle& rect, int quadrant);
			virtual void draw_arc_aa(int x1, int y1, int x2, int y2, int quadrant);
			virtual void draw_rect_aa(const rectangle& rect);
			virtual void draw_rect_aa(int x1, int y1, int x2, int y2);
			virtual void draw_circle_aa(int x, int y, int radius);
			virtual void draw_circle_aa(const rectangle& rect);
			virtual void draw_ellipse_aa(int x, int y, int a, int b);
			virtual void draw_ellipse_aa(const rectangle& rect);
			virtual void draw_poly_aa(const std::vector<point> points);

			// non antialiased fill routines
			virtual void fill_rect(int x1, int y1, int x2, int y2);
			virtual void fill_rect(const rectangle& rect);
			virtual void fill_circle(int x, int y, int radius);
			virtual void fill_circle(const rectangle& rect);
			virtual void fill_ellipse(int x, int y, int a, int b);
			virtual void fill_ellipse(const rectangle& rect);
			virtual void fill_poly(const std::vector<point> points);

			// antialiased fill routines
			virtual void fill_rect_aa(const rectangle& rect);
			virtual void fill_rect_aa(int x1, int y1, int x2, int y2);
			virtual void fill_circle_aa(int x, int y, int radius);
			virtual void fill_circle_aa(const rectangle& rect);
			virtual void fill_ellipse_aa(int x, int y, int a, int b);
			virtual void fill_ellipse_aa(const rectangle& rect);
			virtual void fill_poly_aa(const std::vector<point> points);

			// image routines
			virtual void draw_image(const rectangle& rect, boost::shared_ptr<image> img);
	};

} //end namespace stk

#endif
