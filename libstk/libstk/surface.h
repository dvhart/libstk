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

#include <SDL/SDL.h>
#include <string>
#include <vector>

// this is opposite from the docs, but it works
#if SDL_BYTEORDER != SDL_BIG_ENDIAN
#define RMASK  0xff000000
#define GMASK  0x00ff0000
#define BMASK  0x0000ff00
#define AMASK  0x000000ff
#define RSHIFT 24
#define GSHIFT 16
#define BSHIFT 8
#define ASHIFT 0
#else
#define RMASK 0x000000ff
#define GMASK 0x0000ff00
#define BMASK 0x00ff0000
#define AMASK 0xff000000
#define RSHIFT 0
#define GSHIFT 8
#define BSHIFT 16
#define ASHIFT 24
#endif

#define MIN(A,B) (((A)<(B))?(A):(B))

namespace stk
{

class point
{
private:
    int x_, y_;
public:
    point(int x = 0, int y = 0) : x_(x), y_(y)
    { }
    ;
    inline int x() const
    {
        return x_;
    };
    inline int y() const
    {
        return y_;
    };
    inline void x(int x)
    {
        x_ = x;
    };
    inline void y(int y)
    {
        y_ = y;
    };
};

class edge
{
private:
    int ymax_, xbot_, xinc_num_, xinc_den_, xinc_, xinc_dir_;
public:
    edge(int ymax, int xbot, int xinc_num, int xinc_den) :
            ymax_(ymax), xbot_(xbot), xinc_num_(xinc_num),
        xinc_den_(xinc_den), xinc_(0), xinc_dir_(((xinc_num>0)?1:-1))
    {}
    ;
    inline int ymax() const
    {
        return ymax_;
    };
    inline int xbot() const
    {
        return xbot_;
    };
    inline int xinc_num() const
    {
        return xinc_num_;
    };
    inline int xinc_den() const
    {
        return xinc_den_;
    };
    inline int xinc() const
    {
        return xinc_;
    };
    inline void ymax(int ymax)
    {
        ymax_ = ymax;
    };
    inline void xbot(int xbot)
    {
        xbot_ = xbot;
    };
    inline void xinc_num(int xinc_num)
    {
        xinc_num_ = xinc_num;
    };
    inline void xinc_den(int xinc_den)
    {
        xinc_den_ = xinc_den;
    };
    inline void xinc(int xinc)
    {
        xinc_ = xinc;
    };
    void step()
    {
        xinc_ += xinc_num_;
        while (abs(xinc_) >= xinc_den_)
        {
            xbot_ += xinc_dir_;
            if (xinc_dir_ > 0)
                xinc_ -= xinc_den_;
            else
                xinc_ += xinc_den_;
        }
    }
    bool operator<(const edge &ed) const
    {
        if (xbot_ < ed.xbot())
            return true;
        else
            return false;
    };

};

static Uint8 alpha_filter[256] =
    {
        //#include "aa_filter_linear_m1.h"
        //#include "aa_filter_linear_m1.25.h"
        //#include "aa_filter_linear_m1.3.h"
#include "aa_filter_linear_m1.35.h"
        //#include "aa_filter_linear_m1.75.h"
        //#include "aa_filter_linear_m2.h"
    };

class surface;

class surface
{
private:
    SDL_Surface *surface_;
    void circle_points(int x, int y, int dx, int dy, Uint32 color);
    void ellipse_points(int x, int y, int dx, int dy, Uint32 color);
    void circle_points_aa(int x, int y, int dx, int dy, Uint32 color);
    void ellipse_points_aa(int x, int y, int dx, int dy, Uint32 color);

public:
    surface(SDL_Surface &new_surface);
    surface(SDL_Rect &rect);
    ~surface();

    // non antialiased draw routines
    void draw_pixel(int x, int y, Uint32 color);
    void draw_line(int x1, int y1, int x1, int y2, Uint32 color);
    void draw_rect();
    void draw_arc();
    void draw_circle(int x, int y, int radius, Uint32 color);
    void draw_ellipse(int x, int y, int a, int b, Uint32 color);
    void draw_poly(std::vector<point> points, Uint32 color);

    // antialiased draw routines
    void draw_pixel_aa(int x, int y, Uint32 color_a, double distance);
    void draw_line_aa(int x1, int y1, int x2, int y2, Uint32 color);
    void draw_rect_aa();
    void draw_arc_aa();
    void draw_ellipse_aa();
    void draw_poly_aa(std::vector<point> points, Uint32 color);

    // non antialiased fill routines
    void fill_rect();
    void fill_circle();
    void fill_ellipse();
    void fill_poly(std::vector<point> points, Uint32 color);

    // antialiased fill routines
    void fill_rect_aa();
    void fill_circle_aa();
    void fill_ellipse_aa();
    void fill_poly_aa(std::vector<point> points, Uint32 color);

    // util methods
    Uint32 gen_color(const std::string &str_color);
    Uint32 gen_color(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    inline Uint8 filter_aa(double distance)
    {
        return alpha_filter[(int)(175*distance)];
    }
    void lock()
        ;
    void unlock();
    void blit(surface &p_surface, SDL_Rect *src_rect = NULL,
              SDL_Rect *dst_rect = NULL);
    Uint32 get_pixel(int x, int y);
    inline SDL_Surface *sdl_surface()
    {
        return surface_;
    }
    inline int height()
    {
        return surface_->h;
    };
    inline int width()
    {
        return surface_->w;
    };
};
} //end namespace stk

#endif
