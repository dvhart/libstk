if TESTS
    test_dir = test
else
    test_dir = 
endif
SUBDIRS = libstk $(test_dir)

DIST_SUBDIRS= libstk test

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libstk.pc

EXTRA_DIST=autogen.sh

