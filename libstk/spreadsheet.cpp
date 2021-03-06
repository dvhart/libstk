#include "spreadsheet.h"
#include <libstk/key_event.h>

namespace stk
{
    spreadsheet::ptr spreadsheet::create(container::ptr parent, const rectangle& rect)
    {
        spreadsheet::ptr new_spreadsheet(new spreadsheet(rect));
        new_spreadsheet->parent(parent);
        return new_spreadsheet;
    }
 
    spreadsheet::spreadsheet(const stk::rectangle& rect): widget(rect)
    {
    }
    spreadsheet::row_iterator spreadsheet::focused_row()
    {
        bool found_it = false;
        column_iterator iter;
        int row=0;
        for(iter=columns_.begin();
            iter!=columns_.end(); iter++)
        {
            row = 0;
            for(column_info::Tcells::iterator cell = iter->cells.begin();
                cell!=iter->cells.end(); cell++)
            {
                if(*cell == focused_cell_)
                {
                    found_it = true;
                    break;
                }
                row++;
            }
            if(found_it)
                break;
        }
        return rows_.begin()+row;
    }
    spreadsheet::column_iterator spreadsheet::focused_column()
    {
        bool found_it = false;
        spreadsheet::column_iterator iter;
        int row;
        for(iter=columns_.begin();
            iter!=columns_.end(); iter++)
        {
            row = 0;
            for(column_info::Tcells::iterator cell = iter->cells.begin();
                cell!=iter->cells.end(); cell++)
            {
                if(*cell == focused_cell_)
                {
                    found_it = true;
                    break;
                }
                row++;
            }
            if(found_it)
                break;
        }
        return iter;
    }
    spreadsheet_cell::ptr spreadsheet::focused_cell()
    {
        return focused_cell_;
    }
    
    void spreadsheet::draw(surface::ptr surface, const rectangle& clip_rect)
    {
        surface->clip_rect(clip_rect.empty() ? rect() : clip_rect);
        int current_y_pos = 0;
        for(column_iterator column=columns_begin();column!=columns_end();column++)
        {
            int current_x_pos = 0;
            column_info::Tcells::iterator cell = column->cells.begin();
            for(row_iterator row = rows_begin(); row != rows_end(); row++)
            {
                if(/*!column->cells.empty() && */ cell != column->cells.end())
                {
                    (*cell)->draw(surface,rectangle(current_x_pos+rect_.x1(),
                                                    current_y_pos+rect_.y1(),
                                                    row->width, column->height));
                    cell++;
                }
                current_x_pos += row->width;
            }
            current_y_pos += column->height;
        }
    }

    void spreadsheet::handle_event(stk::event::ptr e)
    {
        if(!e)
        {
            ERROR("Invalid event received in spreadsheet");
        }
        
        switch(e->type())
        {
        case event::mouse_down:
        {
            mouse_event::ptr me = boost::dynamic_pointer_cast<mouse_event>(e);
            if (!me)
                ERROR("Invalid event received in spreadsheet");        
            int xpos = rect_.x1();
            row_iterator row;
            for (row = rows_begin(); row != rows_end(); row++)
            {
                if (me->x()>xpos && me->x() < (xpos+row->width))
                    break;
                xpos += row->width;
            }
            int ypos = rect_.y1();
            column_iterator col;
            for (col = columns_begin(); col != columns_end(); col++)
            {
                if (me->y()>ypos && me->y() < (ypos+col->height))
                    break;
                ypos += col->height;
            }
            if (focused_cell_)
            {
                focused_cell_->handle_event(event::create(event::un_focus));
                focused_cell_.reset();
            }
            if (col!=columns_end() && row != rows_end())
            {
                int index = row-rows_begin();
                
                column_info::Tcells::iterator cell=col->cells.begin()+index;
             
                if (cell != col->cells.end())
                {
                    focused_cell_ =* cell;
                    focused_cell_->handle_event(event::create(event::focus));
                }
            }
            
            redraw(rect());
            
            return;
            break;
        }
        default:
            if (focused_cell_)
                focused_cell_->handle_event(e);
            return;
        }
        widget::handle_event(e);
    }
    
