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

#include "surface.h"
#include "rectangle.h"
#include "point.h"
#include "edge.h"
#include "stk_types.h"
#include <vector>
#include <list>
#include <iostream>
#include <cmath>

using std::cout;
using std::endl;
using std::cerr;

namespace stk
{
	int surface::direction(int x1, int y1, int x2, int y2)
	{
		int	dir = 0;

		// determine slope (steep S1, shallow S0)
		int dy = ((y2 > y1) ? (y2 - y1) : (y1 - y2));
		int dx = ((x2 > x1) ? (x2 - x1) : (x1 - x2));
		if (dy != 0 && dx != 0)
			if (dy > dx)
				dir += S1;
			else if (dy <= dx)
				dir += S0;

		// determine verticle direction
		if (y1 < y2)
			dir += UP;
		else if (y1 > y2)
			dir += DN;

		// determine horizontal direction
		if (x1 < x2)
			dir += LR;
		else if(x1 > x2)
			dir += RL;

		return dir;
	}

	void surface::draw_line(int x1, int y1, int x2, int y2)
	{
		// determine the line direction
		int dir = direction(x1, y1, x2, y2);
		int x = x1;
		int y = y1;

		switch(dir)
		{
			case DOT:
				{
					draw_pixel(x1, y1);
				}
			case LR:
				{
					for (x; x <= x2; x++)
						draw_pixel(x, y1);
					break;
				}
			case RL:
				{
					for (x; x >= x2; x--)
						draw_pixel(x, y1);
					break;
				}
			case UP:
				{
					for (y; y <= y2; y++)
						draw_pixel(x1, y);
					break;
				}
			case DN:
				{
					for (y; y >= y2; y--)
						draw_pixel(x1, y);
					break;
				}
			case LRU_0:
				{
					int dx       = x2-x1;
					int dy       = y2-y1;
					int d        = 2*dy-dx;
					int delta_e  = 2*dy;
					int delta_ne = 2*(dy-dx);

					draw_pixel(x, y);

					while (x < x2)
					{
						if (d <= 0)
						{
							d += delta_e;
							x++;
						}
						else
						{
							d += delta_ne;
							x++;
							y++;
						}
						draw_pixel(x, y);
					}
					break;
				}
			case LRU_1:
				{
					int dx       = x2-x1;
					int dy       = y2-y1;
					int d        = 2*dx-dy;
					int delta_e  = 2*dx;
					int delta_ne = 2*(dx-dy);

					draw_pixel(x, y);

					while (y < y2)
					{
						if (d <= 0)
						{
							d += delta_e;
							y++;
						}
						else
						{
							d += delta_ne;
							y++;
							x++;
						}
						draw_pixel(x, y);
					}
					break;
				}
			case LRD_0:
				{
					int dx       = x2-x1;
					int dy       = y1-y2;
					int d        = 2*dy-dx;
					int delta_e  = 2*dy;
					int delta_ne = 2*(dy-dx);

					draw_pixel(x, y);

					while (x < x2)
					{
						if (d <= 0)
						{
							d += delta_e;
							x++;
						}
						else
						{
							d += delta_ne;
							x++;
							y--;
						}
						draw_pixel(x, y);
					}
					break;
				}
			case LRD_1:
				{
					int dx       = x2-x1;
					int dy       = y1-y2;
					int d        = 2*dx-dy;
					int delta_e  = 2*dx;
					int delta_ne = 2*(dx-dy);

					draw_pixel(x, y);

					while (y > y2)
					{
						if (d <= 0)
						{
							d += delta_e;
							y--;
						}
						else
						{
							d += delta_ne;
							x++;
							y--;
						}
						draw_pixel(x, y);
					}
					break;

				}
			case RLU_0:
				{
					int dx       = x1-x2;
					int dy       = y2-y1;
					int d        = 2*dy-dx;
					int delta_e  = 2*dy;
					int delta_ne = 2*(dy-dx);

					draw_pixel(x, y);

					while (x > x2)
					{
						if (d <= 0)
						{
							d += delta_e;
							x--;
						}
						else
						{
							d += delta_ne;
							x--;
							y++;
						}
						draw_pixel(x, y);
					}
					break;
				}
			case RLU_1:
				{
					int dx       = x1-x2;
					int dy       = y2-y1;
					int d        = 2*dx-dy;
					int delta_e  = 2*dx;
					int delta_ne = 2*(dx-dy);

					draw_pixel(x, y);

					while (x > x2)
					{
						if (d <= 0)
						{
							d += delta_e;
							y++;
						}
						else
						{
							d += delta_ne;
							x--;
							y++;
						}
						draw_pixel(x, y);
					}
					break;
				}
			case RLD_0:
				{
					int dx       = x1-x2;
					int dy       = y1-y2;
					int d        = 2*dy-dx;
					int delta_e  = 2*dy;
					int delta_ne = 2*(dy-dx);

					draw_pixel(x, y);

					while (x > x2)
					{
						if (d <= 0)
						{
							d += delta_e;
							x--;
						}
						else
						{
							d += delta_ne;
							x--;
							y--;
						}
						draw_pixel(x, y);
					}
					break;

					break;
				}
			case RLD_1:
				{
					int dx       = x1-x2;
					int dy       = y1-y2;
					int d        = 2*dx-dy;
					int delta_e  = 2*dx;
					int delta_ne = 2*(dx-dy);

					draw_pixel(x, y);

					while (x > x2)
					{
						if (d <= 0)
						{
							d += delta_e;
							y--;
						}
						else
						{
							d += delta_ne;
							x--;
							y--;
						}
						draw_pixel(x, y);
					}

					break;
				}
		}
	}

