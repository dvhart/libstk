/******************************************************************************
 *    FILENAME: image.cpp
 * DESCRIPTION: Image class implementation.
 *     AUTHORS: Darren Hart, Marc Straemke
 *  START DATE: 03/Mar/2003  LAST UPDATE: 14/May/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available
 *              with the source as license.txt or at 
 *              http://www.libstk.org/index.php?page=docs/license
 *****************************************************************************/

#include "libstk/config.h"

#ifdef HAVE_LIBPNG
# include <png.h>
# include <stdio.h>
# include <unistd.h>
#endif

#include <iostream>
#include <sstream>
#include <fstream>
#include "libstk/image.h"
#include "libstk/logging.h"

using std::cout;
using std::endl;

namespace stk
{
    image::ptr image::create(stk::surface::ptr onscreen_surface,const rectangle& rect)
    {
        image::ptr new_image(new image(onscreen_surface,rect));
        return new_image;
    }

    image::ptr image::create(stk::surface::ptr onscreen_surface,const std::string& filename)
    {
        image::ptr new_image(new image(onscreen_surface,filename));
        return new_image;
    }

    image::image(stk::surface::ptr onscreen_surface,const rectangle& rect) : onscreen_surface(onscreen_surface)
    {
        INFO("image::image()");
        offscreen_surface=onscreen_surface->create_surface(rect);
    }

    image::image(stk::surface::ptr onscreen_surface,const std::string& filename) : onscreen_surface(onscreen_surface)
    {
        INFO("image::image(filename)");
        //load_ppmx(filename);
        load_png(filename);
    }
    

    image::~image()
    {
    }
    
    void image::load_png(const std::string filename)
    {
#ifdef HAVE_LIBPNG
        FILE *fp = fopen(filename.c_str(), "rb");
        if (!fp)
            throw error_message_exception("image::load_png - could not open file: " + filename);
        char header[8];
        fread(header,1,8,fp);
        bool is_png= !png_sig_cmp((png_byte*)header,0,8);
        if (!is_png)
            throw error_message_exception("image::load_png - File is not in PNG format: " + filename);

        png_structp png_ptr = png_create_read_struct ( PNG_LIBPNG_VER_STRING, (png_voidp)NULL, NULL, NULL);
        if(!png_ptr)
            throw error_message_exception("image::load_png - Couldnt create PNG read structure");
        
        png_infop info_ptr = png_create_info_struct(png_ptr);
        if(!info_ptr)
            throw error_message_exception("image::load_png - Couldnt create PNG info struct 1");

        png_infop end_info = png_create_info_struct(png_ptr);
        if(!end_info)
            throw error_message_exception("image::load_png - Couldnt create PNG info struct 2");

        if(setjmp(png_jmpbuf(png_ptr)))
        {
            png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
            fclose(fp);
            throw error_message_exception("image::load_png - PNG error");
        }

        png_init_io(png_ptr,fp);
        png_set_sig_bytes(png_ptr,8);
        
        png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND,NULL);

        unsigned long width,height,bit_depth;

        width=png_get_image_width(png_ptr,info_ptr);
        height=png_get_image_height(png_ptr,info_ptr);
        bit_depth=png_get_bit_depth(png_ptr,info_ptr);
        
        INFO("Read PNG: width " << width << "  height " << height << "  depth " << bit_depth );

        int bytes_per_pixel=png_get_channels(png_ptr,info_ptr);
        
        INFO("PNG file has " << bytes_per_pixel << " Bytes per Pixel");


        png_bytep* row_pointers=png_get_rows(png_ptr, info_ptr);


        offscreen_surface = onscreen_surface->create_surface(rectangle(0,0,width,height));
        
        for(int y=0;y<height;y++)
            for(int x=0;x<width;x++)
            {
                char r=row_pointers[y][(x*bytes_per_pixel)+0];
                char g=row_pointers[y][(x*bytes_per_pixel)+1];
                char b=row_pointers[y][(x*bytes_per_pixel)+2];
                char a=255;
                if(bytes_per_pixel>3)
                    a=row_pointers[y][(x*bytes_per_pixel)+3];
                stk::color pixel_color=offscreen_surface->gen_color(r,g,b,a);
                offscreen_surface->draw_pixel(x,y,pixel_color);
            }
                                                             
        
        
//        png_read_end(png_ptr, end_info);

        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
#else
        ERROR("No support for loading PNG files compiled in!");
#endif
    }
    
    void image::load_ppmx(const std::string filename)
    {
        cout << "image::load_ppmx() - no comment support" << endl;

        std::ifstream infile(filename.c_str(), std::ios::in);
        if (!infile)
            throw error_message_exception("image::load_ppmx - could not open file: " + filename);

        // get the magic number
        unsigned char magic_a, magic_b;
        infile >> magic_a;
        infile >> magic_b;
        unsigned short magic = (magic_a << 8) | magic_b;
        if (magic != 0x5036)
            throw error_message_exception("image::load_ppmx() - file is not a ppmx image");

        int width,height;
        infile >> width ;
        infile >> height;
        int max;
        infile >> max; // must be <= 255
        if (max != 255)
            throw std::string("image::load_ppmx - max value != 255");
        // FIXME: what is the best way to get to the start of the hex data ? (the next line)
        infile.ignore(80, '\n');

        offscreen_surface=onscreen_surface->create_surface(rectangle(0,0,width,height));

        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                unsigned char red, green, blue;
                infile.read(reinterpret_cast<char *>(&red), 1);
                infile.read(reinterpret_cast<char *>(&green), 1);
                infile.read(reinterpret_cast<char *>(&blue), 1);

                // scale colors to a max of 255
                red =   (unsigned char)((255.0/(double)max)*red);
                green = (unsigned char)((255.0/(double)max)*green);
                blue =  (unsigned char)((255.0/(double)max)*blue);

                stk::color pixel_color = offscreen_surface->gen_color(red,green,blue,255);
                offscreen_surface->draw_pixel(x,y,pixel_color);
            }
        }
    }
}
