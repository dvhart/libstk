#include <iostream>
#include <sstream>
#include <fstream>
#include "image.h"

using std::cout;
using std::endl;

namespace stk
{
	image::ptr image::create(const rectangle& rect)	
	{
		image::ptr new_image(new image(rect));
		return new_image;
	}
	
	image::ptr image::create(const std::string& filename)
	{
		image::ptr new_image(new image(filename));
		return new_image;
	}
		
	image::image(const rectangle& rect)
	{
		cout << "image::image()" << endl;

		cout << "\tcreating empty image in: " << rect << endl;
		width_ = rect.width();
		height_ = rect.height();
		init_pixels();
	}
	
	image::image(const std::string& filename)
	{
		cout << "image::image()" << endl;
		load_ppmx(filename);
	}
	
	image::~image()
	{
		for (int x = 0; x < width_; x++)
		{
			delete[] pixels_[x];
		}
		delete[] pixels_;
	}
	
	void image::load_ppmx(const std::string& filename)
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

		infile >> width_;
		infile >> height_;
		int max; infile >> max; // must be <= 255
		if (max != 255) throw std::string("image::load_ppmx - max value != 255");
		// FIXME: what is the best way to get to the start of the hex data ? (the next line)
		infile.ignore(80, '\n'); 
		
		init_pixels();
		
		for (int y = 0; y < height_; y++)
		{
			for (int x = 0; x < width_; x++)
			{
				unsigned char red, green, blue;
				infile.read(reinterpret_cast<char *>(&red), 1);
				infile.read(reinterpret_cast<char *>(&green), 1);
				infile.read(reinterpret_cast<char *>(&blue), 1);
				
				// scale colors to a max of 255
				red =   (unsigned char)((255.0/(double)max)*red);
				green = (unsigned char)((255.0/(double)max)*green);
				blue =  (unsigned char)((255.0/(double)max)*blue);
				
				pixels_[x][y] = ((red << 24) | (green << 16) | (blue << 8));
				pixels_[x][y] |= 0xFF; // set opaque
			}
		}
	}
			
	color image::pixel(int x, int y) const
	{
		return pixels_[x][y];
	}
	
	void image::pixel(int x, int y, color new_color)
	{
	}

	void image::init_pixels()
	{
		pixels_ = new color*[width_];
		for (int x = 0; x < width_; x++)
		{
			pixels_[x] = new color[height_](0xFF);
			memset(pixels_[x], 0, height_);
		}
	}
}
