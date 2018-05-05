/* iksemel (XML parser for Jabber)
** Copyright (C) 2011 Gurer Ozen
** This code is free software; you can redistribute it and/or
** modify it under the terms of GNU Lesser General Public License.
*/

/*
** Copyright (c) 2016 Suleyman Poyraz
**
** This program is free software; you can redistribute it and/or modify it
** under the terms of the GNU General Public License as published by the
** Free Software Foundation; either version 3 of the License, or (at your
** option) any later version. Please read the COPYING file.
*/

#include <Python.h>
#include "iksemel.h"
#include "document.h"
#include "exceptions.h"
#include "node.h"
#include "stream.h"
#include "jid.h"

PyObject *iksemel_module;

/*** Module Functions ***/


static PyMethodDef methods[] = {
	{ "parse", Document_parse, METH_VARARGS,
	  "Parse given XML file and generate document tree."},
	{ "parseString", Document_parseString, METH_VARARGS,
	  "Parse given XML string and generate document tree."},
	{ "newDocument", Document_newDocument, METH_VARARGS,
	  "Create a new document with given root tag name."},
	{ NULL, NULL, 0, NULL }
};

static struct PyModuleDef iksemelmodule ={
	PyModuleDef_HEAD_INIT,
	"iksemel",
	NULL,
	-1,
	methods
};

__attribute__((visibility("default")))
PyMODINIT_FUNC
PyInit_iksemel(void)
{
	PyObject *m;

	m = PyModule_Create(&iksemelmodule);
	Setup_Document(m);
	Setup_Exception(m);
	Setup_Node(m);
	Setup_Stream(m);
	Setup_JID(m);
	iksemel_module = m;
	return m;
}
