/******************************************************************************
 *    FILENAME: spreadsheet.h
 * DESCRIPTION: A spreadsheet widget, contains an matrix of cells with different
 *              and output characteristics
 *     AUTHORS: Marc Straemke
 *  START DATE: 13/Jul/2003  LAST UPDATE: 13/Jul/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available
 *              with the source as license.txt or at 
 *              http://www.libstk.org/index.php?page=docs/license
 *****************************************************************************/

#ifndef SPREADHSHEET_H
#define SPREADHSHEET_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/optional.hpp>
#include <libstk/widget.h>
#include <libstk/container.h>
#include <libstk/scroll_model.h>
#include <libstk/rectangle.h>
#include <libstk/event_handler.h>
#include <libstk/event.h>


namespace stk
{
    class spreadsheet;
    class spreadsheet_cell : public event_handler
    {
    public:
        typedef boost::shared_ptr<spreadsheet_cell> ptr;
        typedef boost::weak_ptr<spreadsheet_cell> weak_ptr;

    protected:
        boost::weak_ptr<spreadsheet> parent_;
        spreadsheet_cell(boost::shared_ptr<spreadsheet> parent);
        bool focused_;

        boost::optional<stk::color> background_color_;
    public:
        virtual ~spreadsheet_cell() {};
        
        /********** EVENT HANDLER INTERFACE **********/
        virtual void handle_event(event::ptr e);
        /********** END EVENT HANDLER INTERFACE **********/

        /// Set the property #background_color_
        virtual void background_color(const boost::optional<stk::color> new_color);
        /// Get the property #background_color_
        virtual boost::optional<stk::color> background_color() const;

        virtual void draw(surface::ptr surface, const rectangle& screen_position);
    };

    class spreadsheet : public widget
    {
    public:
        typedef boost::shared_ptr<spreadsheet> ptr;
        typedef boost::weak_ptr<spreadsheet> weak_ptr;

        struct row_info
        {
            int width;
        };
        struct column_info
        {
            int height;
            typedef std::vector<spreadsheet_cell::ptr> Tcells;
            Tcells cells;
        };

    private:
        typedef std::vector<row_info> Trows;
        typedef std::vector<column_info> Tcolumns;
        Trows rows_;            // Stores the row descriptions
        Tcolumns columns_;      // Stores the column descriptions
        spreadsheet(const rectangle& rect);

        scroll_model::ptr v_scroll_;
        boost::signals::connection v_scroll_con;
        scroll_model::ptr h_scroll_;
        boost::signals::connection h_scroll_con;
        
        stk::spreadsheet_cell::ptr focused_cell_;

    public:

        // Row stl style interface
        typedef Trows::iterator row_iterator;
        typedef Trows::const_iterator const_row_iterator;
        row_iterator rows_begin();
        row_iterator rows_end();
        void rows_erase(row_iterator row);
        void rows_insert(row_iterator before, row_info row);
        void rows_push_back(row_info row);
        void rows_clear();
        
        // Column stl style interface
        typedef Tcolumns::iterator column_iterator;
        typedef Tcolumns::const_iterator const_column_iterator;
        column_iterator columns_begin();
        column_iterator columns_end();
        void columns_erase(column_iterator column);
        void columns_insert(column_iterator before, column_info column);
        void columns_push_back(column_info column);
        void columns_clear();


        spreadsheet::row_iterator focused_row();
        spreadsheet::column_iterator focused_column();
        spreadsheet_cell::ptr focused_cell();


        /// Creates a new spreadsheet and registers it with parent
        static spreadsheet::ptr create(container::ptr parent, const rectangle& rect);

        /********** EVENT HANDLER INTERFACE **********/
        virtual void handle_event(stk::event::ptr e);
        /********** END EVENT HANDLER INTERFACE **********/

        /********** DRAWABLE INTERFACE **********/
        virtual void draw(surface::ptr surface, const rectangle& clip_rect = rectangle());
        /********** END DRAWABLE INTERFACE **********/
    };



    class spreadsheet_cell_text : public spreadsheet_cell
    {
    public:
        typedef boost::shared_ptr<spreadsheet_cell_text> ptr;
        typedef boost::weak_ptr<spreadsheet_cell_text> weak_ptr;
    protected:
        spreadsheet_cell_text(spreadsheet::ptr parent);
        std::wstring text_;
    public:
        static spreadsheet_cell_text::ptr create(spreadsheet::ptr parent);        
        ~spreadsheet_cell_text();
        virtual void draw(surface::ptr surface, const rectangle& screen_position);
        virtual void handle_event(event::ptr e);
        
        std::wstring text() const;
        void text(std::wstring text);
    };
}


#endif
