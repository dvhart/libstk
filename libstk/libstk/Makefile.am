lib_LTLIBRARIES  =\
	libstk.la

libstk_la_SOURCES =\
	decorator.h \
	event_system.h \
	event_system_sdl.h event_system_sdl.cpp \
	event_system_dfb.h event_system_dfb.cpp \
	glyph.h glyph.cpp \
	font.h font.cpp \
	rectangle.h rectangle.cpp \
	surface.h surface.cpp \
	surface_sdl.h surface_sdl.cpp \
	surface_dfb.h surface_dfb.cpp \
	event.h \
	mouse_event.h mouse_event.cpp \
	key_event.h key_event.cpp \
	event_handler.h \
	drawable.h \
	parent.h \
	application.h	application.cpp \
	widget.h widget.cpp \
	container.h container.cpp \
	state.h state.cpp \
	list.h list.cpp \
	combo.h combo.cpp \
	list_item.h list_item.cpp \
	button.h button.cpp \
	progress.h progress.cpp \
	image_panel.h image_panel.cpp \
	image.h image.cpp \
	label.h label.cpp \
	scroll_decorator.h scroll_decorator.cpp \
	spinner.h spinner.cpp \
	tribal_theme.cpp \
	font_mngr.h font_mngr.cpp\
	exceptions.h 

library_includedir=$(includedir)/libstk-$(VERSION)/libstk
	
library_include_HEADERS= \
	aa_filter.h      event_system.h      key_event.h    state.h \
	application.h    event_system_dfb.h  keycode.h      stk.h	\
	button.h         event_system_sdl.h  label.h        surface.h	\
	combiners.h      exceptions.h        list.h         surface_dfb.h \
	combo.h          font.h              list_item.h    surface_sdl.h \
	container.h      font_mngr.h         mouse_event.h  rectangle.h \
	drawable.h       glyph.h             parent.h       widget.h 	\
	edge.h           graphics_context.h  point.h		    decorator.h \
	event.h          image.h             progress.h 	  scroll_decorator.h \
	event_handler.h  image_panel.h       


bin_PROGRAMS      = test_app
test_app_SOURCES  = test_app.cpp
	
# FIXME: do the autotools thing for freetype
test_app_LDADD = $(SDL_LIBS) -lboost_signals -lfreetype -lz libstk.la
libstk_la_LDFLAGS= $(SDL_LIBS) -version-info @libstk_libtool_version@

INCLUDES = $(SDL_CFLAGS) -I/usr/include/freetype2 -I$(top_srcdir)
