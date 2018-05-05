/* iksemel (XML parser for Jabber)
** Copyright (C) 2011 Gurer Ozen
** This code is free software; you can redistribute it and/or
** modify it under the terms of GNU Lesser General Public License.
*/

#include "exceptions.h"

void
Setup_Exception(PyObject *module)
{
	ParseError = PyErr_NewException("iksemel.ParseError", NULL, NULL);
	Py_INCREF(ParseError);
	PyModule_AddObject(module, "ParseError", ParseError);
	NotTag = PyErr_NewException("iksemel.NotTag", NULL, NULL);
	Py_INCREF(NotTag);
	PyModule_AddObject(module, "NotTag", NotTag);
	NotData = PyErr_NewException("iksemel.NotData", NULL, NULL);
	Py_INCREF(NotData);
	PyModule_AddObject(module, "NotData", NotData);
	NotStream = PyErr_NewException("iksemel.NotStream", NULL, NULL);
	Py_INCREF(NotStream);
	PyModule_AddObject(module, "NotStream", NotStream);
}

// FIXME: provide err, lineno, etc
PyObject *
exceptions_parse(void)
{
	PyErr_SetNone(ParseError);
	return NULL;
}

// FIXME: more info on error type
PyObject *
exceptions_object_notdata(int e)
{
	PyErr_SetNone(NotData);
	return NULL;
}

PyObject *
exceptions_object_nottag(int e)
{
	PyErr_SetNone(NotTag);
	return NULL;
}

PyObject *
exceptions_stream(int e)
{
	PyErr_SetNone(NotStream);
	return NULL;
}