	void surface::draw_arc(const rectangle &rect, int quadrant)
	{ cout << "surface::draw_arc - not implemented" << endl; }
	
	void surface::draw_arc(int x1, int y1, int x2, int y2, int quadrant)
	{ cout << "surface::draw_arc - not implementd" << endl; }
	
	void surface::draw_rect(const rectangle &rect)
	{ cout << "surface::draw_rect - not implemented" << endl; }
		
	void surface::draw_rect(int x1, int y1, int x2, int y2)
	{ cout << "surface::draw_rect - not implemented" << endl; }
			
	void surface::circle_points(int x, int y, int dx, int dy)
	{
		draw_pixel(x+dx, y+dy);
		draw_pixel(y+dx, x+dy);
		draw_pixel(y+dx, -x+dy);
		draw_pixel(x+dx, -y+dy);
		draw_pixel(-x+dx, -y+dy);
		draw_pixel(-y+dx, -x+dy);
		draw_pixel(-y+dx, x+dy);
		draw_pixel(-x+dx, y+dy);
	}

	void surface::draw_circle(int x, int y, int radius)
	{
		int xp = 0;
		int yp = radius;
		int d = 1 - radius;
		int delta_e = 3;
		int delta_se = -2*radius+5;
		circle_points(xp, yp, x, y);

		while (yp > xp)
		{
			if (d < 0)
			{
				d += delta_e;
				delta_e += 2;
				delta_se += 2;
			}
			else
			{
				d += delta_se;
				delta_e += 2;
				delta_se += 4;
				yp--;
			}
			xp++;
			circle_points(xp, yp, x, y);
		}
	}

	void surface::draw_circle(const rectangle &rect)
	{
		draw_circle(rect.x1() + rect.w()/2, rect.y1() + rect.h()/2, rect.w()/2);
	}
		
	void surface::ellipse_points(int x, int y, int dx, int dy)
	{
		draw_pixel(x+dx, y+dy);
		draw_pixel(x+dx, -y+dy);
		draw_pixel(-x+dx, -y+dy);
		draw_pixel(-x+dx, y+dy);
	}

