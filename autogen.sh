#!/bin/sh

rm -rf autom4te.cache

echo "Running Autoheader..."; autoheader || exit 1

echo "Running Aclocal..."; aclocal || exit 1

echo "Running Autoconf..."; autoconf || exit 1

echo "Running Libtoolize..."; libtoolize --automake --copy || exit 1

echo "Running Automake..."; automake -a -c --gnu || exit 1

if [ -x configure ];then
    ./configure $*
fi
