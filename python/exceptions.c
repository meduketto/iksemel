/* iksemel (XML parser for Jabber)
** Copyright (C) 2011 Gurer Ozen
** This code is free software; you can redistribute it and/or
** modify it under the terms of GNU Lesser General Public License.
*/

#include "exceptions.h"

static PyObject *parse_exc;
static PyObject *stream_exc;

void
exceptions_setup(PyObject *module)
{
	parse_exc = PyErr_NewException("iksemel.ParseError", NULL, NULL);
	Py_INCREF(parse_exc);
	PyModule_AddObject(module, "ParseError", parse_exc);

	stream_exc = PyErr_NewException("iksemel.StreamError", NULL, NULL);
	Py_INCREF(stream_exc);
	PyModule_AddObject(module, "StreamError", stream_exc);
}

// FIXME: provide err, lineno, etc
PyObject *
exceptions_parse_error(void)
{
	PyErr_SetNone(parse_exc);
	return NULL;
}

// FIXME: more info on error type
PyObject *
exceptions_stream_error(int e)
{
	PyErr_SetNone(stream_exc);
	return NULL;
}
