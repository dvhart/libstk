#!/bin/sh

set -x
aclocal-1.7
autoheader
#autoheader
automake-1.7 --foreign --add-missing --copy
autoconf
#autoconf
