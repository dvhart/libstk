# libstk src Makefile.am

CPPFLAGS=@CFLAGS@ -g -Wall
LDFLAGS=-lsigc
#fix me with macros like the SDL stuff
INCLUDES = -I/usr/include/sigc++-1.0 -I/usr/lib/sigc++-1.0/include

lib_LTLIBRARIES  = libstk.la

libstk_la_SOURCES = stk_util.cpp stk_app.cpp stk_widget.cpp \
                    stk_state.cpp stk_button.cpp stk_listbox.cpp stk_list_item.cpp \
                    stk_font.cpp stk_theme.cpp stk_theme_item.cpp stk_combo.cpp \
	            stk_image.cpp stk_edit.cpp stk_progress.cpp stk_checkbox.cpp \
                    stk_label.cpp stk_callbacks.cpp

libstk_la_LIBADD = -lm @LDFLAGS@


include_HEADERS  =  stk.h stk_util.h stk_widget.h stk_state.h stk_button.h \
                    stk_listbox.h stk_list_item.h stk_font.h stk_theme.h \
                    stk_theme_item.h stk_app.h stk_combo.h stk_image.h \
                    stk_edit.h stk_progress.h stk_checkbox.h stk_label.h \
		    stk_callbacks.h

# the example did these instead/too
#hello_LDADD = @INTLLIBS@ @ALLOCA@
#localedir = $(datadir)/locale
#INCLUDES = -I../intl -DLOCALEDIR=\"$(localedir)\"

