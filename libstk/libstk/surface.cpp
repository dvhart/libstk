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

#include <math.h>
#include <vector>
#include <list>
#include "SDL.h"
#include "surface.h"

using namespace stk;

surface::surface(SDL_Surface &new_surface)
{
    surface_ = &new_surface;
}

surface::surface(SDL_Rect &rect)
{
    SDL_Surface *temp_surface;
    temp_surface = SDL_CreateRGBSurface(SDL_SRCALPHA, rect.w, rect.h,
                                        32, RMASK, GMASK, BMASK, AMASK);
    surface_ = SDL_DisplayFormatAlpha(temp_surface);
    SDL_FreeSurface(temp_surface);
    SDL_SetAlpha(surface_, SDL_SRCALPHA | SDL_RLEACCEL, SDL_ALPHA_OPAQUE);
}

surface::~surface()
{
    if (surface_)
        SDL_FreeSurface(surface_);
}

void surface::draw_pixel(int x, int y, Uint32 color)
{
    // DELETEME check for out of bounds
    if (x < 0 || y < 0 || x >= surface_->w || y >= surface_->h)
    {
        //throw std::string("surface::draw_pixel() - pixel coords out of bounds");
        cout << "surface::draw_pixel() - pixel coords out of bounds" << endl;
        return;
    }

    switch (surface_->format->BytesPerPixel)
    {
    case 1:                                       // Assuming 8-bpp
        {
            Uint8 *bufp;
            bufp = (Uint8 *) surface_->pixels + y * surface_->pitch + x;
            *bufp = color;
        }
        break;
    case 2:                                       // Probably 15-bpp or 16-bpp
        {
            Uint16 *bufp;
            bufp = (Uint16 *) surface_->pixels + y * surface_->pitch / 2 + x;
            *bufp = color;
        }
        break;
    case 3:                                       // Slow 24-bpp mode, usually not used
        {
            Uint8 *bufp;
            bufp = (Uint8 *) surface_->pixels + y * surface_->pitch + x * 3;
            if (SDL_BYTEORDER == SDL_LIL_ENDIAN)
            {
                bufp[0] = color;
                bufp[1] = color >> 8;
                bufp[2] = color >> 16;
            }
            else
            {
                bufp[2] = color;
                bufp[1] = color >> 8;
                bufp[0] = color >> 16;
            }
        }
        break;
    case 4:                                       // Probably 32-bpp
        {
            Uint32 *bufp;
            bufp = (Uint32 *) surface_->pixels + y * surface_->pitch / 4 + x;
            *bufp = color;
        }
        break;
    }
}

Uint32 surface::get_pixel(int x, int y)
{
    // DELETEME check for out of bounds
    if (x < 0 || y < 0 || x >= surface_->w || y >= surface_->h)
    {
        //throw std::string("surface::draw_pixel() - pixel coords out of bounds");
        cout << "surface::get_pixel() - pixel coords out of bounds" << endl;
        return 0;
    }

    switch (surface_->format->BytesPerPixel)
    {
    case 1:                                       // Assuming 8-bpp
        {
            Uint8 *bufp;
            bufp = (Uint8 *) surface_->pixels + y * surface_->pitch + x;
            return (Uint32)(*bufp);
        }
        break;
    case 2:                                       // Probably 15-bpp or 16-bpp
        {
            Uint16 *bufp;
            bufp = (Uint16 *) surface_->pixels + y * surface_->pitch / 2 + x;
            return (Uint32)(*bufp);
        }
        break;
    case 3:                                       // Slow 24-bpp mode, usually not used
        {
            // FIXME
            Uint8 *bufp;
            bufp = (Uint8 *) surface_->pixels + y * surface_->pitch + x * 3;
            Uint32 color;
            if (SDL_BYTEORDER == SDL_LIL_ENDIAN)
            {
                color =  bufp[0];
                color += (bufp[1] >> 8);
                color += (bufp[2] >> 16);
            }
            else
            {
                color =  bufp[2];
                color += (bufp[1] >> 8);
                color += (bufp[0] >> 16);
            }
            return color;
        }
        break;
    case 4:                                       // Probably 32-bpp
        {
            Uint32 *bufp;
            bufp = (Uint32 *) surface_->pixels + y * surface_->pitch / 4 + x;
            return *bufp;
        }
        break;
    }
}


