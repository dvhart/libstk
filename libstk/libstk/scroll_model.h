/******************************************************************************
 *    FILENAME: scroll_model.h
 * DESCRIPTION: Class describing the modell (MVC term) behind scrolling 
 *     AUTHORS: Marc Straemke, Darren Hart
 *  START DATE: 13/May/2003  LAST UPDATE: 13/May/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available
 *              with the source as license.txt or at 
 *              http://www.libstk.org/index.php?page=docs/license
 *****************************************************************************/

#ifndef SCROLL_MODELL_H
#define SCROLL_MODELL_H

#include <boost/smart_ptr.hpp>
#include <boost/signal.hpp>

namespace stk
{
	/// This class describes the modell or data associated with scrolling
	class scroll_modell
	{
		public:
			typedef boost::shared_ptr<scroll_modell> ptr;	
			typedef boost::weak_ptr<scroll_modell> weak_ptr;
		private:
			/// The Size of the Scrollable area
			int size_;
			/// The Begin of the Visible Area
			int begin_;
			/// The End of the Visible Area
			int end_;
		public:
			int size();
			void size(int newsize);
			int begin();
			void begin(int value);
			int end();
			void end(int value);
			scroll_modell();

			boost::signal<void () > on_change;
	};

} // namespace stk

#endif

