## compiler flags
#AM_CXXFLAGS = -Wall

lib_LTLIBRARIES  =\
	libstk.la


if DFB
 headers_dfb=backend_dfb.h surface_dfb.h
 sources_dfb=backend_dfb.cpp backend_dfb.h surface_dfb.cpp surface_dfb.h 
 libs_dfb=-ldirectfb
endif

if SDL
 headers_sdl= event_producer_sdl.h sdl_data.h    surface_sdl.h
 sources_sdl= 	event_producer_sdl.h event_producer_sdl.cpp \
		surface_sdl.h surface_sdl.cpp \
		sdl_data.h sdl_data.cpp 
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
	font.h font.cpp \
	font_manager.h font_manager.cpp \
	glyph.h glyph.cpp \
	hotkey.h \
	image_panel.h image_panel.cpp \
	image.h image.cpp \
	key_event.h key_event.cpp \
	label.h label.cpp \
	list.h list.cpp \
	list_item.h list_item.cpp \
	mouse_event.h mouse_event.cpp \
	posix_time.h posix_time.cpp \
	progress.h progress.cpp \
	rectangle.h rectangle.cpp \
	scroll_bar.h scroll_bar.cpp \
	scroll_box.h scroll_box.cpp \
	scroll_model.h scroll_model.cpp \
	spinner.h spinner.cpp \
	state.h state.cpp \
	stk.h stk_types.h \
	surface.h surface_impl.h \
	theme.h theme.cpp \
	time_value.h \
	timer.h timer.cpp \
	tribal_theme.h tribal_theme.cpp \
	viewport.h viewport.cpp \
	widget.h widget.cpp \
	logging.h logging.cpp \
	$(sources_sdl) $(sources_dfb) \
	edit_box.h edit_box.cpp  \
	event_producer_elotouch.h event_producer_elotouch.cpp 


library_includedir=$(includedir)/libstk-$(VERSION)/libstk



library_include_HEADERS= \
	aa_filter.h      event_system.h      key_event.h    state.h \
	application.h    keycode.h           stk.h	\
	button.h         label.h             surface.h	\
	combiners.h      exceptions.h        list.h          \
	combo.h          font.h              list_item.h  \
	container.h      font_manager.h      mouse_event.h  rectangle.h \
	drawable.h       glyph.h             component.h    widget.h 	\
	edge.h           graphics_context.h  point.h        image.h \
	event.h          progress.h 	     scroll_box.h   event_handler.h \
	image_panel.h    spinner.h           scroll_model.h theme.h \
	color_manager.h  timer.h             surface_impl.h time_value.h \
	posix_time.h     viewport.h          hotkey.h \
	stk_types.h      scroll_bar.h        event_producer.h \
	$(headers_sdl)   $(headers_dfb)      config.h       \
	event_producer_elotouch.h

# FIXME: do the autotools thing for freetype
libstk_la_LDFLAGS= $(SDL_LIBS) -version-info @libstk_libtool_version@ $(libs_dfb)

INCLUDES = $(SDL_CFLAGS) -I/usr/include/freetype2 -I$(top_srcdir) -I/usr/local/include/directfb/ -I/usr/include/directfb/
