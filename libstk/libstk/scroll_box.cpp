/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Scroll Box container implementation.
 *     AUTHORS: Darren Hart
 *  START DATE: 2003/May/22
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Strämke, Dirk Hörner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#include <boost/bind.hpp>
#include "libstk/scroll_box.h"
#include "libstk/event.h"
#include "libstk/key_event.h"

namespace stk
{
    scroll_box::ptr scroll_box::create(container::ptr parent, const rectangle& rect, 
            scroll_policy v_policy, scroll_policy h_policy)
    {
        scroll_box::ptr new_scroll_box(new scroll_box(rect));
        new_scroll_box->parent(parent);
        new_scroll_box->scroll_policies(v_policy, h_policy);
        return new_scroll_box;
    }

    scroll_box::scroll_box(const rectangle& rect) : container(rect),
            check_scrollable_(true), v_policy_(never), h_policy_(never)
    {
        INFO("constructor");
        focusable_ = true; 
        child_rect_ = rect; child_rect_.position(point(0, 0));
    }

    scroll_box::~scroll_box()
    {
        INFO("destructor");
    }

    void scroll_box::update_model_cons()
    {
        INFO("update_model_cons()");
        v_scroll_con_.disconnect();
        v_scroll_con_ = v_scroll()->on_change.connect(boost::bind(&scroll_box::model_change, this));
        //v_scroll_con_ = v_scroll()->on_change.connect(boost::bind(&scroll_box::scroll_policies, this, v_policy_, h_policy_));
        h_scroll_con_.disconnect();
        h_scroll_con_ = h_scroll()->on_change.connect(boost::bind(&scroll_box::model_change, this));
        //h_scroll_con_ = h_scroll()->on_change.connect(boost::bind(&scroll_box::scroll_policies, this, v_policy_, h_policy_));
    }

    void scroll_box::model_change()
    {
        INFO("model_changed()");
        scroll_policies(v_policy_, h_policy_);
    }

    void scroll_box::h_scroll(scroll_model::ptr model)
    { 
        if (child_) boost::shared_dynamic_cast<scrollable>(child_)->h_scroll(model);
        if (h_scroll_bar_) h_scroll_bar_->model(model);
        scroll_policies(v_policy_, h_policy_);
        update_model_cons();
    }

    void scroll_box::v_scroll(scroll_model::ptr model)
    {
        if (child_) boost::shared_dynamic_cast<scrollable>(child_)->v_scroll(model);
        if (v_scroll_bar_) v_scroll_bar_->model(model);
        scroll_policies(v_policy_, h_policy_);
        update_model_cons();
    }

    void scroll_box::handle_event(event::ptr e)
    {
        widget::handle_event(e);
    }

    void scroll_box::add(widget::ptr item)
    {
        INFO("Adding something to scroll_box");        
        if (check_scrollable_)
        { // we are adding the child widget
            INFO("Adding child widget to scroll_box");        
            scrollable::ptr scroll_item;
            if (scroll_item = boost::shared_dynamic_cast<scrollable>(item))
            { // item is scrollable, add it directly
                if (viewport_)
                {
                    remove(viewport_);
                    viewport_.reset();
                }
                item->rect(child_rect_);
                container::add(item); 
                child_ = item;
            }
            else
            { // item isn't scrollable, put it in a viewport first
                if (item->parent()) item->parent()->remove(item);
                if (!viewport_)
                {
                    check_scrollable_ = false;
                    // FIXME: ugly cast, but I don't see a clean way...
                    scroll_box::ptr this_ptr = boost::shared_static_cast<scroll_box>(shared_from_this());
                    INFO("creating viewport");
                    viewport_ = viewport::create(this_ptr, child_rect_);
                    INFO("assigning child as viewport");
                    check_scrollable_ = true;
                }
                INFO("setting child_ to viewport_");
                child_ = viewport_;
                INFO("setting item parent to viewport_");
                item->parent(viewport_);
            }

            // update the scroll bars to use the new models
            scroll_policies(v_policy_, h_policy_);
            update_model_cons();
        }
        else
        { // not checking for scrollable, so we adding scrollbars (not child_)
            container::add(item); 
        }
    }

    scroll_model::ptr scroll_box::h_scroll()
    {
        if (child_)
            return boost::shared_dynamic_cast<scrollable>(child_)->h_scroll();
        else
            return scroll_model::ptr();
    }

    scroll_model::ptr scroll_box::v_scroll()
    {
        if (child_)
            return boost::shared_dynamic_cast<scrollable>(child_)->v_scroll();
        else
            return scroll_model::ptr();
    }
    
    void scroll_box::scroll_policies(scroll_policy v_policy, scroll_policy h_policy)
    {
        h_policy_ = h_policy;
        v_policy_ = v_policy;

        // determine which bars we need
        bool hbar = (h_policy == always) || ((h_policy == automatic) && 
                (h_scroll() && h_scroll()->vis_size() < h_scroll()->size()));
        bool vbar = (v_policy == always) || ((v_policy == automatic) && 
                (v_scroll() && v_scroll()->vis_size() < v_scroll()->size()));
        
        // Scrollbar configuration Changed
        if ((hbar != (bool)h_scroll_bar_) ||
            (vbar != (bool)v_scroll_bar_) )
        {
            
            // resize the child rect 
            child_rect_ = rect(); child_rect_.position(point(0, 0));
            if (hbar) child_rect_.y2(child_rect_.y2() - scroll_bar::default_size);
            if (vbar) child_rect_.x2(child_rect_.x2() - scroll_bar::default_size);
            if (child_) child_->rect(child_rect_);
            
            if (h_scroll_bar_ && !hbar)
            {
                remove(h_scroll_bar_); // FIXME: should we call h_scroll_bar_->parent(container::ptr()) ?
                h_scroll_bar_.reset();
            }
            if (v_scroll_bar_  && !vbar)
            {
                remove(v_scroll_bar_);
                v_scroll_bar_.reset();
            } 
            
            // create the new scroll bars
            scroll_box::ptr this_ptr = boost::shared_static_cast<scroll_box>(shared_from_this());
            check_scrollable_ = false;
            
            if (vbar && !v_scroll_bar_)
            {
                INFO("Creating Vbar");
                rectangle v_rect(width()-scroll_bar::default_size, 0, 
                                 scroll_bar::default_size, height());
                v_scroll_bar_ = scroll_bar::create(this_ptr, v_rect, v_scroll());
            }
            if (hbar && !h_scroll_bar_)
            {
                rectangle h_rect(0, height()-scroll_bar::default_size, 
                                 width(), scroll_bar::default_size);
                h_scroll_bar_ = scroll_bar::create(this_ptr, h_rect, h_scroll());
            }
        }
        // Assign the correct scroll models to the scrollbars
        if (h_scroll() && h_scroll_bar_) h_scroll_bar_->model(h_scroll());
        if (v_scroll() && v_scroll_bar_) v_scroll_bar_->model(v_scroll());
        
        check_scrollable_ = true;
        redraw(rect());
    }

}