	void surface::draw_ellipse(int x, int y, int a, int b)
	{
		int xp = 0;
		int yp = b;

		// some pre-calculated decisions variables
		int a_sq = a*a;
		int b_sq = b*b;
		int a_2_sq = a_sq + a_sq;               // 2*a*a
		int b_2_sq = b_sq + b_sq;               // 2*b*b
		int a_2_sq_p_b_2_sq = a_2_sq + b_2_sq;  // 2*a*a + 2*b*b

		// draw the starting pixels
		ellipse_points(xp, yp, x, y);

		// region 1
		double d1 = b_sq - a_sq*b + 0.25*a_sq;
		int delta_e = b_2_sq + b_sq; // 3*b_sq
		int delta_se = delta_e + a_sq*(-2*b+2); // 3*b_sq + a_sq*(-2*b+2)
		while (a_sq*(yp-0.5) > b_sq*(xp+1))
		{
			if (d1 < 0) // E
			{
				d1 += delta_e;
				delta_e += b_2_sq;
				delta_se += b_2_sq;
			}
			else       // SE
			{
				d1 += delta_se;
				delta_e += b_2_sq;
				delta_se += a_2_sq_p_b_2_sq;
				yp--;
			}
			xp++;
			ellipse_points(xp, yp, x, y);
		}

		//region 2
		double d2 = b_sq*(xp+0.5)*(xp+0.5)+a_sq*(yp-1)*(yp-1)-a_sq*b_sq;
		int delta_s = a_sq*(-2*yp+3);
		while (yp > 0)
		{
			if (d2 < 0)  // SE
			{
				d2 += delta_se;
				delta_s += a_2_sq;
				delta_se += a_2_sq_p_b_2_sq;
				xp++;
			}
			else         // S
			{
				d2 += delta_s;
				delta_s += a_2_sq;
				delta_se += a_2_sq;
			}
			yp--;
			ellipse_points(xp, yp, x, y);
		}
	}

	void surface::draw_ellipse(const rectangle &rect)
	{
		cout << "surface::draw_ellipse - did I get a and b right?" << endl;
		draw_ellipse(rect.x1() + rect.w()/2, rect.y1() + rect.h()/2, 
				rect.w()/2, rect.h()/2);
	}

	void surface::draw_poly(std::vector<point> points)
	{
		if (points.size() < 1)
		{
			cout << "surface::draw_poly - no points to draw" << endl;
		}
		std::vector<point>::iterator p_iter_a;
		std::vector<point>::iterator p_iter_b = points.begin();
		while (1)
		{
			p_iter_a = p_iter_b;
			p_iter_b++;
			if (p_iter_b == points.end())
				break;
			draw_line(p_iter_a->x(), p_iter_a->y(),
					p_iter_b->x(), p_iter_b->y());
		}
		p_iter_b = points.begin();
		draw_line(p_iter_a->x(), p_iter_a->y(),
				p_iter_b->x(), p_iter_b->y());
	}

	void surface::draw_text(int x, int y, const std::string &text)
	{
		// CODEC ...
		cout << "surface::draw_text - not implemented" << endl;
	}

