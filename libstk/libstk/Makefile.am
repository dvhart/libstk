lib_LTLIBRARIES  =\
	libstk.la

libstk_la_SOURCES =\
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
	theme.h theme.cpp \
	font_mngr.h font_mngr.cpp\
	exceptions.h 

library_includedir=$(includedir)/libstk-1.0/libstk
	
library_include_HEADERS= \
	event_system.h event_system_sdl.h event_system_dfb.h glyph.h  \
	font.h rectangle.h surface.h surface_sdl.h \
	surface_dfb.h event.h mouse_event.h key_event.h \
	event_handler.h drawable.h parent.h application.h \
	widget.h container.h state.h list.h \
	combo.h list_item.h button.h progress.h \
	image_panel.h image.h label.h theme.h font_mngr.h \
	exceptions.h 

bin_PROGRAMS      = test_app
test_app_SOURCES  = test_app.cpp
	
# FIXME: do the autotools thing for freetype
test_app_LDADD = $(SDL_LIBS) -lboost_signals -lfreetype -lz libstk.la
libstk_la_LDFLAGS= $(SDL_LIBS) -version-info @libstk_libtool_version@

INCLUDES = $(SDL_CFLAGS) -I/usr/include/freetype2 -I$(top_srcdir)
