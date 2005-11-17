if TESTS
    test_dir = test
else
    test_dir = 
endif
# $(test_dir)
SUBDIRS = libstk test 

DIST_SUBDIRS= libstk test

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libstk.pc

pixmapsdir = $(datadir)/libstk/pixmaps
pixmaps_DATA = pixmaps/tribal_bg.png \
               pixmaps/parrots.png

fontsdir = $(datadir)/libstk/fonts
fonts_DATA = fonts/VeraIt.ttf     fonts/VeraMono.ttf \
             fonts/Vera.ttf       fonts/VeraMoBI.ttf \
	     fonts/VeraSe.ttf     fonts/VeraBI.ttf \
             fonts/VeraMoBd.ttf   fonts/VeraSeBd.ttf \
             fonts/VeraBd.ttf     fonts/VeraMoIt.ttf \
             fonts/copyright

EXTRA_DIST=autogen.sh $(pixmaps_DATA) $(fonts_DATA)

