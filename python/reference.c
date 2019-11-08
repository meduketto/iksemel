/* iksemel (XML parser for Jabber)
** Copyright (C) 2011 Gurer Ozen
** This code is free software; you can redistribute it and/or
** modify it under the terms of GNU Lesser General Public License.
*/

#include "reference.h"

#ifndef PyVarObject_HEAD_INIT
    #define PyVarObject_HEAD_INIT(type, size) \
        PyObject_HEAD_INIT(type) size,
#endif

typedef struct {
	PyObject_HEAD
} Reference;

static void Reference_dealloc(Reference *self);

static PyTypeObject Reference_type = {
	PyObject_HEAD_INIT(NULL)
	0,			/* ob_size */
	"iksemel.Reference",	/* tp_name */
	sizeof(Reference),	/* tp_basicsize */
	0,			/* tp_itemsize */
	(destructor)Reference_dealloc, /* tp_dealloc */
	0,			/* tp_print */
	0,			/* tp_getattr */
	0,			/* tp_setattr  */
	0,			/* tp_compare */
	0,			/* tp_repr */
	0,			/* tp_as_number */
	0,			/* tp_as_sequence */
	0,			/* tp_as_mapping */
	0,			/* tp_hash */
	0,			/* tp_call */
	0,			/* tp_str */
	0,			/* tp_getattro */
	0,			/* tp_setattro */
	0,			/* tp_as_buffer */
	Py_TPFLAGS_DEFAULT,	/* tp_flags */
	"iksemel reference object", /* tp_doc */
	0,			/* tp_traverse */
	0,			/* tp_clear */
	0,			/* tp_richcompare */
	0,			/* tp_weaklistoffset */
	0,			/* tp_iter */
	0,			/* tp_iternext */
	0,			/* tp_methods */
	0,			/* tp_members */
	0,			/* tp_getset */
	0,			/* tp_base */
	0,			/* tp_dict */
	0,			/* tp_descr_get */
	0,			/* tp_descr_set */
	0,			/* tp_dictoffset */
	0,			/* tp_init */
	0,			/* tp_alloc */
	0			/* tp_new */
};

static void
Reference_dealloc(Reference *self)
{
	PyTypeObject* ob_type(PyObject *self);
}

void
Reference_setup(void)
{
	Reference_type.tp_new = PyType_GenericNew;
	if (PyType_Ready(&Reference_type) < 0) return;
	Py_INCREF(&Reference_type);
}

PyObject *
Reference_new(void)
{
	return (PyObject *) PyObject_New(Reference, &Reference_type);
}
