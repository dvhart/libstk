lib_LTLIBRARIES  =\
	libstk.la

libstk_la_SOURCES =\
	event_system.h \
	event_system_sdl.h event_system_sdl.cpp \
	event_system_dfb.h event_system_dfb.cpp \
	surface.h surface.cpp \
	surface_sdl.h surface_sdl.cpp \
	surface_dfb.h surface_dfb.cpp \
	event.h \
	mouse_event.h mouse_event.cpp \
	key_event.h key_event.cpp \
	quit_event.h quit_event.cpp \
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
	image.h image.cpp \
	label.h label.cpp \
	exceptions.h 
	
bin_PROGRAMS      = test_app
test_app_SOURCES  = \
	event_system.h \
	event_system_sdl.h event_system_sdl.cpp \
	event_system_dfb.h event_system_dfb.cpp \
	surface.h surface.cpp \
	surface_sdl.h surface_sdl.cpp \
	surface_dfb.h surface_dfb.cpp \
	event.h \
	mouse_event.h mouse_event.cpp \
	key_event.h key_event.cpp \
	quit_event.h quit_event.cpp \
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
	image.h image.cpp \
	label.h label.cpp \
	exceptions.h \
	test_app.cpp
	
test_app_LDADD = $(SDL_LIBS) -lboost_signals
libstk_la_LDFLAGS =	$(SDL_LIBS)

INCLUDES = $(SDL_CFLAGS) -I$(top_srcdir)
