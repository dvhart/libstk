/******************************************************************************
 *    FILENAME: stk.h
 * DESCRIPTION: Libstk common header files and typedefs used throughout Libstk.
 *     AUTHORS: Darren Hart 
 *  START DATE: 11/Dec/2003  LAST UPDATE: 13/May/2003
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Straemke, Dirk Hoerner
 *     LICENSE: This software is licenced under the Libstk license available
 *              with the source as license.txt or at 
 *              http://www.libstk.org/index.php?page=docs/license
 *****************************************************************************/

#ifndef STK_H
#define STK_H

/* This file contains macros and typedefs used throughout the stk codebase.
 * (Eventually this should contain all the files needed by the user, so
 * they can simply "#include <stk.h>"
 */

// macros
#ifndef MIN
#define MIN(A,B) (((A)<(B))?(A):(B))
#endif

#ifndef MAX
#define MAX(A,B) (((A)>(B))?(A):(B))
#endif

namespace stk
{
	// FIXME
	// wrap with some #ifdefs to assure color is an unsigned 32 bit integer
	typedef unsigned int color;
	typedef unsigned char byte;
}

// boost headers
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/signal.hpp>
#include <boost/bind.hpp>

// libstk headers
//#include <libstk/aa_filter.h> // not really a header file
#include <libstk/application.h>
#include <libstk/button.h>
#include <libstk/color_manager.h>
#include <libstk/combiners.h>
#include <libstk/combo.h>
#include <libstk/component.h>
#include <libstk/container.h>
#include <libstk/drawable.h>
#include <libstk/edge.h>
#include <libstk/event.h>
#include <libstk/event_handler.h>
#include <libstk/event_system.h>
//#include <libstk/event_system_dfb.h> // backend specific
//#include <libstk/event_system_sdl.h> // backend specific 
#include <libstk/exceptions.h>
#include <libstk/font.h>
#include <libstk/font_manager.h>
#include <libstk/glyph.h>
#include <libstk/graphics_context.h>
#include <libstk/image.h>
#include <libstk/image_panel.h>
#include <libstk/key_event.h>
#include <libstk/keycode.h>
#include <libstk/label.h>
#include <libstk/list.h>
#include <libstk/list_item.h>
#include <libstk/mouse_event.h>
#include <libstk/point.h>
#include <libstk/posix_time.h>
#include <libstk/progress.h>
#include <libstk/rectangle.h>
#include <libstk/scroll_bar.h>
#include <libstk/scroll_box.h>
#include <libstk/scroll_model.h>
#include <libstk/sdl_data.h>
#include <libstk/spinner.h>
#include <libstk/state.h>
#include <libstk/stk.h>
#include <libstk/surface.h>
//#include <libstk/surface_dfb.h> // backend specific
#include <libstk/surface_impl.h>
//#include <libstk/surface_sdl.h> // backend specific 
#include <libstk/theme.h>
#include <libstk/time_value.h>
#include <libstk/timer.h>
//#include <libstk/tribal_theme.h> // theme specific
#include <libstk/viewport.h>
#include <libstk/widget.h>

#endif