	// antialiased draw routines
	void surface::draw_line_aa(int x1, int y1, int x2, int y2)
	{
		// determine the line direction
		int dir = direction(x1, y1, x2, y2);
		int x = x1;
		int y = y1;

		switch(dir)
		{
			case LR:
				{
					for (x; x <= x2; x++)
						draw_pixel(x, y1);
					break;
				}
			case RL:
				{
					for (x; x >= x2; x--)
						draw_pixel(x, y1);
					break;
				}
			case UP:
				{
					for (y; y <= y2; y++)
						draw_pixel(x1, y);
					break;
				}
			case DN:
				{
					for (y; y >= y2; y--)
						draw_pixel(x1, y);
					break;
				}
			case LRU_0:
				{
					int dx       = x2-x1;
					int dy       = y2-y1;
					int d        = 2*dy-dx;
					int delta_e  = 2*dy;
					int delta_ne = 2*(dy-dx);

					int two_v_dx = 0;
					double inv_denom = 1.0/(2.0*sqrt(dx*dx+dy*dy));
					double two_dx_inv_denom = 2.0*dx*inv_denom;
					draw_pixel_aa(x, y, 0);
					draw_pixel_aa(x, y+1, two_dx_inv_denom);
					draw_pixel_aa(x, y-1, two_dx_inv_denom);

					while (x < x2)
					{
						if (d <= 0)
						{
							two_v_dx = d + dx;
							d += delta_e;
							x++;
						}
						else
						{
							two_v_dx = d - dx;
							d += delta_ne;
							x++;
							y++;
						}
						draw_pixel_aa(x, y, two_v_dx*inv_denom);
						draw_pixel_aa(x, y+1,
								two_dx_inv_denom - two_v_dx*inv_denom);
						draw_pixel_aa(x, y-1,
								two_dx_inv_denom + two_v_dx*inv_denom);
					}
					break;
				}
			case LRU_1:
				{
					int dx       = x2-x1;
					int dy       = y2-y1;
					int d        = 2*dx-dy;
					int delta_e  = 2*dx;
					int delta_ne = 2*(dx-dy);

					int two_v_dy = 0;
					double inv_denom = 1.0/(2.0*sqrt(dy*dy+dx*dx));
					double two_dy_inv_denom = 2.0*dy*inv_denom;
					draw_pixel_aa(x, y, 0);
					draw_pixel_aa(x+1, y, two_dy_inv_denom);
					draw_pixel_aa(x-1, y, two_dy_inv_denom);

					while (y < y2)
					{
						if (d <= 0)
						{
							two_v_dy = d + dy;
							d += delta_e;
							y++;
						}
						else
						{
							two_v_dy = d - dy;
							d += delta_ne;
							y++;
							x++;
						}
						draw_pixel_aa(x, y, two_v_dy*inv_denom);
						draw_pixel_aa(x+1, y,
								two_dy_inv_denom - two_v_dy*inv_denom);
						draw_pixel_aa(x-1, y,
								two_dy_inv_denom + two_v_dy*inv_denom);
					}
					break;
				}
			case LRD_0:
				{
					int dx       = x2-x1;
					int dy       = y1-y2;
					int d        = 2*dy-dx;
					int delta_e  = 2*dy;
					int delta_ne = 2*(dy-dx);

					int two_v_dx = 0;
					double inv_denom = 1.0/(2.0*sqrt(dx*dx+dy*dy));
					double two_dx_inv_denom = 2.0*dx*inv_denom;
					draw_pixel_aa(x, y, 0);
					draw_pixel_aa(x, y+1, two_dx_inv_denom);
					draw_pixel_aa(x, y-1, two_dx_inv_denom);

					while (x < x2)
					{
						if (d <= 0)
						{
							two_v_dx = d + dx;
							d += delta_e;
							x++;
						}
						else
						{
							two_v_dx = d - dx;
							d += delta_ne;
							x++;
							y--;
						}
						draw_pixel_aa(x, y, two_v_dx*inv_denom);
						draw_pixel_aa(x, y-1,
								two_dx_inv_denom - two_v_dx*inv_denom);
						draw_pixel_aa(x, y+1,
								two_dx_inv_denom + two_v_dx*inv_denom);
					}

					break;
				}
			case LRD_1:
				{
					int dx       = x2-x1;
					int dy       = y1-y2;
					int d        = 2*dx-dy;
					int delta_e  = 2*dx;
					int delta_ne = 2*(dx-dy);

					int two_v_dy = 0;
					double inv_denom = 1.0/(2.0*sqrt(dx*dx+dy*dy));
					double two_dy_inv_denom = 2.0*dy*inv_denom;
					draw_pixel_aa(x, y, 0);
					draw_pixel_aa(x+1, y, two_dy_inv_denom);
					draw_pixel_aa(x-1, y, two_dy_inv_denom);

					while (y > y2)
					{
						if (d <= 0)
						{
							two_v_dy = d + dy;
							d += delta_e;
							y--;
						}
						else
						{
							two_v_dy = d - dy;
							d += delta_ne;
							x++;
							y--;
						}
						draw_pixel_aa(x, y, two_v_dy*inv_denom);
						draw_pixel_aa(x+1, y,
								two_dy_inv_denom - two_v_dy*inv_denom);
						draw_pixel_aa(x-1, y,
								two_dy_inv_denom + two_v_dy*inv_denom);
					}
					break;
				}
			case RLU_0:
				{
					int dx       = x1-x2;
					int dy       = y2-y1;
					int d        = 2*dy-dx;
					int delta_e  = 2*dy;
					int delta_ne = 2*(dy-dx);

					int two_v_dx = 0;
					double inv_denom = 1.0/(2.0*sqrt(dx*dx+dy*dy));
					double two_dx_inv_denom = 2.0*dx*inv_denom;
					draw_pixel_aa(x, y, 0);
					draw_pixel_aa(x, y+1, two_dx_inv_denom);
					draw_pixel_aa(x, y-1, two_dx_inv_denom);

					while (x > x2)
					{
						if (d <= 0)
						{
							two_v_dx = d + dx;
							d += delta_e;
							x--;
						}
						else
						{
							two_v_dx = d - dx;
							d += delta_ne;
							x--;
							y++;
						}
						draw_pixel_aa(x, y, two_v_dx*inv_denom);
						draw_pixel_aa(x, y+1,
								two_dx_inv_denom - two_v_dx*inv_denom);
						draw_pixel_aa(x, y-1,
								two_dx_inv_denom + two_v_dx*inv_denom);
					}
					break;
				}
			case RLU_1:
				{
					int dx       = x1-x2;
					int dy       = y2-y1;
					int d        = 2*dx-dy;
					int delta_e  = 2*dx;
					int delta_ne = 2*(dx-dy);

					int two_v_dy = 0;
					double inv_denom = 1.0/(2.0*sqrt(dy*dy+dx*dx));
					double two_dy_inv_denom = 2.0*dy*inv_denom;
					draw_pixel_aa(x, y, 0);
					draw_pixel_aa(x+1, y, two_dy_inv_denom);
					draw_pixel_aa(x-1, y, two_dy_inv_denom);

					while (x > x2)
					{
						if (d <= 0)
						{
							two_v_dy = d + dy;
							d += delta_e;
							y++;
						}
						else
						{
							two_v_dy = d - dy;
							d += delta_ne;
							x--;
							y++;
						}
						draw_pixel_aa(x, y, two_v_dy*inv_denom);
						draw_pixel_aa(x+1, y,
								two_dy_inv_denom + two_v_dy*inv_denom);
						draw_pixel_aa(x-1, y,
								two_dy_inv_denom - two_v_dy*inv_denom);
					}
					break;
				}
			case RLD_0:
				{
					int dx       = x1-x2;
					int dy       = y1-y2;
					int d        = 2*dy-dx;
					int delta_e  = 2*dy;
					int delta_ne = 2*(dy-dx);

					int two_v_dx = 0;
					double inv_denom = 1.0/(2.0*sqrt(dx*dx+dy*dy));
					double two_dx_inv_denom = 2.0*dx*inv_denom;
					draw_pixel_aa(x, y, 0);
					draw_pixel_aa(x, y+1, two_dx_inv_denom);
					draw_pixel_aa(x, y-1, two_dx_inv_denom);

					while (x > x2)
					{
						if (d <= 0)
						{
							two_v_dx = d + dx;
							d += delta_e;
							x--;
						}
						else
						{
							two_v_dx = d - dx;
							d += delta_ne;
							x--;
							y--;
						}
						draw_pixel_aa(x, y, two_v_dx*inv_denom);
						draw_pixel_aa(x, y+1,
								two_dx_inv_denom + two_v_dx*inv_denom);
						draw_pixel_aa(x, y-1,
								two_dx_inv_denom - two_v_dx*inv_denom);
					}
					break;

					break;
				}
			case RLD_1:
				{
					int dx       = x1-x2;
					int dy       = y1-y2;
					int d        = 2*dx-dy;
					int delta_e  = 2*dx;
					int delta_ne = 2*(dx-dy);

					int two_v_dy = 0;
					double inv_denom = 1.0/(2.0*sqrt(dy*dy+dx*dx));
					double two_dy_inv_denom = 2.0*dy*inv_denom;
					draw_pixel_aa(x, y, 0);
					draw_pixel_aa(x+1, y, two_dy_inv_denom);
					draw_pixel_aa(x-1, y, two_dy_inv_denom);

					while (x > x2)
					{
						if (d <= 0)
						{
							two_v_dy = d + dy;
							d += delta_e;
							y--;
						}
						else
						{
							two_v_dy = d - dy;
							d += delta_ne;
							x--;
							y--;
						}
						draw_pixel_aa(x, y, two_v_dy*inv_denom);
						draw_pixel_aa(x+1, y,
								two_dy_inv_denom + two_v_dy*inv_denom);
						draw_pixel_aa(x-1, y,
								two_dy_inv_denom - two_v_dy*inv_denom);
					}
					break;
				}
		}
	}
	
