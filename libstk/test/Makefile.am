## compiler flags
AM_CXXFLAGS = -Wall

library_includedir  = $(includedir)/libstk-$(VERSION)/libstk
# FIXME: do the autotools thing for freetype
INCLUDES = $(SDL_CFLAGS) -I/usr/include/freetype2 -I$(top_srcdir)

bin_PROGRAMS        = test_app surface_bm hello_world
test_app_SOURCES    = test_app.cpp
surface_bm_SOURCES  = surface_bm.cpp
hello_world_SOURCES = hello_world.cpp
	
# FIXME: fix the way we link to the library here
test_app_LDADD      = $(SDL_LIBS) -lboost_signals -lfreetype -lz $(top_srcdir)/libstk/libstk.la
surface_bm_LDADD    = $(SDL_LIBS) -lboost_signals -lfreetype -lz $(top_srcdir)/libstk/libstk.la
hello_world_LDADD   = $(SDL_LIBS) -lboost_signals -lfreetype -lz $(top_srcdir)/libstk/libstk.la

