/* iksemel (XML parser for Jabber)
** Copyright (C) 2011 Gurer Ozen
** This code is free software; you can redistribute it and/or
** modify it under the terms of GNU Lesser General Public License.
*/

#include <Python.h>
#include "iksemel.h"

PyObject *iksemel_module;

static PyMethodDef methods[] = {
	{ NULL, NULL, 0, NULL }
};

PyMODINIT_FUNC
initiksemel(void)
{
	PyObject *m;

	m = Py_InitModule("iksemel", methods);
	iksemel_module = m;
}
