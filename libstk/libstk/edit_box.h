/******************************************************************************
 *    FILENAME: edit_box.h
 * DESCRIPTION: An edit box widget.
 *     AUTHORS: Dirk Hoerner
 *  START DATE: 23/Jun/2003  LAST UPDATE: 23/Jun/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available
 *              with the source as license.txt or at 
 *              http://www.libstk.org/index.php?page=docs/license
 *****************************************************************************/

#ifndef STK_EDIT_BOX_H
#define STK_EDIT_BOX_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/signal.hpp>

#include <libstk/widget.h>
#include <libstk/container.h>

namespace stk
{
	class edit_box : public widget
	{
	public:
		typedef boost::shared_ptr<edit_box> ptr;
		typedef boost::weak_ptr<edit_box> weak_ptr;

	private:
		std::wstring text_;

	protected:
		edit_box(container::ptr parent, const std::wstring& text, const rectangle& rect);
	
	public:
		//static edit_box::ptr create(container::ptr parent, const rectangle& rect);
		static edit_box::ptr create(container::ptr parent, const std::wstring& text,
				                        const rectangle& rect);

		virtual ~edit_box();

		std::wstring text() const
		{
			return text_;
		}
		void text(std::wstring& text)
		{
			text_ = text;
		}

		// signals
		boost::signal<bool (std::wstring), combiner::logical_or<bool> > on_press;
		boost::signal<bool (std::wstring), combiner::logical_or<bool> > on_release;
		
		virtual void draw(surface::ptr surface, const rectangle& clip_rect);
		virtual void handle_event(event::ptr e);
	};
}

#endif