lib_LTLIBRARIES  =\
	libstk.la

libstk_la_SOURCES =\
	app.h	app.cpp            \
	surface.h    surface.cpp   \
	widget.h     widget.cpp    \
	events.h     events.cpp    \
	exceptions.h	           \
	container.h  container.cpp


libstk_la_LDFLAGS =\
	$(SDL_LIBS)

INCLUDES =\
	$(SDL_CFLAGS) -I$(top_srcdir)
