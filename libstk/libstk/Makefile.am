## compiler flags
AM_CXXFLAGS = -Wall

lib_LTLIBRARIES  =\
	libstk.la

libstk_la_SOURCES =\
	application.h	application.cpp \
	button.h button.cpp \
	color_manager.h color_manager.cpp \
	combo.h combo.cpp \
	component.h \
	container.h container.cpp \
	drawable.h \
	event_system.h \
	event_system_sdl.h event_system_sdl.cpp \
	event_system_dfb.h event_system_dfb.cpp \
	exceptions.h \
	event.h \
	event_handler.h \
	exceptions.h \
	font.h font.cpp \
	font_manager.h font_manager.cpp \
	glyph.h glyph.cpp \
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
	surface_sdl.h surface_sdl.cpp \
	sdl_data.h sdl_data.cpp \
	scroll_bar.h scroll_bar.cpp \
	scroll_box.h scroll_box.cpp \
	scroll_model.h scroll_model.cpp \
	spinner.h spinner.cpp \
	state.h state.cpp \
	surface.h surface_impl.h \
	surface_dfb.h surface_dfb.cpp \
	theme.h theme.cpp \
	time_value.h \
	timer.h timer.cpp \
	tribal_theme.h tribal_theme.cpp \
	viewport.h viewport.cpp \
	widget.h widget.cpp

library_includedir=$(includedir)/libstk-$(VERSION)/libstk
	
library_include_HEADERS= \
	aa_filter.h      event_system.h      key_event.h    state.h \
	application.h    event_system_dfb.h  keycode.h      stk.h	\
	button.h         event_system_sdl.h  label.h        surface.h	\
	combiners.h      exceptions.h        list.h         surface_dfb.h \
	combo.h          font.h              list_item.h    surface_sdl.h \
	container.h      font_manager.h      mouse_event.h  rectangle.h \
	drawable.h       glyph.h             component.h    widget.h 	\
	edge.h           graphics_context.h  point.h		    image.h \
	event.h          progress.h 	       scroll_box.h  	event_handler.h \
	image_panel.h    spinner.h           scroll_model.h theme.h \
	color_manager.h  timer.h             surface_impl.h time_value.h \
	posix_time.h     viewport.h          sdl_data.h


bin_PROGRAMS       = test_app surface_bm
test_app_SOURCES   = test_app.cpp
surface_bm_SOURCES = surface_bm.cpp
	
# FIXME: do the autotools thing for freetype
test_app_LDADD = $(SDL_LIBS) -lboost_signals -lfreetype -lz libstk.la
surface_bm_LDADD = $(SDL_LIBS) -lboost_signals -lfreetype -lz libstk.la
libstk_la_LDFLAGS= $(SDL_LIBS) -version-info @libstk_libtool_version@

INCLUDES = $(SDL_CFLAGS) -I/usr/include/freetype2 -I$(top_srcdir)
