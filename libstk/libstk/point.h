#ifndef STK_POINT_H
#define STK_POINT_H

#include <boost/operators.hpp>		

namespace stk
{
	class point    
			: boost::addable< point> 
    		, boost::subtractable<point>
	{
		private:
			int x_, y_;
		public:
			point(int x = 0, int y = 0) : x_(x), y_(y) { }
			int x() const { return x_; }
			int y() const { return y_; }
			void x(int x) { x_ = x; }
			void y(int y) { y_ = y; }
		
			point operator+=(const point& rhs)	
				{
					x_+=rhs.x_;
					y_+=rhs.y_;
				}
    		point operator-=(const point& rhs)
				{
					x_-=rhs.x_;
					y_-=rhs.y_;
				}
    		
	};
}
#endif
