#!/bin/sh
libtoolize --force
aclocal
autoheader
automake --add-missing
autoconf
./configure
