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

		protected:
			
		public:
			rectangle(int x1 = 0, int y1 = 0, int w = 0, int h = 0);
			rectangle(const point &p1, const point &p2);

			// getters
			int x1() const { return p1_.x(); }
			int y1() const { return p1_.y(); }
			int x2() const { return p2_.x(); }
			int y2() const { return p2_.y(); }
			int width() const { return p2_.x() - p1_.x() + 1; } // inclusive width
			int height() const { return p2_.y() - p1_.y() + 1; } // inclusive height

			// setters
			void x1(int val) { p1_.x(val); }
			void y1(int val) { p1_.y(val); }
			void x2(int val) { p2_.x(val); }
			void y2(int val) { p2_.y(val); }
			void width(int val) { p2_.x(val + p1_.x() - 1); }
			void height(int val) { p2_.y(val + p1_.y() - 1); }

			// utilities
			bool empty() const { return (!width() && !height()); }
			bool contains(int x, int y) const;
			bool intersects(const rectangle& rect) const;

			// operators
			const rectangle operator+(const rectangle& rect) const;
			const rectangle& operator+=(const rectangle& rect);
	};	
}
std::ostream& operator<<(std::ostream& os, const stk::rectangle& rect);
#endif
