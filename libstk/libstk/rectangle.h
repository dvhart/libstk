#ifndef STK_RECTANGLE_H
#define STK_RECTANGLE_H

#include "stk.h" // is this where we should define MIN,MAX,color etc... ?
#include "point.h"
#include "exceptions.h"

namespace stk
{
	class rectangle
	{
		private:
			point p1_, p2_;
		public:
			rectangle(int x1 = 0, int y1 = 0, int w = 0, int h = 0) : 
				p1_(point(x1, y1)), p2_(x1 + w, y1 + h) 
			{
				// enforce p1 as upper left coordinate
				// (this assume 0,0 is the upper left corner of the screen (like SDL))
				if ((x1 > x1+w) || (y1 > y1+h))
					throw error_message_exception(
							"x1,y1 must be less than x2,y2");
			};
			rectangle(const point &p1, const point &p2) : p1_(p1), p2_(p2)
			{
				// enforce p1 as upper left coordinate
				// (this assume 0,0 is the upper left corner of the screen (like SDL))
				if ((p1.x() > p2.x()) || (p1.y() > p2.y()))
					throw error_message_exception(
							"p1.x(),p1.y() must be less than p1.x(),p1.y()");
			}; 

			// getters
			int x1() const { return p1_.x(); };
			int y1() const { return p1_.y(); };
			int x2() const { return p2_.x(); };
			int y2() const { return p2_.y(); };
			int w() const { return p2_.x() - p1_.x(); };
			int h() const { return p2_.y() - p1_.y(); };

			// setters
			void x1(int val) { p1_.x(val); };
			void y1(int val) { p1_.y(val); };
			void x2(int val) { p2_.x(val); };
			void y2(int val) { p2_.y(val); };
			void w(int val) { p2_.x(val + p1_.x()); };
			void h(int val) { p2_.y(val + p1_.y()); };

			// utilities
			bool contains(int x, int y) 
			{ return (x > p1_.x() && x < p2_.x() && y > p1_.y() && y < p2_.y()); }
			
			bool intersects(rectangle& rect)
			{
				bool x_in = ((rect.x1() > x1() && rect.x1() < x2) || (rect.x2() > x1() && rect.x2() < x2));
				bool x_span = (rect.x1() <= x1() && rect.x2() >= x2());
				bool y_in = ((rect.y1() > y1() && rect.y1() < y2) || (rect.y2() > y1() && rect.y2() < y2));
				bool y_span (rect.y1() <= y1() && rect.y2() >= y2());
				return ((x_span || x_in) && (y_span || y_in)) 
			}

			// operators
			const rectangle& operator+(const rectangle& rect) const
			{
				return rectangle(MIN(x1(), rect.x1()), MIN(y1(), rect.y1()), 
						             MAX(x2(), rect.x2()), MAX(y2(), rect.y2()));
			}
			
			const rectangle& operator+=(const rectangle& rect)
			{
				x1(MIN(x1(), rect.x1())); y1(MIN(y1(), rect.y1()));
				x2(MAX(x2(), rect.x2())); y2(MAX(y2(), rect.y2()));
				return *this;
			}

	};
}

#endif
