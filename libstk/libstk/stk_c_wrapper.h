#ifndef STK_C_WRAPPER_H
#define STK_C_WRAPPER_H

// move these to a common file (atm also defined in overlay.h)
#define STK_FORMAT_YV12 0x32315659 
#define STK_FORMAT_YUY2 0x32595559

#ifdef __cplusplus
extern "C" {
#endif

typedef struct surface_s surface_t;
typedef struct overlay_s overlay_t;
typedef struct xine_panel_s xine_panel_t;

overlay_t* stk_surface_create_overlay(surface_t* surface, int width, int height, int format);

void stk_overlay_free(overlay_t* overlay);       
void stk_overlay_lock(overlay_t* overlay);       
void stk_overlay_unlock(overlay_t* overlay);       
void stk_overlay_display(overlay_t* overlay, int x, int y, int width, int height);       
int stk_overlay_pitches(overlay_t* overlay, int index);       
unsigned char* stk_overlay_pixels(overlay_t* overlay, int index);       

int stk_xine_panel_x(xine_panel_t* xine_panel);
int stk_xine_panel_y(xine_panel_t* xine_panel);
int stk_xine_panel_width(xine_panel_t* xine_panel);
int stk_xine_panel_height(xine_panel_t* xine_panel);
surface_t* stk_xine_panel_surface(xine_panel_t* xine_panel);


#ifdef __cplusplus
}
#endif

#endif