	void surface::draw_arc_aa(const rectangle &rect, int quadrant)
	{ cout << "surface::draw_arc_aa - not implemented" << endl; }
	
	void surface::draw_arc_aa(int x1, int y1, int x2, int y2, int quadrant)
	{ cout << "surface::draw_arc_aa - not implementd" << endl; }
	
	void surface::draw_rect_aa(const rectangle &rect)
	{ cout << "surface::draw_rect_aa - not implemented" << endl; }
		
	void surface::draw_rect_aa(int x1, int y1, int x2, int y2)
	{ cout << "surface::draw_rect_aa - not implemented" << endl; }

	void surface::draw_circle_aa(int x, int y, int radius)
	{ cout << "surface::draw_circle_aa - not implemented" << endl; }
		
	void surface::draw_circle_aa(const rectangle &rect)
	{
		draw_circle_aa(rect.x1() + rect.w()/2, rect.y1() + rect.h()/2, rect.w()/2);
	}
	
	void surface::draw_ellipse_aa(int x, int y, int a, int b)
	{ cout << "surface::draw_ellipse_aa - not implemented" << endl; }

	void surface::draw_ellipse_aa(const rectangle &rect)
	{
		cout << "surface::draw_ellipse_aa - did I get a and b right?" << endl;
		draw_ellipse(rect.x1() + rect.w()/2, rect.y1() + rect.h()/2, 
				rect.w()/2, rect.h()/2);
	}
		
