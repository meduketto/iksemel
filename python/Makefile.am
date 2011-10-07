##
## Process this file with automake to produce Makefile.in
##

INCLUDES = -I$(top_srcdir)/include $(PYTHON_INCLUDES)

pyexec_LTLIBRARIES = iksemel.la

iksemel_la_LIBADD = $(top_builddir)/src/libiksemel.la
iksemel_la_SOURCES = xml.c
iksemel_la_LDFLAGS = -avoid-version -module -export-symbols-regex initiksemel
