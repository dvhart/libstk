/***************************************************************************
                          stk_widget.cpp  -  description
                             -------------------
    begin                : Sat Apr 27 2002
    copyright            : (C) 2002 by Darren Hart
    email                : dvhart@byu.edu
***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <string>

#include <string.h>
#include <malloc.h>

#include "widget.h"
#include "container.h"
#include "app.h"

namespace stk
{

widget::widget(boost::weak_ptr<state> state,boost::weak_ptr<container> parent):parent_state_(state),parent_(parent)
{
}
	

widget::~widget()
{
}

}
