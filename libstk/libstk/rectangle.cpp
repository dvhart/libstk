#include <iostream>
#include "rectangle.h"

namespace stk
{
	rectangle::rectangle(int x1, int y1, int w, int h) : 
		p1_(point(x1, y1)), p2_(x1 + w, y1 + h) 
		{
			// enforce p1 as upper left coordinate
			// (this assume 0,0 is the upper left corner of the screen (like SDL))
			if ((x1 > x1+w) || (y1 > y1+h))
				throw error_message_exception(
						"x1,y1 must be less than x2,y2");
		};
	rectangle::rectangle(const point &p1, const point &p2) : p1_(p1), p2_(p2)
	{
		// enforce p1 as upper left coordinate
		// (this assume 0,0 is the upper left corner of the screen (like SDL))
		if ((p1.x() > p2.x()) || (p1.y() > p2.y()))
			throw error_message_exception(
					"p1.x(),p1.y() must be less than p1.x(),p1.y()");
	}; 

	// utilities
	bool rectangle::contains(int x, int y) 
	{ return (x > p1_.x() && x < p2_.x() && y > p1_.y() && y < p2_.y()); }

	bool rectangle::intersects(const rectangle& rect)
	{
		bool x_in = ((rect.x1() > x1() && rect.x1() < x2()) || (rect.x2() > x1() && rect.x2() < x2()));
		bool x_span = (rect.x1() <= x1() && rect.x2() >= x2());
		bool y_in = ((rect.y1() > y1() && rect.y1() < y2()) || (rect.y2() > y1() && rect.y2() < y2()));
		bool y_span (rect.y1() <= y1() && rect.y2() >= y2());
		return ((x_span || x_in) && (y_span || y_in));
	}

	// operators
	const rectangle rectangle::operator+(const rectangle& rect) const
	{
		if (empty()) return rect;

		rectangle new_rect(MIN(x1(), rect.x1()), MIN(y1(), rect.y1()), 
				MAX(x2(), rect.x2()), MAX(y2(), rect.y2()));
		return new_rect;
	}

	const rectangle& rectangle::operator+=(const rectangle& rect)
	{
		if (!empty())
		{
			x1(MIN(x1(), rect.x1())); y1(MIN(y1(), rect.y1()));
			x2(MAX(x2(), rect.x2())); y2(MAX(y2(), rect.y2()));
		}
		else
		{
			x1(rect.x1()); y1(rect.y1());
			x2(rect.x2()); y2(rect.y2());
		}
		return *this;
	}
}

std::ostream& operator<<(std::ostream& os, const stk::rectangle& rect)
{
	os << rect.x1() << "," << rect.y1() << " " << rect.x2() << "," << rect.y2() 
		 << " w:" << rect.width() << ", h:" << rect.height(); 
	return os;
}

