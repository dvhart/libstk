#include <iostream>
#include <vector>
#include <algorithm>
#include <boost/thread/mutex.hpp>

#include "libstk/rectangle.h"
#include "libstk/surface.h"
#include "libstk/overlay.h"
#include "libstk/xine_panel.h"
#include "libstk/stk_c_wrapper.h"
#include "libstk/logging.h"

#ifdef __cplusplus
extern "C" {
#endif

static std::vector<stk::overlay::ptr> overlays;
static boost::mutex overlays_mutex;
    
overlay_t* stk_surface_create_overlay(surface_t* surface, int width, int height, int format)
{
    boost::mutex::scoped_lock scoped_lock(overlays_mutex);
    INFO("stk_surface_create_overlay()");
    stk::overlay::ptr overlay = 
        reinterpret_cast<stk::surface*>(surface)->create_overlay(width, height, format);
    overlays.push_back(overlay);
    return reinterpret_cast<overlay_t*>(overlay.get());
}

void stk_overlay_free(overlay_t* overlay)
{
    boost::mutex::scoped_lock scoped_lock(overlays_mutex);
    INFO("stk_overlay_free(): " << overlays.size() << " overlays stored");
    stk::overlay::ptr temp_overlay(reinterpret_cast<stk::overlay*>(overlay));
    // remove overlay from vector (automatic delete should follow)
    std::vector<stk::overlay::ptr>::iterator o_iter;
    if ((o_iter = std::find(overlays.begin(), overlays.end(), temp_overlay)) != overlays.end())
    {
        INFO("Found overlay: w x h = " << (*o_iter)->width() << " x " << (*o_iter)->height());
        overlays.erase(o_iter);
    }
    INFO("stk_overlay_free() done: " << overlays.size() << " remain");
}

void stk_overlay_lock(overlay_t* overlay)
{
    INFO("stk_overlay_lock()");
    reinterpret_cast<stk::overlay*>(overlay)->lock();
}

void stk_overlay_unlock(overlay_t* overlay)
{
    INFO("stk_overlay_unlock()");
    reinterpret_cast<stk::overlay*>(overlay)->unlock();
}

void stk_overlay_display(overlay_t* overlay, int x, int y, int width, int height)
{
    INFO("stk_overlay_display()");
    reinterpret_cast<stk::overlay*>(overlay)->display(stk::rectangle(x, y, width, height));
}

int stk_overlay_pitches(overlay_t* overlay, int index)
{
    INFO("stk_overlay_pitches()");
    return reinterpret_cast<stk::overlay*>(overlay)->pitches(index);
}

unsigned char* stk_overlay_pixels(overlay_t* overlay, int index)
{
    INFO("stk_overlay_pixels()");
    return reinterpret_cast<stk::overlay*>(overlay)->pixels(index);
}

// xine_panel methods
int stk_xine_panel_x(xine_panel_t* xine_panel)
{
    INFO("stk_xine_panel_x()");
    return reinterpret_cast<stk::xine_panel*>(xine_panel)->x1();
}
int stk_xine_panel_y(xine_panel_t* xine_panel)
{
    INFO("stk_xine_panel_y()");
    return reinterpret_cast<stk::xine_panel*>(xine_panel)->y1();
}
int stk_xine_panel_width(xine_panel_t* xine_panel)
{
    INFO("stk_xine_panel_width()");
    return reinterpret_cast<stk::xine_panel*>(xine_panel)->width();
}
int stk_xine_panel_height(xine_panel_t* xine_panel)
{
    INFO("stk_xine_panel_height()");
    return reinterpret_cast<stk::xine_panel*>(xine_panel)->height();
}
surface_t* stk_xine_panel_surface(xine_panel_t* xine_panel)
{
    INFO("stk_xine_panel_surface()");
    return reinterpret_cast<surface_t*>(
            reinterpret_cast<stk::xine_panel*>(xine_panel)->surface().get());
}

#ifdef __cplusplus
}
#endif