	void surface::draw_poly_aa(std::vector<point> points)
	{
		if (points.size() < 1)
		{
			cout << "surface::draw_poly_aa - no points to draw" << endl;
		}
		std::vector<point>::iterator p_iter_a;
		std::vector<point>::iterator p_iter_b = points.begin();
		while (1)
		{
			p_iter_a = p_iter_b;
			p_iter_b++;
			if (p_iter_b == points.end())
				break;
			draw_line_aa(p_iter_a->x(), p_iter_a->y(),
					p_iter_b->x(), p_iter_b->y());
		}
		p_iter_b = points.begin();
		draw_line_aa(p_iter_a->x(), p_iter_a->y(),
				p_iter_b->x(), p_iter_b->y());
	}

	// non antialiased fill routines
	void surface::fill_rect(int x1, int y1, int x2, int y2)
	{ cout << "surface::fill_rect - not implemented" << endl; }
	
	void surface::fill_rect(const rectangle &rect)
	{ cout << "surface::fill_rect - not implemented" << endl; }
	
	void fill_circle(int x, int y, int radius)
	{ cout << "surface::fill_circle - not implemented" << endl; }
	
	void fill_circle(const rectangle &rect)
	{ cout << "surface::fill_circle - not implemented" << endl; }
	
	void fill_ellipse(int x, int y, int a, int b)
	{ cout << "surface::fill_ellipse - not implemented" << endl; }
	
