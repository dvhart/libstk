/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Image class implementation.
 *     AUTHORS: Darren Hart, Marc Strämkei, Chris Slade
 *  START DATE: 2003/Mar/03
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include "libstk/config.h"
# include <stdio.h>

#ifdef HAVE_LIBPNG
# include <png.h>
# include <unistd.h>
#endif

#ifdef HAVE_LIBJPEG
#include <jpeglib.h>
#include <setjmp.h>
#endif

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "libstk/image.h"
#include "libstk/logging.h"

namespace stk
{
#ifdef HAVE_LIBJPEG    
    //handle JPEG ERRORS
    struct my_error_mgr {
        struct jpeg_error_mgr pub;	/* "public" fields */
        jmp_buf setjmp_buffer;	/* for return to caller */
    };

    typedef struct my_error_mgr * my_error_ptr;

    /*
    * Here's the routine that will replace the standard error_exit method:
    */

    METHODDEF(void)
    my_error_exit (j_common_ptr cinfo)
    {
        /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
        my_error_ptr myerr = (my_error_ptr) cinfo->err;

        /* Always display the message. */
        /* We could postpone this until after returning, if we chose. */
        //(*cinfo->err->output_message) (cinfo);

        /* Return control to the setjmp point */
        longjmp(myerr->setjmp_buffer, 1);
    }
#endif
    
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
        INFO("image::image(" << filename << ")");
        // switch on extension until we remove ppm support
        if (filename.find(".ppm") != std::string::npos)
            load_ppmx(filename);
        else if (filename.find(".jpg") != std::string::npos || filename.find(".jpeg") != std::string::npos)
            load_jpeg(filename);
        else
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
            throw image_file_exception("image::load_png - could not open file: " + filename);
        char header[8];
        fread(header,1,8,fp);
        bool is_png= !png_sig_cmp((png_byte*)header,0,8);
        if (!is_png)
            throw image_read_exception("image::load_png - File is not in PNG format: " + filename);

        png_structp png_ptr = png_create_read_struct ( PNG_LIBPNG_VER_STRING, (png_voidp)NULL, NULL, NULL);
        if(!png_ptr)
            throw image_read_exception("image::load_png - Couldnt create PNG read structure");
        
        png_infop info_ptr = png_create_info_struct(png_ptr);
        if(!info_ptr)
            throw image_read_exception("image::load_png - Couldnt create PNG info struct 1");

        png_infop end_info = png_create_info_struct(png_ptr);
        if(!end_info)
            throw image_read_exception("image::load_png - Couldnt create PNG info struct 2");

        if(setjmp(png_jmpbuf(png_ptr)))
        {
            png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
            fclose(fp);
            throw image_read_exception("image::load_png - PNG error");
        }

        png_init_io(png_ptr,fp);
        png_set_sig_bytes(png_ptr,8);
        
        png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND,NULL);

        unsigned long width,height,bit_depth;

        width=png_get_image_width(png_ptr,info_ptr);
        height=png_get_image_height(png_ptr,info_ptr);
        bit_depth=png_get_bit_depth(png_ptr,info_ptr);
        
        INFO("Read PNG: width " << width << "  height " << height << "  depth " << bit_depth );

        int bytes_per_pixel = png_get_channels(png_ptr,info_ptr);
        
        INFO("PNG file has " << bytes_per_pixel << " Bytes per Pixel");

        png_bytep* row_pointers = png_get_rows(png_ptr, info_ptr);

        offscreen_surface = onscreen_surface->create_surface(rectangle(0, 0, width, height));
        
        offscreen_surface->clip_rect(rectangle(0, 0, width, height));
        for (unsigned int y = 0; y < height; y++)
        {
            for (unsigned int x = 0; x < width; x++)
            {
                char r = row_pointers[y][(x*bytes_per_pixel)+0];
                char g = row_pointers[y][(x*bytes_per_pixel)+1];
                char b = row_pointers[y][(x*bytes_per_pixel)+2];
                char a = 255; // FIXME we need acces to stk::opaque here
                if (bytes_per_pixel > 3) a = row_pointers[y][(x*bytes_per_pixel)+3];
                color pixel_color = offscreen_surface->gen_color(r, g, b, a);
                offscreen_surface->draw_pixel(x, y, pixel_color);
            }
        }
        
