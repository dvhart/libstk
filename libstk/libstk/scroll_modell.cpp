/***************************************************************************
 *                                                                         *
 * 	sroll_modell.cpp - Implementation of the class stk::scroll_modell*
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
 
#include "scroll_modell.h"
 
namespace stk
{
 
int scroll_modell::size()
{
	return size_;
}
void scroll_modell::size(int newsize)
{
	size_=newsize;
	on_change();
}
int scroll_modell::begin()
{
	return begin_;
}
void scroll_modell::begin(int value)
{
	begin_=value;
	on_change();
}
int scroll_modell::end()
{
	return end_;
}
void scroll_modell::end(int value)
{
	end_=value;
	on_change();
}
scroll_modell::scroll_modell():begin_(0),end_(1),size_(1)
{}
	
} // namespace stk
