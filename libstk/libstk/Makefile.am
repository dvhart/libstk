## compiler flags
#AM_CXXFLAGS = -Wall

lib_LTLIBRARIES  =\
	libstk.la


if DFB
 headers_dfb = backend_dfb.h surface_dfb.h
 sources_dfb = backend_dfb.cpp backend_dfb.h surface_dfb.cpp surface_dfb.h 
endif

if SDL
 headers_sdl =  event_producer_sdl.h sdl_data.h    surface_sdl.h
 sources_sdl = 	event_producer_sdl.h event_producer_sdl.cpp \
		surface_sdl.h surface_sdl.cpp \
		sdl_data.h sdl_data.cpp 
endif

if PRINTING
 headers_printing = surface_bitmap.h
 sources_printing = surface_bitmap.cpp
endif

if FBDEV
 headers_fbdev = surface_fbdev.h
 sources_fbdev = surface_fbdev.h surface_fbdev.cpp
endif

if LOGGING
 headers_logging = logging.h
 sources_logging = logging.h logging.cpp
endif

if XINE
 headers_xine = xine_panel.h stk_c_wrapper.h
 sources_xine = xine_panel.cpp stk_c_wrapper.cpp 
endif

libstk_la_SOURCES =\
	application.h	application.cpp \
	button.h button.cpp \
	color_manager.h color_manager.cpp \
        combo.h combo.cpp \
	component.h \
	container.h container.cpp \
	drawable.h \
	event_system.h event_system.cpp \
	exceptions.h \
	event.h \
	event_handler.h \
	exceptions.h \
	font_properties.h font_properties.cpp \
	read_dir.h read_dir.cpp \
	font.h font.cpp \
	font_manager.h font_manager.cpp \
	glyph.h glyph.cpp \
	hotkey.h \
	image_panel.h image_panel.cpp \
	image.h image.cpp \
	key_event.h key_event.cpp \
	label.h label.cpp \
	list.h \
	list_item.h list_item.cpp \
	list_item_userdata.h \
	list_template.h \
	mouse_event.h mouse_event.cpp \
	numeric_spinner.h numeric_spinner.cpp \
	overlay.h \
	overlay_dfb.h\
	overlay_sdl.h \
        override_return.h \
	posix_time.h posix_time.cpp \
	progress.h progress.cpp \
	rectangle.h rectangle.cpp \
	scrollable.h scrollable.cpp \
	scroll_bar.h scroll_bar.cpp \
	scroll_box.h scroll_box.cpp \
	scroll_model.h scroll_model.cpp \
	spinner.h spinner.cpp \
	state.h state.cpp \
	stk.h stk_types.h \
	surface.h \
	surface_impl.h \
	theme.h theme.cpp \
	time_value.h \
	timer.h timer.cpp \
	tribal_theme.h tribal_theme.cpp \
	viewport.h viewport.cpp \
	widget.h widget.cpp \
	spreadsheet.cpp spreadsheet.h\
	switched_panel.h switched_panel.cpp \
	$(sources_logging) $(sources_fbdev) \
	$(sources_sdl) $(sources_dfb) \
	edit_box.h edit_box.cpp  \
	text_area.h text_area.cpp \
	event_producer_elotouch.h event_producer_elotouch.cpp \
	$(headers_xine) $(sources_xine)  \
	$(sources_printing) $(headers_printing)

library_includedir=$(includedir)/libstk-$(VERSION)/libstk



library_include_HEADERS= \
	aa_filter.h      event_system.h      key_event.h    state.h \
	application.h    keycode.h           stk.h	\
	button.h         label.h             surface.h	\
	combiners.h      exceptions.h        list.h         list_template.h \
        override_return.h \
	combo.h          font.h              list_item.h    list_item_userdata.h \
	container.h      font_manager.h      mouse_event.h  rectangle.h \
	drawable.h       glyph.h             component.h    widget.h 	\
	edge.h           graphics_context.h  point.h        image.h \
	event.h          progress.h 	     scroll_box.h   event_handler.h \
	image_panel.h    spinner.h           scroll_model.h theme.h \
	color_manager.h  timer.h             surface_impl.h time_value.h \
	posix_time.h     viewport.h          hotkey.h       \
	overlay.h        overlay_sdl.h       overlay_dfb.h \
	stk_types.h      scroll_bar.h        event_producer.h \
	event_producer_elotouch.h            stk_c_wrapper.h \
	switched_panel.h	edit_box.h   text_area.h read_dir.h\
	numeric_spinner.h config.h           $(headers_sdl) $(headers_dfb) \
	$(headers_fbdev) $(headers_logging)  $(headers_xine)  \
	$(headers_printing) font_properties.h scrollable.h \
	spreadsheet.h tribal_theme.h

libstk_la_LDFLAGS= $(SDL_LIBS) $(DIRECTFB_LIBS) $(FREETYPE_LIBS) $(LIBPNG_LIBS) $(LIBJPEG_LIBS) $(XINE_LIBS) -version-info @libstk_libtool_version@ 

INCLUDES = $(SDL_CFLAGS) $(DIRECTFB_CFLAGS) $(FREETYPE_CFLAGS) $(LIBPNG_CFLAGS) $(XINE_CFLAGS) -I$(top_srcdir) \
	   -DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	   -DPACKAGE_PIXMAPS_DIR=\""$(datadir)/libstk/pixmaps"\" \
	   -DPACKAGE_FONTS_DIR=\""$(datadir)/libstk/fonts"\" 
