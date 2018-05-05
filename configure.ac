dnl Process this file with autoconf to produce a configure script.
AC_INIT
AC_PREREQ(2.50)
AC_CONFIG_SRCDIR([configure.ac])
AM_INIT_AUTOMAKE(iksemel,1.5)
AM_CONFIG_HEADER(include/config.h)

AC_CANONICAL_HOST

dnl Checks for programs
AC_PROG_CC
AC_PROG_INSTALL
AC_CHECK_TOOL(OBJDUMP, objdump, :)
AM_PROG_LIBTOOL
AC_LANG_C
PKG_PROG_PKG_CONFIG

dnl Checks for header files
AC_HEADER_STDC
AC_CHECK_HEADERS(unistd.h strings.h errno.h)

dnl Checks for typedefs and structures
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
AC_CHECK_MEMBERS([struct stat.st_blksize])

dnl Special check for windows
case "$host_os" in
	*mingw*)
		AC_CHECK_LIB(wsock32, main, , AC_MSG_ERROR(Winsock is needed for a Win32 build.))
		;;
esac

dnl Check for posix io
AC_ARG_ENABLE(default-io,
  AS_HELP_STRING([--disable-default-io],[disable builtin socket transport code]),
  defio=$enableval, defio=yes)
if test "$defio" = yes; then
  AC_DEFINE(USE_DEFAULT_IO,1,[Define this is you want default transport])
fi
AM_CONDITIONAL(DO_POSIX, test "x$defio" = "xyes")

dnl Checks for library functions
AC_SEARCH_LIBS(recv,socket)
AC_CHECK_FUNCS(getopt_long)
AC_CHECK_FUNCS(getaddrinfo)

dnl Check -Wall flag of GCC
if test "x$GCC" = "xyes"; then
  if test -z "`echo "$CFLAGS" | grep "\-Wall" 2> /dev/null`" ; then
    CFLAGS="$CFLAGS -Wall"
  fi
fi

dnl Options for overriding TLS checks
AC_ARG_WITH([openssl],
  AS_HELP_STRING([--without-openssl],[disable checking for openssl]),
  [with_openssl=$enableval],
  [with_openssl=yes]
)
AC_ARG_WITH([gnutls],
  AS_HELP_STRING([--without-gnutls],[disable checking for GNU TLS]),
  [with_gnutls=$enableval],
  [with_gnutls=yes]
)

dnl Check OpenSSL
if test "x$with_openssl" = "xyes"; then
  PKG_CHECK_MODULES([OPENSSL], openssl >= 0.9.8, have_openssl=yes, have_openssl=no)
  if test "x$have_openssl" = "xyes"; then
    LIBOPENSSL_CFLAGS="$OPENSSL_CFLAGS"
    LIBOPENSSL_LIBS="$OPENSSL_LIBS"
    AC_SUBST(LIBOPENSSL_CFLAGS)
    AC_SUBST(LIBOPENSSL_LIBS)
    AC_DEFINE(HAVE_OPENSSL, 1, [whether to use OpenSSL support.])
  fi
fi

dnl Check GNU TLS
if test "x$with_gnutls" = "xyes"; then
  if test "x$have_openssl" != "xyes"; then
    PKG_CHECK_MODULES([GNUTLS], gnutls >= 2.0.0, have_gnutls=yes, have_gnutls=no)
    if test "x$have_gnutls" = "xyes"; then
      LIBGNUTLS_CFLAGS="$GNUTLS_CFLAGS"
      LIBGNUTLS_LIBS="$GNUTLS_LIBS"
      AC_SUBST(LIBGNUTLS_CFLAGS)
      AC_SUBST(LIBGNUTLS_LIBS)
      AC_DEFINE(HAVE_GNUTLS, 1, [whether to use GnuTSL support.])
    fi
  fi
fi

dnl Option for overriding Python check
AC_ARG_ENABLE([python],
  AS_HELP_STRING([--disable-python],[disable checking for Python bindings]),
  [with_python=$enableval],
  [with_python=yes]
)

dnl a macro to check for ability to create python extensions
dnl  AM_CHECK_PYTHON_HEADERS([ACTION-IF-POSSIBLE], [ACTION-IF-NOT-POSSIBLE])
dnl function also defines PYTHON_INCLUDES
AC_DEFUN([AM_CHECK_PYTHON_HEADERS],
[AC_REQUIRE([AM_PATH_PYTHON])
AC_MSG_CHECKING(for headers required to compile python extensions)
dnl deduce PYTHON_INCLUDES
py_prefix=`$PYTHON -c "import sys; print(sys.prefix)"`
py_exec_prefix=`$PYTHON -c "import sys; print(sys.exec_prefix)"`
PYTHON_INCLUDES="-I${py_prefix}/include/python${PYTHON_VERSION}"
if test "$py_prefix" != "$py_exec_prefix"; then
  PYTHON_INCLUDES="$PYTHON_INCLUDES -I${py_exec_prefix}/include/python${PYTHON_VERSION}"
fi
AC_SUBST(PYTHON_INCLUDES)
dnl check if the headers exist:
save_CPPFLAGS="$CPPFLAGS"
CPPFLAGS="$CPPFLAGS $PYTHON_INCLUDES"
AC_TRY_CPP([#include <Python.h>],dnl
[AC_MSG_RESULT(found)
$1],dnl
[AC_MSG_RESULT(not found)
$2])
CPPFLAGS="$save_CPPFLAGS"
])

dnl Check Python for binding
if test "x$with_python" = "xyes"; then
  with_python=no
  AM_PATH_PYTHON([3.2], , [:])
  AM_CHECK_PYTHON_HEADERS(with_python=yes,[AC_MSG_ERROR(could not find Python headers)])
fi
AM_CONDITIONAL([DO_PYTHON], [test "x$with_python" = "xyes"])

dnl Generating makefiles
AC_CONFIG_FILES([
Makefile
iksemel.pc
src/Makefile
include/Makefile
tools/Makefile
python/Makefile
test/Makefile
doc/Makefile
])
AC_OUTPUT
