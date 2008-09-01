#!/bin/sh

failed()
{
    echo "FAILED !"
    exit 1
}

echo -n "Running Autoheader..."
 autoheader || failed
echo "done"

echo -n "Running Aclocal..."
 aclocal || failed
echo "done"

echo -n "Running Autoconf..."
 autoconf || failed
echo "done"

echo -n "Running Automake..."
 automake -a -c --gnu || failed
echo "done"

echo "you can run now : ./configure"