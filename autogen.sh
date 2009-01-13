#!/bin/sh

if [ -d autom4te.cache ];then
    echo "Remove autotools cache..."
    rm -rf autom4te.cache
fi

echo "Running Autoheader..."; autoheader || exit 1

echo "Running Aclocal..."; aclocal || exit 1

echo "Running Autoconf..."; autoconf || exit 1

echo "Running Libtoolize..."; libtoolize --automake --copy || exit 1

echo "Running Automake..."; automake -a -c --gnu || exit 1

if [ -x configure ];then
    ./configure $*
fi
