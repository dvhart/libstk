lib_LTLIBRARIES  =\
	libstk.la

libstk_la_SOURCES =\
	app.h	app.cpp            \
	surface.h    surface.cpp   \
	widget.h     widget.cpp    \
	exceptions.h	           \
	container.h  container.cpp \
	state.h state.cpp
	
bin_PROGRAMS      = stk_test
stk_test_SOURCES  = 		   \
	app.h	app.cpp            \
	surface.h    surface.cpp   \
	widget.h     widget.cpp    \
	exceptions.h	           \
	container.h  container.cpp \
	state.h state.cpp test.cpp
stk_test_LDADD = $(SDL_LIBS) $(SIGC_LIBS) -lboost_signals
libstk_la_LDFLAGS =\
	$(SDL_LIBS)

INCLUDES =\
	$(SDL_CFLAGS) -I$(top_srcdir)
