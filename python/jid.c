/* iksemel (XML parser for Jabber)
** Copyright (C) 2011 Gurer Ozen
** This code is free software; you can redistribute it and/or
** modify it under the terms of GNU Lesser General Public License.
*/

#include "jid.h"
#include <structmember.h>

// NOTE: This class uses custom implementation of JID parsing
// instead of the iks_id_new from jabber.c
//

static PyMemberDef JID_members[] = {
	{ "local", T_OBJECT_EX, offsetof(JID, local), 0, "localpart" },
	{ "domain", T_OBJECT_EX, offsetof(JID, domain), 0, "domainpart" },
	{ "resource", T_OBJECT_EX, offsetof(JID, resource), 0, "resourcepart" },
	{ NULL }
};

static PyTypeObject JID_type = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"iksemel.JID",		/* tp_name */
	sizeof(JID),		/* tp_basicsize */
	0,			/* tp_itemsize */
	(destructor)JID_dealloc, /* tp_dealloc */
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
	(reprfunc)JID_str,	/* tp_str */
	0,			/* tp_getattro */
	0,			/* tp_setattro */
	0,			/* tp_as_buffer */
	Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /* tp_flags */
	"iksemel JID object", /* tp_doc */
	0,			/* tp_traverse */
	0,			/* tp_clear */
	0,			/* tp_richcompare */
	0,			/* tp_weaklistoffset */
	0,			/* tp_iter */
	0,			/* tp_iternext */
	0,			/* tp_methods */
	JID_members,		/* tp_members */
	0,			/* tp_getset */
	0,			/* tp_base */
	0,			/* tp_dict */
	0,			/* tp_descr_get */
	0,			/* tp_descr_set */
	0,			/* tp_dictoffset */
	(initproc)JID_init,	/* tp_init */
	0,			/* tp_alloc */
	0			/* tp_new */
};

static int
JID_init(JID *self, PyObject *args, PyObject *kwargs)
{
	char *str;
	char *d;
	char *r;

	self->local = NULL;
	self->domain = NULL;
	self->resource = NULL;

	if (!PyArg_ParseTuple(args, "s", &str))
		return -1;

	/* skip scheme */
	if (strncmp("jabber:", str, 7) == 0) str += 7;
	if (strncmp("xmpp:", str, 5) == 0) str += 5;

	d = strchr (str, '@');
	r = strchr(str, '/');
	if (d) {
		self->local = Py_BuildValue("s#", str, d - str);
		str = d + 1;
	} else {
		Py_INCREF(Py_None);
		self->local = Py_None;
	}
	if (r) {
		self->resource = Py_BuildValue("s", r + 1);
		self->domain = Py_BuildValue("s#", str, r - str);
	} else {
		Py_INCREF(Py_None);
		self->resource = Py_None;
		self->domain = Py_BuildValue("s", str);
	}

	return 0;
}

static PyObject *
JID_str(JID *self)
{
	char *local = "";
	char *localsep = "";
	char *domain;
	char *resourcesep = "";
	char *resource = "";

	if (self->local && self->local != Py_None) {
		local = PyBytes_AsString(self->local);
		if (!local) return NULL;
		localsep = "@";
	}
	domain = PyBytes_AsString(self->domain);
	if (!domain) return NULL;
	if (self->resource && self->resource != Py_None) {
		resource = PyBytes_AsString(self->resource);
		if (!resource) return NULL;
		resourcesep = "/";
	}
	return PyBytes_FromFormat("%s%s%s%s%s", local, localsep, domain, resourcesep, resource);
}

static void
JID_dealloc(JID *self)
{
	if (self->local) { Py_DECREF(self->local); }
	if (self->domain) { Py_DECREF(self->domain); }
	if (self->resource) { Py_DECREF(self->resource); }
	PyTypeObject* ob_type(PyObject *self);
}

void
Setup_JID(PyObject *module)
{
	JID_type.tp_new = PyType_GenericNew;
	if (PyType_Ready(&JID_type) < 0) return;
	Py_INCREF(&JID_type);

	PyModule_AddObject(module, "JID", (PyObject *) &JID_type);
}