//        png_read_end(png_ptr, end_info);

        INFO("Destroying png read struct");
        png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
#else
        ERROR("No support for loading PNG files compiled in!");
#endif
    }
    
    void image::load_ppmx(const std::string filename)
    {
        INFO("image::load_ppmx() - no comment support");

        std::ifstream infile(filename.c_str(), std::ios::in);
        if (!infile)
            throw image_file_exception("image::load_ppmx - could not open file: " + filename);

        // get the magic number
        unsigned char magic_a, magic_b;
        infile >> magic_a;
        infile >> magic_b;
        unsigned short magic = (magic_a << 8) | magic_b;
        if (magic != 0x5036)
            throw image_read_exception("image::load_ppmx() - file is not a ppmx image");

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

    void image::load_jpeg(const std::string filename)
    {
        INFO("Loading JPEG Image: "<< filename);
#ifdef HAVE_LIBJPEG
        //create decompression struct and error struct
        struct jpeg_decompress_struct cinfo;
        struct my_error_mgr jerr;

        FILE * infile;		/* source file */
        JSAMPARRAY buffer;	/* Output row buffer */

        //open file image file
        if ((infile = std::fopen(filename.c_str(), "rb")) == NULL)
            throw image_file_exception("image::load_jpeg() - could not open file " + filename);
        
        //set error handling routine
        cinfo.err = jpeg_std_error(&jerr.pub);
        jerr.pub.error_exit = my_error_exit;
        /* Establish the setjmp return context for my_error_exit to use. */
        if (setjmp(jerr.setjmp_buffer)) {
            /* If we get here, the JPEG code has signaled an error.
            * We need to clean up the JPEG object, close the input file, and return.
            */
            char error_msg[200];
            (*cinfo.err->format_message) ((jpeg_common_struct*)&cinfo,error_msg);
            jpeg_destroy_decompress(&cinfo);
            fclose(infile);


            throw image_read_exception("image::load_jpeg() " + std::string(error_msg));
            return;
        }
        //intialize JPEG Compression object
        jpeg_create_decompress(&cinfo);
        //specify data source
        jpeg_stdio_src(&cinfo, infile);
        //read the jpeg header
        jpeg_read_header(&cinfo, TRUE);
        INFO("Color Components: " << cinfo.out_color_components);
        cinfo.out_color_space = JCS_RGB;
        //start decompression
        jpeg_start_decompress(&cinfo);
        // JSAMPLEs per row in output buffer (row width)
        int row_stride = cinfo.output_width * cinfo.output_components;
        // Make a one-row-high sample array that will go away when done with image
        buffer = (*cinfo.mem->alloc_sarray)
		((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
        
        offscreen_surface = onscreen_surface->create_surface(rectangle(0,0,cinfo.output_width,cinfo.output_height));
        offscreen_surface->clip_rect(rectangle(0, 0, cinfo.output_width, cinfo.output_height));
        //read image
        int y = 0;
        while (cinfo.output_scanline < cinfo.output_height) {
            //read one line at a time
            jpeg_read_scanlines(&cinfo, buffer, 1);
            // Assume put_scanline_someplace wants a pointer and sample count.
            // Copy into surface
            for (int i=0; i<row_stride/3; i++) {
                stk::color pixel_color = offscreen_surface->gen_color((unsigned char)buffer[0][i*3],(unsigned char)buffer[0][i*3+1],(unsigned char)buffer[0][i*3+2],255);
                offscreen_surface->draw_pixel(i,y,pixel_color);
            }
            y++;
        }
        //clean up
        jpeg_finish_decompress(&cinfo);
        jpeg_destroy_decompress(&cinfo);
        fclose(infile);
#else
     ERROR("No support for loading JPEG files compiled in!");
#endif
    }
}
