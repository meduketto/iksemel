/* iksemel (XML parser for Jabber)
** Copyright (C) 2011 Gurer Ozen
** This code is free software; you can redistribute it and/or
** modify it under the terms of GNU Lesser General Public License.
*/

#include "document.h"
#include "stream.h"

typedef struct {
	PyObject_HEAD
	iksparser *parser;
} Stream;

static int Stream_init(Stream *self, PyObject *args, PyObject *kwargs);
static PyObject *Stream_connect(Stream *self, PyObject *args, PyObject *kwargs);
static void Stream_dealloc(Stream *self);

static PyMethodDef Stream_methods[] = {
	{ "connect", (PyCFunction) Stream_connect, METH_KEYWORDS, "Connect stream to an XMPP server" },
	{ NULL }
};

static PyTypeObject Stream_type = {
	PyObject_HEAD_INIT(NULL)
	0,			/* ob_size */
	"iksemel.Stream",	/* tp_name */
	sizeof(Stream),		/* tp_basicsize */
	0,			/* tp_itemsize */
	(destructor)Stream_dealloc, /* tp_dealloc */
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
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /* tp_flags */
	"iksemel stream object", /* tp_doc */
	0,			/* tp_traverse */
	0,			/* tp_clear */
	0,			/* tp_richcompare */
	0,			/* tp_weaklistoffset */
	0,			/* tp_iter */
	0,			/* tp_iternext */
	Stream_methods,		/* tp_methods */
	0,			/* tp_members */
	0,			/* tp_getset */
	0,			/* tp_base */
	0,			/* tp_dict */
	0,			/* tp_descr_get */
	0,			/* tp_descr_set */
	0,			/* tp_dictoffset */
	(initproc)Stream_init,	/* tp_init */
	0,			/* tp_alloc */
	0			/* tp_new */
};

static int
on_stream(Stream *self, int type, iks *node)
{
	PyObject *hook;
	PyObject *doc;
	PyObject *ret;

	// FIXME: handle different types

	hook = PyObject_GetAttrString((PyObject *) self, "on_stanza");
	if (hook) {
		doc = Document_from_iks(NULL, node);
		if (!doc) {
			Py_DECREF(hook);
			return IKS_NOMEM;
		}
		ret = PyObject_CallFunctionObjArgs(hook, doc, NULL);
		Py_DECREF(hook);
		// FIXME: handle error
		if (ret == NULL) {
			Py_DECREF(ret);
			return IKS_HOOK;
		}
	}
	return IKS_OK;
}

static int
Stream_init(Stream *self, PyObject *args, PyObject *kwargs)
{
	self->parser = iks_stream_new (
		IKS_NS_CLIENT,
		self,
		(iksStreamHook *) on_stream
	);
	return 0;
}

static PyObject *
Stream_connect(Stream *self, PyObject *args, PyObject *kwargs)
{
	// FIXME: simple test for now, should initiate connection instead
	on_stream(self,0,iks_new("lala"));

	Py_INCREF(Py_None);
	return Py_None;
}

static void
Stream_dealloc(Stream *self)
{
	iks_parser_delete(self->parser);
	self->ob_type->tp_free((PyObject *) self);
}

void
Stream_setup(PyObject *module)
{
	Stream_type.tp_new = PyType_GenericNew;
	if (PyType_Ready(&Stream_type) < 0) return;
	Py_INCREF(&Stream_type);

	PyModule_AddObject(module, "Stream", (PyObject *) &Stream_type);
}
