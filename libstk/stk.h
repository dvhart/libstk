/**************************************************************************************************
 *     CVS $Id$
 * DESCRIPTION: Libstk common header files for inclusion in application, do
 *              not include this file in Libstk implementation files.
 *     AUTHORS: Darren Hart 
 *  START DATE: 2003/Dec/11
 *
 *   COPYRIGHT: 2003 by Darren Hart, Vernon Mauery, Marc Str�mke, Dirk H�rner
 *     LICENSE: This software is licenced under the Libstk license available with the source as 
 *              license.txt or at http://www.libstk.org/index.php?page=docs/license
 *************************************************************************************************/

#ifndef STK_H
#define STK_H

#include <libstk/config.h>

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
#include <libstk/hotkey.h>
#include <libstk/image.h>
#include <libstk/image_panel.h>
#include <libstk/key_event.h>
#include <libstk/keycode.h>
#include <libstk/label.h>
#include <libstk/list.h>
#include <libstk/list_item.h>
#include <libstk/list_item_userdata.h>
#include <libstk/mouse_event.h>
#include <libstk/numeric_spinner.h>
#include <libstk/overlay.h>
#include <libstk/point.h>
#include <libstk/posix_time.h>
#include <libstk/progress.h>
#include <libstk/rectangle.h>
#include <libstk/scroll_bar.h>
#include <libstk/scroll_box.h>
#include <libstk/scroll_model.h>
#include <libstk/spinner.h>
#include <libstk/state.h>
#include <libstk/stk_types.h>
#include <libstk/surface.h>
#include <libstk/logging.h>
//#include <libstk/surface_dfb.h> // backend specific
//#include <libstk/surface_impl.h> // only needed by backend not by user code
//#include <libstk/surface_sdl.h> // backend specific
#include <libstk/theme.h>
#include <libstk/time_value.h>
#include <libstk/timer.h>
//#include <libstk/tribal_theme.h> // theme specific
#include <libstk/viewport.h>
#include <libstk/widget.h>
#include <libstk/edit_box.h>
#include <libstk/text_area.h>
#include <libstk/switched_panel.h>
#ifdef HAVE_XINE
# include <libstk/xine_panel.h>
#endif
#include <libstk/override_return.h>

#endif