// make static ?
Uint8 composite_a_over_b(Uint8 Ca, Uint8 Cb, float Aa, float Ab, float Ao)
{
    // the alphas are floats in the range [0,1]
    // the colors are ints in the range [0,255]
    return (Uint8)((Aa*Ca + Ab*Cb*(1-Aa))/Ao);
}

// make static ?
Uint8 composite_alpha(Uint8 Aa, Uint8 Ab)
{
    // here the alphas are ints in the range [0,255]
    return ((Aa+Ab)-(Aa*Ab/255));
}

void surface::draw_pixel_aa(int x, int y, Uint32 color_a, double distance)
{
    Uint8 red_a, green_a, blue_a, alpha_a;
    SDL_GetRGB(color_a, surface_->format, &red_a, &green_a, &blue_a);
    alpha_a = filter_aa((fabs(distance)));

    Uint8 red_b, green_b, blue_b, alpha_b;
    Uint32 color_b = get_pixel(x, y);
    SDL_GetRGBA(color_b, surface_->format, &red_b, &green_b, &blue_b, &alpha_b);

    Uint8 red_o, green_o, blue_o, alpha_o;
    alpha_o = composite_alpha(alpha_a, alpha_b);
    float alpha_a_f = alpha_a / 255.0;
    float alpha_b_f = alpha_b / 255.0;
    float alpha_o_f = alpha_o / 255.0;

    red_o   = composite_a_over_b(red_a, red_b, alpha_a_f, alpha_b_f, alpha_o_f);
    green_o = composite_a_over_b(green_a, green_b, alpha_a_f, alpha_b_f, alpha_o_f);
    blue_o  = composite_a_over_b(blue_a, blue_b, alpha_a_f, alpha_b_f, alpha_o_f);

    draw_pixel(x, y, gen_color(red_o, green_o, blue_o, alpha_o));
}

int direction(int x1, int y1, int x2, int y2)
{
#define DOT   0x00
#define LR    0x01
#define RL    0x02
#define UP    0x04
#define DN    0x08
#define S0    0x10
#define S1    0x20

#define LRU_0 LR+UP+S0
#define LRU_1 LR+UP+S1
#define LRD_0 LR+DN+S0
#define LRD_1 LR+DN+S1
#define RLU_0 RL+UP+S0
#define RLU_1 RL+UP+S1
#define RLD_0 RL+DN+S0
#define RLD_1 RL+DN+S1

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

void surface::draw_line(int x1, int y1, int x2, int y2, Uint32 color)
{
    // determine the line direction
    int dir = direction(x1, y1, x2, y2);
    int x = x1;
    int y = y1;

    switch(dir)
    {
    case DOT:
        {
            draw_pixel(x1, y1, color);
        }
    case LR:
        {
            for (x; x <= x2; x++)
                draw_pixel(x, y1, color);
            break;
        }
    case RL:
        {
            for (x; x >= x2; x--)
                draw_pixel(x, y1, color);
            break;
        }
    case UP:
        {
            for (y; y <= y2; y++)
                draw_pixel(x1, y, color);
            break;
        }
    case DN:
        {
            for (y; y >= y2; y--)
                draw_pixel(x1, y, color);
            break;
        }
    case LRU_0:
        {
            int dx       = x2-x1;
            int dy       = y2-y1;
            int d        = 2*dy-dx;
            int delta_e  = 2*dy;
            int delta_ne = 2*(dy-dx);

            draw_pixel(x, y, color);

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
                draw_pixel(x, y, color);
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

            draw_pixel(x, y, color);

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
                draw_pixel(x, y, color);
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

            draw_pixel(x, y, color);

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
                draw_pixel(x, y, color);
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

            draw_pixel(x, y, color);

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
                draw_pixel(x, y, color);
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

            draw_pixel(x, y, color);

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
                draw_pixel(x, y, color);
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

            draw_pixel(x, y, color);

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
                draw_pixel(x, y, color);
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

            draw_pixel(x, y, color);

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
                draw_pixel(x, y, color);
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

            draw_pixel(x, y, color);

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
                draw_pixel(x, y, color);
            }

            break;
        }
    }
}

void surface::draw_rect()
{}

void surface::draw_arc()
{}

