if TESTS
    test_dir = test
else
    test_dir = 
endif
SUBDIRS = libstk $(test_dir)

DIST_SUBDIRS= libstk test

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libstk.pc

pixmapsdir = $(datadir)/libstk/pixmaps
pixmaps_DATA = pixmaps/tribal_bg.png

fontsdir = $(datadir)/libstk/fonts
fonts_DATA = fonts/FreeSans.ttf            fonts/FreeSerifBold.ttf \
             fonts/FreeMono.ttf            fonts/FreeSansBold.ttf \
             fonts/FreeSerifBoldItalic.ttf fonts/FreeMonoBold.ttf \
             fonts/FreeSansBoldOblique.ttf fonts/FreeSerifItalic.ttf \
             fonts/FreeMonoBoldOblique.ttf fonts/FreeSansOblique.ttf \
             fonts/FreeMonoOblique.ttf     fonts/FreeSerif.ttf \
             fonts/copyright               fonts/GPL

EXTRA_DIST=autogen.sh $(pixmaps_DATA) $(fonts_DATA)