    spreadsheet_cell::spreadsheet_cell(spreadsheet::ptr parent) : parent_(parent) , focused_(false)
    {
    }
    void spreadsheet_cell::background_color(const boost::optional<stk::color> new_color)
    {
        background_color_=new_color;
    }
    
    boost::optional<stk::color> spreadsheet_cell::background_color() const
    {
        return background_color_;
    }
    

    void spreadsheet_cell::handle_event(stk::event::ptr e)
    {
        switch(e->type())
        {
        case event::focus:
            focused_ = true;
            if(parent_.lock())
                parent_.lock()->redraw(parent_.lock()->rect());
            break;
        case event::un_focus:
            focused_ = false;
            if(parent_.lock())
                parent_.lock()->redraw(parent_.lock()->rect());
            break;
        }
    }
                                        
        
//**************************************************************************************************
//                          row and column stl like interface!
//**************************************************************************************************
    spreadsheet::row_iterator spreadsheet::rows_begin()
    {
        return rows_.begin();
        redraw(rect());
    }
    spreadsheet::row_iterator spreadsheet::rows_end()
    {
        return rows_.end();
        redraw(rect());
    }
    void spreadsheet::rows_erase(spreadsheet::row_iterator row)
    {
        rows_.erase(row);
        redraw(rect());
    }
    void spreadsheet::rows_push_back(row_info row)
    {
        rows_.push_back(row);
        redraw(rect());
    }
    void spreadsheet::rows_insert(spreadsheet::row_iterator before, row_info row)
    {
        rows_.insert(before, row);
        redraw(rect());
    }
    void spreadsheet::rows_clear()
    {
        rows_.clear();
        redraw(rect());
    }
    spreadsheet::column_iterator spreadsheet::columns_begin()
    {
        return columns_.begin();
        redraw(rect());
    }
    spreadsheet::column_iterator spreadsheet::columns_end()
    {
        return columns_.end();
        redraw(rect());
    }
    void spreadsheet::columns_erase(spreadsheet::column_iterator column)
    {
        columns_.erase(column);
        redraw(rect());
    }
    void spreadsheet::columns_push_back(column_info column)
    {
        columns_.push_back(column);
        redraw(rect());
    }
    void spreadsheet::columns_insert(spreadsheet::column_iterator before, column_info column)
    {
        columns_.insert(before, column);
        redraw(rect());
    }
    void spreadsheet::columns_clear()
    {
        columns_.clear();
        redraw(rect());
    }
    spreadsheet_cell_text::spreadsheet_cell_text(spreadsheet::ptr parent) : spreadsheet_cell(parent) 
    {
    }

    spreadsheet_cell_text::~spreadsheet_cell_text()
    {
    }

    spreadsheet_cell_text::ptr spreadsheet_cell_text::create(spreadsheet::ptr parent)
    {
        return ptr(new spreadsheet_cell_text(parent));
    }
    void spreadsheet_cell_text::handle_event(event::ptr e)
    {
        switch (e->type())
        {
        case event::key_up:
        {
            key_event::ptr ke = boost::dynamic_pointer_cast<key_event>(e);
            switch ( ke->fn_key() )
            {
            case key_backspace:
                INFO("Backspace hit in ssheet_cell_text");
                if (text_.size() > 0)
                    text_.resize(text_.size()-1);
                if (parent_.lock())
                    parent_.lock()->redraw(parent_.lock()->rect());
                return;
            }
            
            if ((ke->fn_key() != key_backspace) && (ke->character() != 0))
            {
                text_+=ke->character();
                if (parent_.lock())
                    parent_.lock()->redraw(parent_.lock()->rect());
                return;
            }
        }
        }
        
        spreadsheet_cell::handle_event(e);
    }
    
    void spreadsheet_cell_text::text(std::wstring text)
    {
        text_ = text;
        if(parent_.lock())
            parent_.lock()->redraw(parent_.lock()->rect());
    }
    std::wstring spreadsheet_cell_text::text() const
    {
        return text_;
    }

}

