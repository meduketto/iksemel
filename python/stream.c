/* iksemel (XML parser for Jabber)
** Copyright (C) 2011 Gurer Ozen
** This code is free software; you can redistribute it and/or
** modify it under the terms of GNU Lesser General Public License.
*/

#include "exceptions.h"
#include "document.h"
#include "stream.h"

/*** Stream ***/

static PyMethodDef Stream_methods[] = {
	{ "connect", (PyCFunction) Stream_connect, METH_KEYWORDS, "Connect stream to an XMPP server" },
	{ "fileno", (PyCFunction) Stream_fileno, METH_NOARGS, "Return file descriptor of the socket" },
	{ "send", (PyCFunction) Stream_send, METH_VARARGS, "Send stanzas to server" },
	{ "recv", (PyCFunction) Stream_recv, METH_NOARGS, "Read data from server" },
	{ NULL }
};

static PyTypeObject Stream_type = {
	PyVarObject_HEAD_INIT(NULL,0)
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

static void
start_sasl(Stream *self, enum ikssasltype type)
{
	PyObject *hook;
	PyObject *ret;
	PyObject *o;
	char *local;
	char *pw;

	o = PyObject_GetAttrString(self->jid, "local");
	if (!o) return;
	local = PyBytes_AsString(o);
	if (!local) {
		Py_DECREF(o);
		return;
	}

	hook = PyObject_GetAttrString((PyObject *) self, "ask_password");
	if (!hook) {
		Py_DECREF(o);
		return;
	}
	ret = PyObject_CallObject(hook, NULL);
	Py_DECREF(hook);
	if (!ret) {
		Py_DECREF(o);
		return;
	}
	pw = PyBytes_AsString(ret);
	if (!pw) {
		Py_DECREF(ret);
		Py_DECREF(o);
		return;
	}

	iks_start_sasl(self->parser, type, local, pw);

	Py_DECREF(ret);
	Py_DECREF(o);
}

static void
make_bind(Stream *self)
{
	iks *x, *y, *z;
	PyObject *o;
	char *resource;

	o = PyObject_GetAttrString(self->jid, "resource");
	if (!o) return;
	resource = PyBytes_AsString(o);
	if (!resource) {
		PyErr_Clear();
		resource = "iksemel";
	}

	x = iks_new("iq");
	iks_insert_attrib(x, "type", "set");
	y = iks_insert(x, "bind");
	iks_insert_attrib(y, "xmlns", IKS_NS_XMPP_BIND);
	z = iks_insert(y, "resource");
	iks_insert_cdata(z, resource, 0);

	iks_send(self->parser, x);

	iks_delete(x);
	Py_DECREF(o);
}

static void
on_features(Stream *self, iks *node)
{
	iks *x;

	self->features = iks_stream_features(node);
	if (self->use_sasl) {
		if (self->use_tls && !iks_is_secure(self->parser)) return;
		if (self->authorized) {
			if (self->features & IKS_STREAM_BIND) {
				make_bind(self);
			}
			if (self->features & IKS_STREAM_SESSION) {
				x = iks_make_session();
				iks_insert_attrib(x, "id", "auth");
				iks_send(self->parser, x);
				iks_delete(x);
			}
		} else {
			if (self->features & IKS_STREAM_SASL_MD5)
				start_sasl(self, IKS_SASL_DIGEST_MD5);
			else if (self->features & IKS_STREAM_SASL_PLAIN)
				start_sasl(self, IKS_SASL_PLAIN);
		}
	}
}

static void
on_success(Stream *self, iks *node)
{
	PyObject *o;
	char *domain;

	o = PyObject_GetAttrString(self->jid, "domain");
	if (!o) return;
	domain = PyBytes_AsString(o);
	if (!domain) {
		Py_DECREF(o);
		return;
	}

	self->authorized = 1;
	iks_send_header(self->parser, domain);

	Py_DECREF(o);
}

static int
on_stream(Stream *self, int type, iks *node)
{
	PyObject *hook;
	PyObject *doc;
	PyObject *ret;
//	iks *x;

	switch (type) {
		case IKS_NODE_NORMAL:
			if (strcmp("stream:features", iks_name(node)) == 0) {
				on_features(self, node);
			} else if (strcmp("success", iks_name(node)) == 0) {
				on_success(self, node);
			}
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
#endif
	hook = PyObject_GetAttrString((PyObject *) self, "on_stanza");
	if (hook) {
		doc = Document_parseString(NULL, node);
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

void
on_log(Stream *self, const char *data, size_t size, int is_incoming)
{
	PyObject *hook;
	PyObject *args;
	PyObject *ret;
	PyObject *b;

	hook = PyObject_GetAttrString((PyObject *) self, "on_xml");
	if (hook) {
		b = Py_False;
		if (is_incoming) b = Py_True;
		args = Py_BuildValue("s#O", data, size, b);
		if (args) {
			ret = PyObject_CallObject(hook, args);
			if (ret) { Py_DECREF(ret); }
			Py_DECREF(args);
		}
		Py_DECREF(hook);
	}
}

static int
Stream_init(Stream *self, PyObject *args, PyObject *kwargs)
{
	PyObject *hook;

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

	hook = PyObject_GetAttrString((PyObject *) self, "on_xml");
	if (hook) {
		iks_set_log_hook(self->parser, (iksLogHook *) on_log);
		Py_DECREF(hook);
	}

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
	PyErr_Clear();

	o = PyMapping_GetItemString(kwargs, "sasl");
	if (o) {
		if (PyObject_IsTrue(o)) {
			self->use_sasl = 1;
		} else {
			self->use_sasl = 0;
		}
	}
	PyErr_Clear();

	o = PyMapping_GetItemString(kwargs, "jid");
	if (!o) {
		PyErr_SetString(PyExc_TypeError, "jid keyword argument is required");
		return NULL;
	}
	Py_INCREF(o);
	self->jid = o;

	o = PyObject_GetAttrString(self->jid, "domain");
	if (!o) return NULL;
	host = PyBytes_AsString(o);
	if (!host) {
		Py_DECREF(o);
		return NULL;
	}

	e = iks_connect_tcp(self->parser, host, IKS_JABBER_PORT);
	if (e) {
		return exceptions_stream(e);
	}

	Py_DECREF(o);

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
Stream_fileno(Stream *self)
{
	return Py_BuildValue("i", iks_fd(self->parser));
}

static PyObject *
Stream_send(Stream *self, PyObject *args)
{
	int e;
	PyObject *s;
	char *str;

	if (!PyArg_ParseTuple(args, "O", &s))
		return NULL;

	s = PyObject_Str(s);
	if (s) {
		str = PyBytes_AsString(s);
		if (str) {
			e = iks_send_raw(self->parser, str);
			if (e) {
				Py_DECREF(s);
				return exceptions_stream(e);
			}
		}
		Py_DECREF(s);
	}

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
Stream_recv(Stream *self)
{
	int e;
	e = iks_recv(self->parser, 0);
	if (e) {
		return exceptions_stream(e);
	}

	Py_INCREF(Py_None);
	return Py_None;
}

static void
Stream_dealloc(Stream *self)
{
	if (self->jid) { Py_DECREF(self->jid); }
	iks_parser_delete(self->parser);
	PyTypeObject* ob_type(PyObject *self);
}

void
Setup_Stream(PyObject *module)
{
	Stream_type.tp_new = PyType_GenericNew;
	if (PyType_Ready(&Stream_type) < 0) return;
	Py_INCREF(&Stream_type);

	PyModule_AddObject(module, "Stream", (PyObject *) &Stream_type);
}