void surface::circle_points(int x, int y, int dx, int dy, Uint32 color)
{
    draw_pixel(x+dx, y+dy, color);
    draw_pixel(y+dx, x+dy, color);
    draw_pixel(y+dx, -x+dy, color);
    draw_pixel(x+dx, -y+dy, color);
    draw_pixel(-x+dx, -y+dy, color);
    draw_pixel(-y+dx, -x+dy, color);
    draw_pixel(-y+dx, x+dy, color);
    draw_pixel(-x+dx, y+dy, color);
}

void surface::draw_circle(int x, int y, int radius, Uint32 color)
{
    int xp = 0;
    int yp = radius;
    int d = 1 - radius;
    int delta_e = 3;
    int delta_se = -2*radius+5;
    circle_points(xp, yp, x, y, color);

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
        circle_points(xp, yp, x, y, color);
    }

}

void surface::ellipse_points(int x, int y, int dx, int dy, Uint32 color)
{
    draw_pixel(x+dx, y+dy, color);
    draw_pixel(x+dx, -y+dy, color);
    draw_pixel(-x+dx, -y+dy, color);
    draw_pixel(-x+dx, y+dy, color);
}

void surface::draw_ellipse(int x, int y, int a, int b, Uint32 color)
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
    ellipse_points(xp, yp, x, y, color);

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
        ellipse_points(xp, yp, x, y, color);
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
        ellipse_points(xp, yp, x, y, color);
    }
}

void surface::draw_poly(std::vector<point> points, Uint32 color)
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
                  p_iter_b->x(), p_iter_b->y(), color);
    }
    p_iter_b = points.begin();
    draw_line(p_iter_a->x(), p_iter_a->y(),
              p_iter_b->x(), p_iter_b->y(), color);
}

