#!/bin/sh

set -x
aclocal
#autoheader2.50
autoheader
automake --foreign --add-missing --copy
#autoconf2.50
autoconf
