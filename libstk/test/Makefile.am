## compiler flags
AM_CXXFLAGS = -Wall

if SDL
 tests_sdl = hello_world test_app
else 
 tests_sdl =
endif
if DFB
 tests_dfb = hello_world_dfb test_app_dfb
 dfb_cflags = -I/usr/local/include/directfb -I/usr/include/directfb
else
 tests_dfb = 
endif

library_includedir  = $(includedir)/libstk-$(VERSION)/libstk
# FIXME: do the autotools thing for freetype
INCLUDES = $(SDL_CFLAGS) -I/usr/include/freetype2 -I$(top_srcdir)  $(dfb_cflags)

bin_PROGRAMS        = $(tests_sdl) $(tests_dfb)
hello_world_SOURCES = hello_world.cpp
test_app_SOURCES    = test_app.cpp
test_app_dfb_SOURCES = test_app_dfb.cpp
hello_world_dfb_SOURCES = hello_world_dfb.cpp
#surface_bm_SOURCES  = surface_bm.cpp
	
# FIXME: fix the way we link to the library here
hello_world_LDADD   = $(SDL_LIBS) -lboost_signals -lboost_thread -lfreetype -lz $(top_srcdir)/libstk/libstk.la
test_app_LDADD      = $(SDL_LIBS) -lboost_signals -lboost_thread -lfreetype -lz $(top_srcdir)/libstk/libstk.la
test_app_dfb_LDADD      = -lboost_signals -lboost_thread -lfreetype -lz $(top_srcdir)/libstk/libstk.la -ldirectfb
hello_world_dfb_LDADD      = -lboost_signals -lboost_thread -lfreetype -lz $(top_srcdir)/libstk/libstk.la -ldirectfb
#surface_bm_LDADD    = $(SDL_LIBS) -lboost_signals -lboost_thread -lfreetype -lz $(top_srcdir)/libstk/libstk.la

