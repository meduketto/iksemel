/* iksemel (XML parser for Jabber)
** Copyright (C) 2011 Gurer Ozen
** This code is free software; you can redistribute it and/or
** modify it under the terms of GNU Lesser General Public License.
*/

#include "document.h"
#include "stream.h"

typedef struct {
	PyObject_HEAD
	PyObject *jid;
	iksparser *parser;
	int features;
	int authorized;
	int use_sasl;
	int use_tls;
} Stream;

static int Stream_init(Stream *self, PyObject *args, PyObject *kwargs);
static PyObject *Stream_connect(Stream *self, PyObject *args, PyObject *kwargs);
static PyObject *Stream_recv(Stream *self);
static void Stream_dealloc(Stream *self);

static PyMethodDef Stream_methods[] = {
	{ "connect", (PyCFunction) Stream_connect, METH_KEYWORDS, "Connect stream to an XMPP server" },
	{ "recv", (PyCFunction) Stream_recv, METH_NOARGS, "Read data from server" },
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
	iks *x;

	switch (type) {
#if 0
		case IKS_NODE_START:
			if (!iks_is_secure(self->parser)) {
				iks_start_tls(self->parser);
				break;
			}
			if (!self->use_sasl) {
				x = iks_make_auth (, , NULL);
				iks_insert_attrib (x, "id", "auth");
				iks_send (self->parser, x);
				iks_delete (x);
			}
			break;
		case IKS_NODE_NORMAL:
			if (strcmp ("stream:features", iks_name (node)) == 0) {
				self->features = iks_stream_features (node);
				if (self->use_sasl) {
					if (self->use_tls && !iks_is_secure (self->parser)) break;
					if (self->authorized) {
						iks *t;
						if (self->features & IKS_STREAM_BIND) {
							t = iks_make_resource_bind (sess->acc);
							iks_send (self->parser, t);
							iks_delete (t);
						}
						if (self->features & IKS_STREAM_SESSION) {
							t = iks_make_session ();
							iks_insert_attrib (t, "id", "auth");
							iks_send (self->parser, t);
							iks_delete (t);
						}
					} else {
						if (self->features & IKS_STREAM_SASL_MD5)
							iks_start_sasl (self->prs, IKS_SASL_DIGEST_MD5, sess->acc->user, sess->pass);
						else if (self->features & IKS_STREAM_SASL_PLAIN)
							iks_start_sasl (self->prs, IKS_SASL_PLAIN, sess->acc->user, sess->pass);
					}
				}
#endif
		case IKS_NODE_NORMAL:
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
	}
	if (node) iks_delete (node);
	return IKS_OK;
}

static int
Stream_init(Stream *self, PyObject *args, PyObject *kwargs)
{
	self->jid = NULL;
	self->parser = iks_stream_new (
		IKS_NS_CLIENT,
		self,
		(iksStreamHook *) on_stream
	);
	self->features = 0;
	self->authorized = 0;
	self->use_sasl = 1;
	self->use_tls = 1;
	return 0;
}

static PyObject *
Stream_connect(Stream *self, PyObject *args, PyObject *kwargs)
{
	PyObject *o;
	char *host;
	int e;

	o = PyMapping_GetItemString(kwargs, "tls");
	if (o) {
		if (PyObject_IsTrue(o)) {
			self->use_tls = 1;
		} else {
			self->use_tls = 0;
		}
	}

	o = PyMapping_GetItemString(kwargs, "sasl");
	if (o) {
		if (PyObject_IsTrue(o)) {
			self->use_sasl = 1;
		} else {
			self->use_sasl = 0;
		}
	}

	o = PyMapping_GetItemString(kwargs, "jid");
	if (!o) {
		// FIXME: except
		return NULL;
	}
	Py_INCREF(o);
	self->jid = o;

	o = PyObject_GetAttrString(self->jid, "domain");
	if (!o) return NULL;
	host = PyString_AsString(o);
	if (!host) {
		Py_DECREF(o);
		return NULL;
	}

	e = iks_connect_tcp(self->parser, host, IKS_JABBER_PORT);
	if (e) {
		// FIXME: throw exception
		return NULL;
	}

	Py_DECREF(o);

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
Stream_recv(Stream *self)
{
	int e;

	e = iks_recv(self->parser, 0);
	if (e) {
		// FIXME: throw exception
		return NULL;
	}

	Py_INCREF(Py_None);
	return Py_None;
}

static void
Stream_dealloc(Stream *self)
{
	if (self->jid) { Py_DECREF(self->jid); }
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
