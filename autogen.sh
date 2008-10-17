#!/bin/sh

failed()
{
    echo "FAILED !"
    exit 1
}

rm -rf autom4te.cache

echo "Running Autoheader..."; autoheader || failed

echo "Running Aclocal..."; aclocal || failed

echo "Running Autoconf..."; autoconf || failed

echo "Running Libtoolize..."; libtoolize --automake --copy || failed

echo "Running Automake..."; automake -a -c --gnu || failed

if [ -x configure ];then
    ./configure $*
fi
