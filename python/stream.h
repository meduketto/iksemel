/* iksemel (XML parser for Jabber)
** Copyright (C) 2011 Gurer Ozen
** This code is free software; you can redistribute it and/or
** modify it under the terms of GNU Lesser General Public License.
*/

#ifndef STREAM_H
#define STREAM_H 1

#include <Python.h>
#include "iksemel.h"

/*** Types ***/
typedef struct {
	PyObject_HEAD
	PyObject *jid;
	iksparser *parser;
	int features;
	int authorized;
	int use_sasl;
	int use_tls;
} Stream;

static PyTypeObject Stream_type;
static PyObject *Stream_connect(Stream *self, PyObject *args, PyObject *kwargs);
static PyObject *Stream_fileno(Stream *self);
static PyObject *Stream_send(Stream *self, PyObject *args);
static PyObject *Stream_recv(Stream *self);
static void Stream_dealloc(Stream *self);

static int Stream_init(Stream *self, PyObject *args, PyObject *kwargs);
/*** Setup ***/
void Setup_Stream(PyObject *module);


#endif  /* STREAM_H */
