/* iksemel (XML parser for Jabber)
** Copyright (C) 2011 Gurer Ozen
** This code is free software; you can redistribute it and/or
** modify it under the terms of GNU Lesser General Public License.
*/

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H 1

#include <Python.h>

/*** Objects ***/

static PyObject *ParseError;
static PyObject *NotTag;
static PyObject *NotData;
static PyObject *NotStream;

/*** Exceptions ***/

PyObject *exceptions_parse(void);
PyObject *exceptions_stream(int e);
PyObject *exceptions_object(int e);

/*** Setup ***/
void Setup_Exception(PyObject *module);

#endif  /* EXCEPTIONS_H */
