/******************************************************************************
 *    FILENAME: surface.h
 * DESCRIPTION: An abstract base class providing the interface for all surface
 *              backends.
 *     AUTHORS: Darren Hart, Vernon Mauery, Marc Straemke 
 *  START DATE: 08/Sep/2002  LAST UPDATE: 13/May/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available
 *              with the source as license.txt or at 
 *              http://www.libstk.org/index.php?page=docs/license
 *****************************************************************************/

#ifndef STK_SURFACE_H
#define STK_SURFACE_H

#include <iostream>
#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "libstk/point.h"
#include "libstk/rectangle.h"
#include "libstk/graphics_context.h"
#include "libstk/stk.h"

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

	class surface
	{
		public:
			typedef boost::shared_ptr<surface> ptr;
			typedef boost::weak_ptr<surface> weak_ptr;
	
		private:
			
		protected:
			surface() : offset_(0,0) { cout << "surface::surface()" << endl; }
			surface(const rectangle &rect) : rect_(rect),offset_(0,0) { cout << "surface::surface(rectangle)" << endl; }
			
			/// Position on the screen, width and height
			rectangle rect_;      
			/// No pixels may be drawn outside this area ,given in screen coordinates
			rectangle clip_rect_; 
			// Offset from the coordinate system origin, where the current drawing operation should take place
			point offset_;		  
			// Opacity (255 is opaque)
			byte alpha_;          
			/// Stores graphics settings used by draw routines
			graphics_context::ptr gc_; 

		public:
			virtual ~surface() { cout << "surface::~surface()" << endl; }

			// inline property methods
			rectangle rect() const { return rect_; }
			rectangle clip_rect() const { return clip_rect_; }
			void clip_rect(const rectangle& clip_rectangle) { clip_rect_ = clip_rectangle; }
			point offset() const { return offset_;}			
			void offset(const point& newoffset) { offset_ = newoffset; }
			byte alpha() const { return alpha_; }
			void alpha(byte a) { alpha_ = a; }
			graphics_context::ptr gc() const { return gc_; }
			void gc(graphics_context::ptr new_gc) { gc_ = new_gc; }
			void x1(int x) { rect_.x1(x); }
			void y1(int y) { rect_.y1(y); }

			// The following methods MUST be implemented in all backend surface classes.
			// In addition to:
			//   void put_pixel(int x, int y); 
			//   void put_pixel_aa(int x, int y, double distance, color clr);  
			//   void put_pixel_aa(int x, int y, unsigned char alpha_a, color clr) 
			//   color get_pixel(int x, int y);
			// format: "0xRRGGBBAA", [0-255], alpha 255 being opaque
			virtual color gen_color(const std::string& str_color) const = 0;
			virtual color gen_color(byte r, byte g, byte b, byte a) const = 0;
			/// Set buf to point to a framebuffer in RRGGBBAA format.  
			/// If this is the native pixel format, then writing to this buffer is
			/// reflected immediate, if not, it will be translated to the appropriate
			/// format and written to the real framebuffer when unlock is called
			/// \param rect The area of the framebuffer requested
			/// \param flags MARC WHAT DID YOU HAVE IN MIND HERE ? << marc:Read only, read write for example!
			/// \param buf The pointer to be set to the start of the area in the framebuffer
			/// \param stride The length in pixels between the first column in each row
			virtual void lock(rectangle& rect, int flags, color** buf, int& stride) = 0;
			/// Signal that the user is the done with the framebuffer area aquired with lock.
			/// If the native pixel format is not RRGGBBAA, then this will convert
			/// temporary framebuffer data to the native pixel format and write it 
			/// to the real framebuffer.
			virtual void unlock() = 0;
			virtual void update(const rectangle& u_rect = rectangle()) = 0;

			// The following methods are defined in surface_impl 
			virtual void draw_pixel(int x, int y, color clr) = 0;
			virtual void draw_pixel_aa(int x, int y, double distance, color clr) = 0;
			virtual void draw_pixel_aa(int x, int y, unsigned char alpha_a, color clr) = 0;
			virtual color read_pixel(int x, int y) = 0;
			virtual void blit(surface &dst_surface) = 0;

			// non antialiased draw routines
			virtual void draw_line(int x1, int y1, int x2, int y2) = 0;
			/* draw_arc() routines draw an arc with the rectangle passed or defined
			 * and uses quadrant to determine how the arc gets drawn.  quadrant is
			 * one of (ur)|(lr)|(ll)|(ul)_quadrant as defined in surface_impl.h and 
			 * the arc is drawn as that portion of the quarter ellipse inscribed 
			 * within the rectangle.
			 */
			virtual void draw_arc(const rectangle& rect, int quadrant) = 0;
			virtual void draw_arc(int x1, int y1, int x2, int y2, int quadrant) = 0;
			virtual void draw_rect(const rectangle& rect) = 0;
			virtual void draw_rect(int x1, int y1, int x2, int y2) = 0;
			virtual void draw_circle(int x, int y, int radius) = 0;
			virtual void draw_circle(const rectangle& rect) = 0;
			virtual void draw_ellipse(int x, int y, int a, int b) = 0;
			virtual void draw_ellipse(const rectangle& rect) = 0;
			/* draw_poly() receives a vector of points (at least 2) and connects
			 * sequential points and finally the last to the first.
			 */
			virtual void draw_poly(const std::vector<point> points) = 0;
			// define the text interface... should we pass a rect and
			// clip to that?  --vhmauery
			// no, the clip rect should be set by the theme before drawing 
			//   we still have to add surface::slip(const rectangle& clip_rect) to surface...
			virtual void draw_text(const rectangle& rect, const std::wstring& text, int kerning_mode = 0) = 0;

			// antialiased draw routines
			virtual void draw_line_aa(int x1, int y1, int x2, int y2) = 0;
			virtual void draw_arc_aa(const rectangle& rect, int quadrant) = 0;
			virtual void draw_arc_aa(int x1, int y1, int x2, int y2, int quadrant) = 0;
			virtual void draw_rect_aa(const rectangle& rect) = 0;
			virtual void draw_rect_aa(int x1, int y1, int x2, int y2) = 0;
			virtual void draw_circle_aa(int x, int y, int radius) = 0;
			virtual void draw_circle_aa(const rectangle& rect) = 0;
			virtual void draw_ellipse_aa(int x, int y, int a, int b) = 0;
			virtual void draw_ellipse_aa(const rectangle& rect) = 0;
			virtual void draw_poly_aa(const std::vector<point> points) = 0;

			// non antialiased fill routines
			virtual void fill_rect(int x1, int y1, int x2, int y2) = 0;
			virtual void fill_rect(const rectangle& rect) = 0;
			virtual void fill_circle(int x, int y, int radius) = 0;
			virtual void fill_circle(const rectangle& rect) = 0;
			virtual void fill_ellipse(int x, int y, int a, int b) = 0;
			virtual void fill_ellipse(const rectangle& rect) = 0;
			virtual void fill_poly(const std::vector<point> points) = 0;

			// antialiased fill routines
			virtual void fill_rect_aa(const rectangle& rect) = 0;
			virtual void fill_rect_aa(int x1, int y1, int x2, int y2) = 0;
			virtual void fill_circle_aa(int x, int y, int radius) = 0;
			virtual void fill_circle_aa(const rectangle& rect) = 0;
			virtual void fill_ellipse_aa(int x, int y, int a, int b) = 0;
			virtual void fill_ellipse_aa(const rectangle& rect) = 0;
			virtual void fill_poly_aa(const std::vector<point> points) = 0;

			// image routines
			virtual void draw_image(int x, int y, boost::shared_ptr<image> img) = 0;
	};

} //end namespace stk

#endif
