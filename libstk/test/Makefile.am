## compiler flags
AM_CXXFLAGS = -Wall


if XINE
 tests_xine  = xine_test
else
 tests_xine =
endif
if SDL
 tests_sdl  = test_app hello_world hydra $(tests_xine)
else 
 tests_sdl  =
endif
if DFB
 tests_dfb  = test_app hello_world
else
 tests_dfb  = 
endif
if FBDEV
 tests_fbdev  = test_app hello_world
else
 tests_fbdev  = 
endif



library_includedir  = $(includedir)/libstk-$(VERSION)/libstk
INCLUDES = $(SDL_CFLAGS) $(DIRECTFB_CFLAGS) $(FREETYPE_CFLAGS) $(LIBPNG_CFLAGS) -I$(top_srcdir)  

bin_PROGRAMS              = $(tests_sdl) $(tests_dfb) $(tests_fbdev)
hello_world_SOURCES       = hello_world.cpp
test_app_SOURCES          = test_app.cpp
hydra_SOURCES             = hydra.cpp
xine_test_SOURCES         = xine_test.cpp
	
# FIXME: fix the way we link to the library here
hello_world_LDADD       = $(SDL_LIBS) $(DIRECTFB_LIBS) $(FREETYPE_LIBS) $(LIBPNG_LIBS) -lboost_signals -lboost_thread $(top_srcdir)/libstk/libstk.la
test_app_LDADD          = $(SDL_LIBS) $(DIRECTFB_LIBS) $(FREETYPE_LIBS) $(LIBPNG_LIBS) -lboost_signals -lboost_thread $(top_srcdir)/libstk/libstk.la
hydra_LDADD             = $(SDL_LIBS) $(DIRECTFB_LIBS) $(FREETYPE_LIBS) $(LIBPNG_LIBS) -lboost_signals -lboost_thread $(top_srcdir)/libstk/libstk.la
xine_test_LDADD         = $(SDL_LIBS) $(DIRECTFB_LIBS) $(FREETYPE_LIBS) $(LIBPNG_LIBS) -lboost_signals -lboost_thread $(top_srcdir)/libstk/libstk.la 
xine_test_LDFLAGS       = `xine-config --libs`

