/***************************************************************************
 *                                                                         *
 * 	sroll_modell.h - Class describing the modell (MVC term) behind scrolling*
 * 	---------------------------------------------------------------------  *
 * 	begin                : Fri May 2 2003                           	   *
 * 	copyright            : (C) 2003 by Marc Straemke                       *
 * 	email                : marcstraemke@gmx.net                            *
 * 	                                                                       *
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the Libstk Library License, Version 1.0         *
 *   ,or (at your option) any later Version of the License				   *				
 *                                                                         *
 ***************************************************************************/
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

//
