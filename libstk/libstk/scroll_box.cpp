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
            bool v_policy, bool h_policy)
    {
        scroll_box::ptr new_scroll_box(new scroll_box(parent, rect));
        new_scroll_box->scroll_policies(v_policy, h_policy);
        parent->add(new_scroll_box);
        return new_scroll_box;
    }

    scroll_box::scroll_box(container::ptr parent, const rectangle& rect) : container(parent, rect),
            check_scrollable_(false), v_policy_(false), h_policy_(false)
    {
        INFO("constructor");
        focusable_ = true; 
        child_rect_ = rect; child_rect_.position(point(0, 0));
    }

    scroll_box::~scroll_box()
    {
        INFO("destructor");
    }

    void scroll_box::h_scroll(scroll_model::ptr model)
    {
        viewport_->h_scroll(model);
        if (h_scroll_bar_) h_scroll_bar_->model(model);
    }

    void scroll_box::v_scroll(scroll_model::ptr model)
    {
        viewport_->v_scroll(model);
        if (v_scroll_bar_) v_scroll_bar_->model(model);
    }

    void scroll_box::handle_event(event::ptr e)
    {
        widget::handle_event(e);
    }

    void scroll_box::add(widget::ptr item)
    {
        if (check_scrollable_)
        { // we are adding the child widget
            scrollable::ptr scroll_item;
            if (scroll_item = boost::shared_dynamic_cast<scrollable>(item))
            { // item is scrollable, add it directly
                INFO("item is scrollable");
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
                INFO("item is not scrollable");
                if (item->parent()) item->parent()->remove(item);
                if (!viewport_)
                {
                    check_scrollable_ = false;
                    // FIXME: ugly cast, but I don't see a clean way...
                    scroll_box::ptr this_ptr = boost::shared_static_cast<scroll_box>(shared_from_this());
                    viewport_ = viewport::create(this_ptr, child_rect_);
                    child_ = viewport_;
                    check_scrollable_ = true;
                }
                viewport_->add(item);
            }

            // update the scroll bars to use the new models
            if (v_scroll_bar_) v_scroll_bar_->model(v_scroll());
            if (h_scroll_bar_) h_scroll_bar_->model(h_scroll());
        }
        else
        { // no checking for scrollable, so we adding scrollbars
            container::add(item); 
        }
    }

    scroll_model::ptr scroll_box::h_scroll()
    {
        if (child_)
            // FXIME: ugly cast, but should always be true
            return boost::shared_dynamic_cast<scrollable>(child_)->h_scroll();
        else
            return scroll_model::ptr();
    }

    scroll_model::ptr scroll_box::v_scroll()
    {
        if (child_)
            // FXIME: ugly cast, but should always be true
            return boost::shared_dynamic_cast<scrollable>(child_)->v_scroll();
        else
            return scroll_model::ptr();
    }

    void scroll_box::scroll_policies(bool v_policy, bool h_policy)
    {
        h_policy_ = h_policy;
        v_policy_ = v_policy;

        child_rect_ = rect(); child_rect_.position(point(0, 0));
        if (v_policy_) child_rect_.x2(child_rect_.x2() - scroll_bar::default_size);
        if (h_policy_) child_rect_.y2(child_rect_.y2() - scroll_bar::default_size);
        // FIXME: tell the child we changed their rect
        if (child_) child_->rect(child_rect_);

        scroll_box::ptr this_ptr = boost::shared_static_cast<scroll_box>(shared_from_this());

        // destroy existing scroll bars
        if (h_scroll_bar_)
        {
            remove(h_scroll_bar_);
            h_scroll_bar_.reset();
        }
        if (v_scroll_bar_)
        {
            remove(v_scroll_bar_);
            v_scroll_bar_.reset();
        }

        // create the new scroll bars
        check_scrollable_ = false;
        if (v_policy_ && h_policy_)
        {
            rectangle v_rect(width()-scroll_bar::default_size, 0, 
                    scroll_bar::default_size, height()-scroll_bar::default_size);
            v_scroll_bar_ = scroll_bar::create(this_ptr, v_rect, v_scroll());

            rectangle h_rect(0, height()-scroll_bar::default_size, 
                    width()-scroll_bar::default_size, scroll_bar::default_size);
            h_scroll_bar_ = scroll_bar::create(this_ptr, h_rect, h_scroll());
        }
        else
        {
            if (v_policy_)
            {
                rectangle v_rect(width()-scroll_bar::default_size, 0, 
                        scroll_bar::default_size, height());
                v_scroll_bar_ = scroll_bar::create(this_ptr, v_rect, v_scroll());
            }
            if (h_policy_)
            {
                rectangle h_rect(0, height()-scroll_bar::default_size, 
                        width(), scroll_bar::default_size);
                h_scroll_bar_ = scroll_bar::create(this_ptr, h_rect, h_scroll());
            }
        }
        check_scrollable_ = true;

        redraw(rect());
    }

}
