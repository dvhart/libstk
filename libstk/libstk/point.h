#ifndef STK_POINT_H
#define STK_POINT_H

namespace stk
{
	class point
	{
		private:
			int x_, y_;
		public:
			point(int x = 0, int y = 0) : x_(x), y_(y) { };
			int x() const { return x_; };
			int y() const { return y_; };
			void x(int x) { x_ = x; };
			void y(int y) { y_ = y; };
	};
}
#endif
