/* iksemel (XML parser for Jabber)
** Copyright (C) 2011 Gurer Ozen
** This code is free software; you can redistribute it and/or
** modify it under the terms of GNU Lesser General Public License.
*/

#include "exceptions.h"
#include "reference.h"
#include "document.h"

typedef struct {
	PyObject_HEAD
	PyObject *ref;
	iks *doc;
	int tags;
	char *tagname;
} Iter;

static PyObject *Iter_iter(Iter *self);
static PyObject *Iter_next(Iter *self);

static PyTypeObject Iter_type = {
	PyObject_HEAD_INIT(NULL)
	0,			/* ob_size */
	"iksemel.Iter",		/* tp_name */
	sizeof(Iter),		/* tp_basicsize */
	0,			/* tp_itemsize */
	0,			/* tp_dealloc */
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
	"iksemel iter object",	/* tp_doc */
	0,			/* tp_traverse */
	0,			/* tp_clear */
	0,			/* tp_richcompare */
	0,			/* tp_weaklistoffset */
	(getiterfunc)Iter_iter,	/* tp_iter */
	(iternextfunc)Iter_next,/* tp_iternext */
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

static PyObject *
Iter_iter(Iter *self)
{
	Py_INCREF(self);
	return (PyObject *)self;
}

static PyObject *
Iter_next(Iter *self)
{
	iks *doc;

	doc = self->doc;
	if (!doc) return NULL;

	if (self->tags) {
		self->doc = iks_next_tag(doc);
		if (self->doc && self->tagname) {
			while (self->doc && (strcmp(self->tagname, iks_name(self->doc)) != 0)) {
				self->doc = iks_next_tag(self->doc);
			}
		}
	} else {
		self->doc = iks_next(doc);
	}

	return Document_from_iks(self->ref, doc);
}

/*
**
*/

typedef struct {
	PyObject_HEAD
	PyObject *ref;
	iks *doc;
} Document;

static int Document_init(Document *self, PyObject *args, PyObject *kwargs);
static PyObject *Document_str(Document *self);
static PyObject *Document_iter(Document *self);
static PyObject *Document_tags(Document *self, PyObject *args);
static PyObject *Document_child(Document *self);
static PyObject *Document_parent(Document *self);
static PyObject *Document_root(Document *self);
static PyObject *Document_next(Document *self);
static PyObject *Document_next_tag(Document *self, PyObject *args);
static PyObject *Document_prev(Document *self);
static PyObject *Document_prev_tag(Document *self, PyObject *args);
static void Document_dealloc(Document *self);

static PyMethodDef Document_methods[] = {
	{ "tags", (PyCFunction) Document_tags, METH_VARARGS, "Iterate over all or specified child tags" },
	{ "child", (PyCFunction) Document_child, METH_NOARGS, "Return first child node." },
	{ "parent", (PyCFunction) Document_parent, METH_NOARGS, "Return parent node." },
	{ "root", (PyCFunction) Document_root, METH_NOARGS, "Return topmost node." },
	{ "next", (PyCFunction) Document_next, METH_NOARGS, "Return next sibling node." },
	{ "next_tag", (PyCFunction) Document_next_tag, METH_VARARGS, "Return next sibling tag node." },
	{ "prev", (PyCFunction) Document_prev, METH_NOARGS, "Return previous sibling node." },
	{ "prev_tag", (PyCFunction) Document_prev_tag, METH_VARARGS, "Return previous sibling tag node." },
	{ NULL }
};

static PyTypeObject Document_type = {
	PyObject_HEAD_INIT(NULL)
	0,			/* ob_size */
	"iksemel.Document",	/* tp_name */
	sizeof(Document),	/* tp_basicsize */
	0,			/* tp_itemsize */
	(destructor)Document_dealloc, /* tp_dealloc */
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
	(reprfunc)Document_str,	/* tp_str */
	0,			/* tp_getattro */
	0,			/* tp_setattro */
	0,			/* tp_as_buffer */
	Py_TPFLAGS_DEFAULT,	/* tp_flags */
	"iksemel document object", /* tp_doc */
	0,			/* tp_traverse */
	0,			/* tp_clear */
	0,			/* tp_richcompare */
	0,			/* tp_weaklistoffset */
	(getiterfunc) Document_iter, /* tp_iter */
	0,			/* tp_iternext */
	Document_methods,	/* tp_methods */
	0,			/* tp_members */
	0,			/* tp_getset */
	0,			/* tp_base */
	0,			/* tp_dict */
	0,			/* tp_descr_get */
	0,			/* tp_descr_set */
	0,			/* tp_dictoffset */
	(initproc)Document_init, /* tp_init */
	0,			/* tp_alloc */
	0			/* tp_new */
};

static int
Document_init(Document *self, PyObject *args, PyObject *kwargs)
{
	int e;
	char *str;

	if (!PyArg_ParseTuple(args, "s", &str))
		return -1;

	self->doc = iks_tree(str, 0, &e);
	if (!self->doc) {
		if (e == IKS_NOMEM) {
			PyErr_NoMemory();
		} else {
			exceptions_parse_error();
		}
		return -1;
	}
	self->ref = Reference_new();
	if (!self->ref) {
		iks_delete(self->doc);
		PyErr_NoMemory();
		return -1;
	}

	return 0;
}

static PyObject *
Document_str(Document *self)
{
	PyObject *ret;
	char *str;

	str = iks_string(NULL, self->doc);
	if (!str) return PyErr_NoMemory();
	ret = Py_BuildValue("s", str);
	iks_free(str);

	return ret;
}

static PyObject *
Document_iter(Document *self)
{
	Iter *iter;

	// FIXME: tag error

	iter = PyObject_New(Iter, &Iter_type);
	iter->doc = iks_child(self->doc);
	iter->ref = self->ref;
	iter->tags = 0;
	iter->tagname = NULL;
	return (PyObject *) iter;
}

static PyObject *
Document_tags(Document *self, PyObject *args)
{
	Iter *iter;
	char *name = NULL;

	if (!PyArg_ParseTuple(args, "|s", &name))
		return NULL;

	if (iks_type(self->doc) != IKS_TAG) {
// FIXME: ?
//		PyErr_SetNone(NotTag);
		return NULL;
	}

	iter = PyObject_New(Iter, &Iter_type);
	iter->ref = self->ref;
	iter->doc = iks_first_tag(self->doc);
	if (name && iter->doc) {
		while (iter->doc && (strcmp(name, iks_name(iter->doc)) != 0)) {
			iter->doc = iks_next_tag(iter->doc);
		}
	}
	iter->tags = 1;
	iter->tagname = name;

	return (PyObject *)iter;
}

static PyObject *
move_to(Document *self, iks *dest)
{
	if (dest) {
		return Document_from_iks(self->ref, dest);
	}
	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
Document_child(Document *self)
{
	return move_to(self, iks_child(self->doc));
}

static PyObject *
Document_parent(Document *self)
{
	return move_to(self, iks_parent(self->doc));
}

static PyObject *
Document_root(Document *self)
{
	return move_to(self, iks_root(self->doc));
}

static PyObject *
Document_next(Document *self)
{
	return move_to(self, iks_next(self->doc));
}

static PyObject *
Document_next_tag(Document *self, PyObject *args)
{
	iks *x;
	char *name = NULL;

	if (!PyArg_ParseTuple(args, "|s", &name))
		return NULL;

	x = iks_next_tag (self->doc);
	if (name) {
		while (x && strcmp(iks_name(x), name) != 0) {
			x = iks_next_tag (x);
		}
	}
	return move_to(self, x);
}

static PyObject *
Document_prev(Document *self)
{
	return move_to(self, iks_prev(self->doc));
}

static PyObject *
Document_prev_tag(Document *self, PyObject *args)
{
	iks *x;
	char *name = NULL;

	if (!PyArg_ParseTuple(args, "|s", &name))
		return NULL;

	x = iks_prev_tag (self->doc);
	if (name) {
		while (x && strcmp(iks_name(x), name) != 0) {
			x = iks_prev_tag (x);
		}
	}
	return move_to(self, x);
}











static void
Document_dealloc(Document *self)
{
	if (self->ref && self->ref->ob_refcnt == 1) {
		iks_delete(self->doc);
	}
	Py_DECREF(self->ref);
	self->doc = NULL;
	self->ref = NULL;
	self->ob_type->tp_free((PyObject *) self);
}

void
Document_setup(PyObject *module)
{
	Iter_type.tp_new = PyType_GenericNew;
	if (PyType_Ready(&Iter_type) < 0) return;
	Py_INCREF(&Iter_type);

	Document_type.tp_new = PyType_GenericNew;
	if (PyType_Ready(&Document_type) < 0) return;
	Py_INCREF(&Document_type);

	PyModule_AddObject(module, "Document", (PyObject *) &Document_type);
}

PyObject *
Document_from_iks(PyObject *ref, iks *doc)
{
	Document *new_doc;

	new_doc = PyObject_New(Document, &Document_type);
	if (!new_doc) return NULL;

	if (ref) {
		// Take of ref on it to prevent XML tree from deletion
		// as long as new object exists.
		Py_INCREF(ref);
		new_doc->ref = ref;
	} else {
		// Create a reference object to keep track of how many
		// doc objects share this XML tree.
		new_doc->doc = NULL;
		new_doc->ref = Reference_new();
		if (!new_doc->ref) {
			PyErr_NoMemory();
			Py_DECREF(new_doc);
			return NULL;
		}
	}
	new_doc->doc = doc;

	return (PyObject *) new_doc;
}
