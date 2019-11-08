/* iksemel (XML parser for Jabber)
** Copyright (C) 2011 Gurer Ozen
** This code is free software; you can redistribute it and/or
** modify it under the terms of GNU Lesser General Public License.
*/

#include "exceptions.h"
#include "reference.h"
#include "document.h"
#include "jid.h"
#include "stream.h"
#include "iksemel.h"

PyObject *iksemel_module;

/*static PyMethodDef methods[] = {
	{ "parse", ciks_parse, METH_VARARGS,
	  "Parse given XML file and generate document tree."},
	{ "parseString", ciks_parseString, METH_VARARGS,
	  "Parse given XML string and generate document tree."},
	{ "newDocument", ciks_newDocument, METH_VARARGS,
	  "Create a new document with given root tag name."},
	{ NULL, NULL, 0, NULL }
};*/

static struct PyModuleDef iksemeldef = {
    PyModuleDef_HEAD_INIT,
    "iksemel",     /* m_name */
    "This is a module",  /* m_doc */
    -1,                  /* m_size */
    NULL,                /* m_methods */
    NULL,                /* m_reload */
    NULL,                /* m_traverse */
    NULL,                /* m_clear */
    NULL,                /* m_free */
};


PyMODINIT_FUNC
PyInit_iksemel(void)
{
	PyObject *m;

	m = PyModule_Create(&iksemeldef);

	exceptions_setup(m);
	Reference_setup();
	Document_setup(m);
	JID_setup(m);
	Stream_setup(m);

	iksemel_module = m;
  return m;
}
