/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: A generic image class.
 *     AUTHORS: Darren Hart, Marc Strämke, Chris Slade
 *  START DATE: 2003/Mar/03
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_IMAGE_H
#define STK_IMAGE_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <libstk/rectangle.h>
#include <libstk/surface.h>
#include <libstk/exceptions.h>



namespace stk
{
    
   
    class image_exception: exception
    {
    private:
        std::string message_;
        /*image_exception()
        { } // what is this for ?*/
    public:
        image_exception()
        {}
        image_exception(const std::string& message)
        {
            message_ = message;
        }
        virtual ~image_exception()
        { }
        virtual std::string what() const
        {
            return message_;
        }
        void message(const std::string& message)
        {
            message_=message;
        }
    };

    class image_file_exception : image_exception
    {
    private:
        std::string message_;
        image_file_exception()
        { } // what is this for ?
    public:
        image_file_exception(const std::string& message)
        {
            message_ = message;
        }
        virtual ~image_file_exception()
        { }
        virtual std::string what() const
        {
            return message_;
        }
        void message(const std::string& message)
        {
            message_=message;
        }
    };

    class image_read_exception : image_exception
    {
    private:
        std::string message_;
        image_read_exception()
        { } // what is this for ?
    public:
        image_read_exception(const std::string& message)
        {
            message_ = message;
        }
        virtual ~image_read_exception()
        { }
        virtual std::string what() const
        {
            return message_;
        }
        void message(const std::string& message)
        {
            message_=message;
        }
    };

    class image_write_exception : image_exception
    {
    private:
        std::string message_;
        image_write_exception()
        { } // what is this for ?
    public:
        image_write_exception(const std::string& message)
        {
            message_ = message;
        }
        virtual ~image_write_exception()
        { }
        virtual std::string what() const
        {
            return message_;
        }
        void message(const std::string& message)
        {
            message_=message;
        }
    };
        
    
    class image
    {
    public:
        typedef boost::shared_ptr<image> ptr;
        typedef boost::weak_ptr<image> weak_ptr;
    private:
    protected:
        image(stk::surface::ptr onscreen_surface, const std::string& filename = "");
        image(stk::surface::ptr onscreen_surface, const rectangle& rect);
        void resize(image::ptr new_img, double x_factor, double y_factor);
        color data_protect(int x, int y);//for the scalling
        void init_pixels();
        int width_;
        int height_;
        surface::ptr onscreen_surface; /// Used for offscreen surface construction
    public:
        static image::ptr create(stk::surface::ptr onscreen_surface, 
                const std::string& filename = "");
        static image::ptr create(stk::surface::ptr onscreen_surface, const rectangle& _rect);
        virtual ~image();

        surface::ptr offscreen_surface;
        /********** IMAGE INTERFACE **********/
        void load_ppmx(const std::string filename);
        void load_png(const std::string filename);
        void load_jpeg(const std::string filename);

        void write(const std::string filename);
        void write_png(const std::string filename);
        void write_jpeg(const std::string filename);

        image::ptr scale(int x_size, int y_size, bool maintain_aspec = false);
        
        //color pixel(int x, int y) const; // \FIXME Needed anymore if we are using offscreen surfaces?
        //void pixel(int x, int y, color new_color); // \FIXME Needed anymore if we are using offscreen surfaces?
        //int width() // \FIXME Needed anymore if we are using offscreen surfaces?
        //{
        //    return width_;
        //}
        //int height() // \FIXME Needed anymore if we are using offscreen surfaces?
        //{
        //    return height_;
        //}
        /********** END IMAGE INTERFACE **********/
    };
}

#endif