	void fill_ellipse(const rectangle &rect)
	{ cout << "surface::fill_ellipse - not implemented" << endl; }

	void surface::fill_poly(std::vector<point> points)
	{
		typedef std::list<edge> edge_list;
		// FIXME: this will segfault for poly's outside the surface vertically
		std::vector<edge_list> edges(rect_.h());
		std::list<edge> active_edges;

		// populate the global edge table
		int i, ymin, ymax, xbot, xinc_num, xinc_den;
		int length = points.size();
		if (length < 3)
			return; // can't fill a polygon with less than 3 points
		// FIXME: these should be iterators
		point *prev_point;
		point *next_point = &(points[0]);
		int y = next_point->y();
		for (i = 1; i <= length; i++)
		{
			prev_point = next_point;
			if (i == length) // wrap around to the first point
				next_point = &(points[0]);
			else
				next_point = &(points[i]);
			if (prev_point->y() < next_point->y())
			{
				ymin = prev_point->y();
				xbot = prev_point->x();
				ymax = next_point->y();
				xinc_num = next_point->x()-xbot;
				xinc_den = ymax-ymin;
			}
			else
			{
				ymin = next_point->y();
				xbot = next_point->x();
				ymax = prev_point->y();
				xinc_num = prev_point->x()-xbot;
				xinc_den = ymax-ymin;
			}
			if (xinc_den != 0)
				edges[ymin].push_back(edge(ymax, xbot, xinc_num, xinc_den));
			y = MIN(ymin,y);
		}

		std::list<edge>::iterator edge_iter, edge_iter2;
		for (y; y < edges.size(); y++)
		{
			edge_iter = active_edges.begin();
			while (edge_iter != active_edges.end())
			{
				if (edge_iter->ymax() == y)
				{
					edge_iter2 = edge_iter++;
					active_edges.erase(edge_iter2);
				}
				else
				{
					edge_iter++;
				}
			}

			active_edges.merge(edges[y]);
			active_edges.sort();
			edge_iter2 = active_edges.begin();

			while (edge_iter2 != active_edges.end())
			{
				edge_iter = edge_iter2++;
				draw_line(edge_iter->xbot(), y, edge_iter2->xbot(), y);
				edge_iter->step();
				edge_iter2->step();
				edge_iter2++;
			}
		}
	}

	// antialiased fill routines
	void surface::fill_rect_aa(int x1, int y1, int x2, int y2)
	{ cout << "surface::fill_rect_aa - not implemented" << endl; }
	
	void surface::fill_rect_aa(const rectangle &rect)
	{ cout << "surface::fill_rect_aa - not implemented" << endl; }
	
	void fill_circle_aa(int x, int y, int radius)
	{ cout << "surface::fill_circle_aa - not implemented" << endl; }
	
	void fill_circle_aa(const rectangle &rect)
	{ cout << "surface::fill_circle_aa - not implemented" << endl; }
	
	void fill_ellipse_aa(int x, int y, int a, int b)
	{ cout << "surface::fill_ellipse_aa - not implemented" << endl; }
	
	void fill_ellipse_aa(const rectangle &rect)
	{ cout << "surface::fill_ellipse_aa - not implemented" << endl; }

	void surface::fill_poly_aa(std::vector<point> points)
	{
		fill_poly(points);
		draw_poly_aa(points);
	}
}
