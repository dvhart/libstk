#!/bin/sh

set -x
aclocal-1.7
autoheader2.50
#autoheader
automake-1.7 --foreign --add-missing --copy
autoconf2.50
#autoconf
