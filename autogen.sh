#!/bin/sh
# Run this to generate the configure script.

(autoconf --version) < /dev/null > /dev/null 2>&1 || {
	echo
	echo "Error: You must have 'autoconf' installed on your system in order"
	echo "  to generate a configure script."
	echo
	exit 1
}

(autoheader --version) < /dev/null > /dev/null 2>&1 || {
	echo
	echo "Error: You must have 'autoheader' installed on your system in"
	echo "  order to generate a configure script."
	echo "  (Note that 'autoheader' is part of the 'autoconf' package)"
	echo
	exit 1
}	

(libtool --version) < /dev/null > /dev/null 2>&1 || {
	echo
	echo "Error: You must have 'libtool' installed on your system in order"
	echo "  to generate a configure script."
	echo 
	exit 1
}

(automake --version) < /dev/null > /dev/null 2>&1 || {
	echo
	echo "Error: You must have 'automake' installed on your system in order"
	echo "  to generate a configure script."
	echo
	exit 1
}	

(aclocal --version) < /dev/null > /dev/null 2>&1 || {
	echo
	echo "Error: You must have 'aclocal' installed on your system in order"
	echo "  to generate a configure script."
	echo "  (Note that 'aclocal' is part of the 'automake' package)"
	echo
	exit 1
}


srcdir=`dirname $0`

echo "Running libtoolize..."
libtoolize --copy --force --automake

echo "Running aclocal..."
aclocal

echo "Running autoheader..."
autoheader

echo "Running automake..."
automake --add-missing --gnu --include-deps

echo "Running autoconf..."
autoconf

echo 
echo "Done!"
echo "Now run $srcdir/configure in order to create Makefiles."
echo
