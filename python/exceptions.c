/* iksemel (XML parser for Jabber)
** Copyright (C) 2011 Gurer Ozen
** This code is free software; you can redistribute it and/or
** modify it under the terms of GNU Lesser General Public License.
*/

#include "exceptions.h"

static PyObject *parse_exc;

void
exceptions_setup(PyObject *module)
{
	parse_exc = PyErr_NewException("iksemel.ParseError", NULL, NULL);
	Py_INCREF(parse_exc);
	PyModule_AddObject(module, "ParseError", parse_exc);
}

// FIXME: provide err, lineno, etc
PyObject *
exceptions_parse_error(void)
{
	PyErr_SetNone(parse_exc);
	return NULL;
}
