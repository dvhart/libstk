## compiler flags
AM_CXXFLAGS = -Wall


if XINE
 tests_xine  = xine_test
else
 tests_xine =
endif
if SDL
 tests_sdl  = hello_world test_app hydra $(tests_xine)
else 
 tests_sdl  =
endif
if DFB
 tests_dfb  = hello_world_dfb test_app_dfb
 dfb_cflags = -I/usr/local/include/directfb -I/usr/include/directfb
else
 tests_dfb  = 
endif
if FBDEV
 tests_fbdev  = hello_world_fbdev test_app_fbdev
else
 tests_fbdev  = 
endif

library_includedir  = $(includedir)/libstk-$(VERSION)/libstk
# FIXME: do the autotools thing for freetype
INCLUDES = $(SDL_CFLAGS) -I/usr/include/freetype2 -I$(top_srcdir)  $(dfb_cflags)

bin_PROGRAMS              = $(tests_sdl) $(tests_dfb) $(tests_fbdev)
hello_world_SOURCES       = hello_world.cpp
test_app_SOURCES          = test_app.cpp
hydra_SOURCES             = hydra.cpp
xine_test_SOURCES         = xine_test.cpp
test_app_dfb_SOURCES      = test_app_dfb.cpp
hello_world_dfb_SOURCES   = hello_world_dfb.cpp
test_app_fbdev_SOURCES    = test_app_fbdev.cpp
hello_world_fbdev_SOURCES = hello_world_fbdev.cpp
	
# FIXME: fix the way we link to the library here
hello_world_LDADD       = $(SDL_LIBS) -lboost_signals -lboost_thread -lfreetype -lz $(top_srcdir)/libstk/libstk.la
test_app_LDADD          = $(SDL_LIBS) -lboost_signals -lboost_thread -lfreetype -lz $(top_srcdir)/libstk/libstk.la
hydra_LDADD             = $(SDL_LIBS) -lboost_signals -lboost_thread -lfreetype -lz $(top_srcdir)/libstk/libstk.la
xine_test_LDADD         = $(SDL_LIBS) -lboost_signals -lboost_thread -lfreetype -lz $(top_srcdir)/libstk/libstk.la 
xine_test_LDFLAGS       = `xine-config --libs`
test_app_dfb_LDADD      = -lboost_signals -lboost_thread -lfreetype -lz $(top_srcdir)/libstk/libstk.la -ldirectfb
hello_world_dfb_LDADD   = -lboost_signals -lboost_thread -lfreetype -lz $(top_srcdir)/libstk/libstk.la -ldirectfb
hello_world_fbdev_LDADD = -lboost_signals -lboost_thread -lfreetype -lz $(top_srcdir)/libstk/libstk.la
test_app_fbdev_LDADD    = -lboost_signals -lboost_thread -lfreetype -lz $(top_srcdir)/libstk/libstk.la