// antialiased draw routines
void surface::draw_line_aa(int x1, int y1, int x2, int y2, Uint32 color)
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
                draw_pixel(x, y1, color);
            break;
        }
    case RL:
        {
            for (x; x >= x2; x--)
                draw_pixel(x, y1, color);
            break;
        }
    case UP:
        {
            for (y; y <= y2; y++)
                draw_pixel(x1, y, color);
            break;
        }
    case DN:
        {
            for (y; y >= y2; y--)
                draw_pixel(x1, y, color);
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
            draw_pixel_aa(x, y, color, 0);
            draw_pixel_aa(x, y+1, color, two_dx_inv_denom);
            draw_pixel_aa(x, y-1, color, two_dx_inv_denom);

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
                draw_pixel_aa(x, y, color, two_v_dx*inv_denom);
                draw_pixel_aa(x, y+1, color,
                              two_dx_inv_denom - two_v_dx*inv_denom);
                draw_pixel_aa(x, y-1, color,
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
            draw_pixel_aa(x, y, color, 0);
            draw_pixel_aa(x+1, y, color, two_dy_inv_denom);
            draw_pixel_aa(x-1, y, color, two_dy_inv_denom);

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
                draw_pixel_aa(x, y, color, two_v_dy*inv_denom);
                draw_pixel_aa(x+1, y, color,
                              two_dy_inv_denom - two_v_dy*inv_denom);
                draw_pixel_aa(x-1, y, color,
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
            draw_pixel_aa(x, y, color, 0);
            draw_pixel_aa(x, y+1, color, two_dx_inv_denom);
            draw_pixel_aa(x, y-1, color, two_dx_inv_denom);

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
                draw_pixel_aa(x, y, color, two_v_dx*inv_denom);
                draw_pixel_aa(x, y-1, color,
                              two_dx_inv_denom - two_v_dx*inv_denom);
                draw_pixel_aa(x, y+1, color,
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
            draw_pixel_aa(x, y, color, 0);
            draw_pixel_aa(x+1, y, color, two_dy_inv_denom);
            draw_pixel_aa(x-1, y, color, two_dy_inv_denom);

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
                draw_pixel_aa(x, y, color, two_v_dy*inv_denom);
                draw_pixel_aa(x+1, y, color,
                              two_dy_inv_denom - two_v_dy*inv_denom);
                draw_pixel_aa(x-1, y, color,
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
            draw_pixel_aa(x, y, color, 0);
            draw_pixel_aa(x, y+1, color, two_dx_inv_denom);
            draw_pixel_aa(x, y-1, color, two_dx_inv_denom);

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
                draw_pixel_aa(x, y, color, two_v_dx*inv_denom);
                draw_pixel_aa(x, y+1, color,
                              two_dx_inv_denom - two_v_dx*inv_denom);
                draw_pixel_aa(x, y-1, color,
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
            draw_pixel_aa(x, y, color, 0);
            draw_pixel_aa(x+1, y, color, two_dy_inv_denom);
            draw_pixel_aa(x-1, y, color, two_dy_inv_denom);

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
                draw_pixel_aa(x, y, color, two_v_dy*inv_denom);
                draw_pixel_aa(x+1, y, color,
                              two_dy_inv_denom + two_v_dy*inv_denom);
                draw_pixel_aa(x-1, y, color,
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
            draw_pixel_aa(x, y, color, 0);
            draw_pixel_aa(x, y+1, color, two_dx_inv_denom);
            draw_pixel_aa(x, y-1, color, two_dx_inv_denom);

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
                draw_pixel_aa(x, y, color, two_v_dx*inv_denom);
                draw_pixel_aa(x, y+1, color,
                              two_dx_inv_denom + two_v_dx*inv_denom);
                draw_pixel_aa(x, y-1, color,
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
            draw_pixel_aa(x, y, color, 0);
            draw_pixel_aa(x+1, y, color, two_dy_inv_denom);
            draw_pixel_aa(x-1, y, color, two_dy_inv_denom);

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
                draw_pixel_aa(x, y, color, two_v_dy*inv_denom);
                draw_pixel_aa(x+1, y, color,
                              two_dy_inv_denom + two_v_dy*inv_denom);
                draw_pixel_aa(x-1, y, color,
                              two_dy_inv_denom - two_v_dy*inv_denom);
            }
            break;
        }
    }
}

void surface::draw_rect_aa()
{}

void surface::draw_arc_aa()
{}

void surface::draw_ellipse_aa()
{}

void surface::draw_poly_aa(std::vector<point> points, Uint32 color)
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
                     p_iter_b->x(), p_iter_b->y(), color);
    }
    p_iter_b = points.begin();
    draw_line_aa(p_iter_a->x(), p_iter_a->y(),
                 p_iter_b->x(), p_iter_b->y(), color);
}

// non antialiased fill routines
void surface::fill_rect()
{}

void surface::fill_ellipse()
{}

void surface::fill_poly(std::vector<point> points, Uint32 color)
{
    typedef std::list<edge> edge_list;
    // FIXME: this will segfault for poly's outside the surface vertically
    std::vector<edge_list> edges(surface_->h);
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
            draw_line(edge_iter->xbot(), y, edge_iter2->xbot(), y, color);
            edge_iter->step();
            edge_iter2->step();
            edge_iter2++;
        }
    }
}

// antialiased fill routines
void surface::fill_rect_aa()
{}

void surface::fill_ellipse_aa()
{}

void surface::fill_poly_aa(std::vector<point> points, Uint32 color)
{
    fill_poly(points, color);
    draw_poly_aa(points, color);
}

// utility methods
Uint32 surface::gen_color(const std::string &str_color)
{
    Uint8 r, g, b, a;
    Uint32 int_color = strtoll(str_color.c_str(), NULL, 16);
    Uint32 mapped_color;
    r = (int_color & RMASK) >> RSHIFT;
    g = (int_color & GMASK) >> GSHIFT;
    b = (int_color & BMASK) >> BSHIFT;
    a = (int_color & AMASK) >> ASHIFT;
    return gen_color(r, g, b, a);
}

Uint32 surface::gen_color(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    return SDL_MapRGBA(surface_->format, r, g, b, a);
}

void surface::blit(surface &screen, SDL_Rect *src_rect = NULL,
                   SDL_Rect *dst_rect = NULL)
{
    // blit the local surface to the parent surface (screen)
    if (surface_ && SDL_BlitSurface(surface_, src_rect,
                                    screen.sdl_surface(), dst_rect) < 0)
    {
        cerr << "widget: Failed to blit surface_ to screen" << endl;
    }
}

void surface::lock()
{
    if (SDL_MUSTLOCK(surface_))
    {
        if (SDL_LockSurface(surface_) < 0)
        {
            return;     // wtf ?
        }
    }
}

void surface::unlock()
{
    if (SDL_MUSTLOCK(surface_))
    {
        SDL_UnlockSurface(surface_);
    }
}
