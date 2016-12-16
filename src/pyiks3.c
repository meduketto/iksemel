/*
** Copyright (c) 2016 Aquila NIPALENSIS
**
** This program is free software; you can redistribute it and/or modify it
** under the terms of the GNU General Public License as published by the
** Free Software Foundation; either version 2 of the License, or (at your
** option) any later version. Please read the COPYING file.
*/

#include <python3.5m/Python.h>
#include "iksemel.h"

PyObject *piksemel_module;


/*** Exceptions ***/

static PyObject *ParseError;
static PyObject *NotTag;
static PyObject *NotData;

/*** Types ***/

typedef struct {
	PyObject_HEAD
	iks *document;
} Document;

typedef struct {
	PyObject_HEAD
	Document *doc;
	iks *node;
} Node;

typedef struct {
	PyObject_HEAD
	Document *doc;
	iks *node;
	int tags;
	char *tagname;
} Iter;

/*** Document ***/
static void
Document_dealloc(Document *self)
{
	if (self->document) iks_delete(self->document);
	PyTypeObject* ob_type(PyObject *self);
}

static PyTypeObject Document_type = {
	PyVarObject_HEAD_INIT(NULL,0)
	"piksemel.Document",	/* tp_name */
	sizeof(Document),	/* tp_basicsize */
	0,			/* tp_itemsize */
	(destructor)Document_dealloc,	/* tp_dealloc */
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
	"XML doc object",	/* tp_doc */
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

/*** Iterator ***/

static PyObject *
Iter_iter(Iter *self)
{
	Py_INCREF(self);
	return (PyObject *)self;
}

static PyObject *
Iter_next(Iter *self)
{
	iks *node;

	node = self->node;
	if (!node) return NULL;

	if (self->tags) {
		self->node = iks_next_tag(node);
		if (self->node && self->tagname) {
			while (self->node && (strcmp(self->tagname, iks_name(self->node)) != 0)) {
				self->node = iks_next_tag(self->node);
			}
		}
	} else {
		self->node = iks_next(node);
	}

	return new_node(self->doc, node);
}

static PyTypeObject Iter_type = {
	PyVarObject_HEAD_INIT(NULL,0)
	"piksemel.Iter",	/* tp_name */
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
	"XML iter object",	/* tp_doc */
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

/*** Nodes ***/
static PyObject *
new_node(Document *doc, iks *xml)
{
	Node *node;
	int ref = 1;

	if (!xml) return PyErr_NoMemory();

	if (!doc) {
		doc = PyObject_New(Document, &Document_type);
		doc->document = xml;
		ref = 0;
	}
	node = PyObject_New(Node, &Node_type);
	node->doc = doc;
	if (ref) {
		Py_INCREF(doc);
	}
	node->node = xml;
	return (PyObject *)node;
}

static void
Node_dealloc(Node *self)
{
	if (self->doc) {
		Py_DECREF(self->doc);
	}
	PyTypeObject* ob_type(PyObject *self);
}

static PyObject *
Node_toString(Node *self, PyObject *args)
{
	PyObject *ret;
	char *str;

	str = iks_string(NULL, self->node);
	ret = Py_BuildValue("s", str);
	iks_free(str);

	return ret;
}

static PyObject *
Node_iter(Node *self)
{
	Iter *iter;

	if (iks_type(self->node) != IKS_TAG) {
		PyErr_SetNone(NotTag);
		return NULL;
	}

	iter = PyObject_New(Iter, &Iter_type);
	iter->node = iks_child(self->node);
	iter->tags = 0;
	iter->doc = self->doc;
	iter->tagname = NULL;
	return (PyObject *)iter;
}

static PyObject *
Node_type_func(Node *self)
{
	PyObject *ret;

	ret = Py_BuildValue("i", iks_type(self->node));
	return ret;
}

static PyObject *
Node_reduce(Node *self, PyObject *args)
{
	PyObject *ret;
	PyObject *dict;
	PyObject *tuple;
	PyObject *func;
	PyObject *state;

	state = Node_toString(self, args);
	if (!state) return NULL;

	dict = PyModule_GetDict(piksemel_module);
	if (!dict) return NULL;
	func = PyDict_GetItemString(dict, "parseString");
	if (!func) return NULL;

	tuple = PyTuple_Pack(1, state);
	if (!tuple) return NULL;

	ret = PyTuple_Pack(2, func, tuple);

	return ret;
}

static PyObject *
Node_data(Node *self)
{
	PyObject *ret;

	if (iks_type(self->node) != IKS_CDATA) {
		PyErr_SetNone(NotData);
		return NULL;
	}

	ret = Py_BuildValue("s", iks_cdata(self->node));
	return ret;
}

static PyObject *
Node_name(Node *self)
{
	if (iks_type(self->node) != IKS_TAG) {
		PyErr_SetNone(NotTag);
		return NULL;
	}

	return Py_BuildValue("s", iks_name(self->node));
}

static PyObject *
Node_attributes(Node *self, PyObject *args)
{
	PyObject *ret, *p;
	iks *attr;

	if (iks_type(self->node) != IKS_TAG) {
		PyErr_SetNone(NotTag);
		return NULL;
	}

	ret = PyList_New(0);
	if (!ret) return NULL;

	for (attr = iks_attrib(self->node); attr; attr = iks_next(attr)) {
		p = PyBytes_FromString(iks_name(attr));
		if (p) PyList_Append(ret, p);
	}

	return ret;
}

static PyObject *
Node_getAttribute(Node *self, PyObject *args)
{
	PyObject *ret;
	char *name;
	char *val;

	if (iks_type(self->node) != IKS_TAG) {
		PyErr_SetNone(NotTag);
		return NULL;
	}

	if (!PyArg_ParseTuple(args, "s", &name))
		return NULL;

	val = iks_find_attrib(self->node, name);
	if (!val) {
		Py_INCREF(Py_None);
		return Py_None;
	}

	ret = Py_BuildValue("s", val);
	return ret;
}

static PyObject *
Node_setAttribute(Node *self, PyObject *args)
{
	char *name;
	char *value;

	if (iks_type(self->node) != IKS_TAG) {
		PyErr_SetNone(NotTag);
		return NULL;
	}

	if (!PyArg_ParseTuple(args, "sz", &name, &value))
		return NULL;

	iks_insert_attrib(self->node, name, value);

	Py_INCREF(Py_None);
	return Py_None;
}

static PyObject *
Node_setData(Node *self, PyObject *args)
{
    char *data;

    if (iks_type(self->node) != IKS_TAG) {
        PyErr_SetNone(NotTag);
        return NULL;
    }

    if (!PyArg_ParseTuple(args, "s", &data))
        return NULL;

    iks_set_cdata(self->node, data, 0);

    Py_INCREF(Py_None);
    return Py_None;
}

static PyObject *
Node_getTag(Node *self, PyObject *args)
{
	iks *child;
	char *name;

	if (iks_type(self->node) != IKS_TAG) {
		PyErr_SetNone(NotTag);
		return NULL;
	}

	if (!PyArg_ParseTuple(args, "s", &name))
		return NULL;

	child = iks_find(self->node, name);
	if (!child) {
		Py_INCREF(Py_None);
		return Py_None;
	}

	return new_node(self->doc, child);
}

static PyObject *
Node_getTagData(Node *self, PyObject *args)
{
	PyObject *ret;
	char *name;
	char *data;

	if (iks_type(self->node) != IKS_TAG) {
		PyErr_SetNone(NotTag);
		return NULL;
	}

	if (!PyArg_ParseTuple(args, "s", &name))
		return NULL;

	data = iks_find_cdata(self->node, name);
	if (!data) {
		Py_INCREF(Py_None);
		return Py_None;
	}

	ret = Py_BuildValue("s", data);
	return ret;
}

static PyObject *
Node_tags(Node *self, PyObject *args)
{
	Iter *iter;
	char *name = NULL;

	if (!PyArg_ParseTuple(args, "|s", &name))
		return NULL;

	if (iks_type(self->node) != IKS_TAG) {
		PyErr_SetNone(NotTag);
		return NULL;
	}

	iter = PyObject_New(Iter, &Iter_type);
	iter->doc = self->doc;
	iter->node = iks_first_tag(self->node);
	if (name && iter->node) {
		while (iter->node && (strcmp(name, iks_name(iter->node)) != 0)) {
			iter->node = iks_next_tag(iter->node);
		}
	}
	iter->tags = 1;
	iter->tagname = name;
	return (PyObject *)iter;
}

static PyObject *
Node_firstChild(Node *self)
{
	iks *child;

	if (iks_type(self->node) != IKS_TAG) {
		PyErr_SetNone(NotTag);
		return NULL;
	}

	child = iks_child(self->node);
	if (!child) {
		Py_INCREF(Py_None);
		return Py_None;
	}

	return new_node(self->doc, child);
}

static PyObject *
Node_parent(Node *self)
{
	iks *parent;

	parent = iks_parent(self->node);
	if (!parent) {
		Py_INCREF(Py_None);
		return Py_None;
	}

	return new_node(self->doc, parent);
}

static PyObject *
Node_root(Node *self)
{
	iks *root;

	root = iks_root(self->node);
	if (!root) {
		Py_INCREF(Py_None);
		return Py_None;
	}

	return new_node(self->doc, root);
}

static PyObject *
Node_next(Node *self)
{
	iks *sibling;

	sibling = iks_next(self->node);
	if (!sibling) {
		Py_INCREF(Py_None);
		return Py_None;
	}

	return new_node(self->doc, sibling);
}

static PyObject *
Node_nextTag(Node *self, PyObject *args)
{
	iks *sibling;
	char *name = NULL;

	if (!PyArg_ParseTuple(args, "|s", &name))
		return NULL;

	sibling = iks_next_tag(self->node);
	if (sibling && name) {
		while (strcmp(iks_name(sibling), name) != 0) {
			sibling = iks_next_tag(sibling);
			if (!sibling) break;
		}
	}
	if (!sibling) {
		Py_INCREF(Py_None);
		return Py_None;
	}

	return new_node(self->doc, sibling);
}

static PyObject *
Node_previous(Node *self)
{
	iks *sibling;

	sibling = iks_prev(self->node);
	if (!sibling) {
		Py_INCREF(Py_None);
		return Py_None;
	}

	return new_node(self->doc, sibling);
}

static PyObject *
Node_previousTag(Node *self, PyObject *args)
{
	iks *sibling;
	char *name = NULL;

	if (!PyArg_ParseTuple(args, "|s", &name))
		return NULL;

	sibling = iks_prev_tag(self->node);
	if (sibling && name) {
		while (strcmp(iks_name(sibling), name) != 0) {
			sibling = iks_prev_tag(sibling);
			if (!sibling) break;
		}
	}
	if (!sibling) {
		Py_INCREF(Py_None);
		return Py_None;
	}

	return new_node(self->doc, sibling);
}

struct makeup_ctx {
	unsigned int level;
	int can_indent;
};

static void
prettify(struct makeup_ctx *ctx, iks *parent, iks *node)
{
	iks *x, *y, *a;
	int i;

	for (x = iks_child(node); x; x = iks_next(x)) {
		if (iks_type(x) == IKS_TAG) {
			if (ctx->can_indent) {
				iks_insert_cdata(parent, "\n", 1);
				for (i = 0; i < ctx->level; ++i) {
					iks_insert_cdata(parent, "    ", 4);
				}
			}
			y = iks_insert(parent, iks_name(x));
			for (a = iks_attrib(x); a; a = iks_next(a)) {
				iks_insert_attrib(y, iks_name(a), iks_cdata(a));
			}
			if (iks_child(x)) {
				ctx->can_indent = 1;
				++ctx->level;
				prettify(ctx, y, x);
				--ctx->level;
				if (!iks_next(x)) {
					iks_insert_cdata(parent, "\n", 1);
					for (i = 0; i < ctx->level - 1; ++i) {
						iks_insert_cdata(parent, "    ", 4);
					}
				}
			}
			ctx->can_indent = 1;
		} else {
			iks_insert_cdata(parent, iks_cdata(x), iks_cdata_size(x));
			ctx->can_indent = 0;
		}
	}
}

static PyObject *
Node_toPrettyString(Node *self, PyObject *args)
{
	struct makeup_ctx ctx;
	PyObject *ret;
	iks *tree, *a;
	char *str;

	if (iks_type(self->node) != IKS_TAG) {
		PyErr_SetNone(NotTag);
		return NULL;
	}

	ctx.level = 1;
	ctx.can_indent = 1;

	tree = iks_new(iks_name(self->node));
	for (a = iks_attrib(self->node); a; a = iks_next(a)) {
		iks_insert_attrib(tree, iks_name(a), iks_cdata(a));
	}
	prettify(&ctx, tree, self->node);

	str = iks_string(NULL, tree);
	ret = Py_BuildValue("s", str);
	iks_free(str);
	iks_delete(tree);

	return ret;
}

static PyObject *
Node_insertTag(Node *self, PyObject *args)
{
	iks *node;
	char *name;

	if (iks_type(self->node) != IKS_TAG) {
		PyErr_SetNone(NotTag);
		return NULL;
	}

	if (!PyArg_ParseTuple(args, "s", &name))
		return NULL;

	node = iks_insert(self->node, name);

	return new_node(self->doc, node);
}

static PyObject *
Node_appendSibling(Node *self, PyObject *args)
{
	iks *node;
	char *name;

	if (!PyArg_ParseTuple(args, "s", &name))
		return NULL;

	if (iks_parent(self->node) == NULL) {
		PyErr_SetString(NotTag, "Cannot append to the top level tag, use insert");
		return NULL;
	}

	node = iks_insert_sibling(self->node, name);

	return new_node(self->doc, node);
}

static PyObject *
Node_prependTag(Node *self, PyObject *args)
{
	iks *node;
	char *name;

	if (!PyArg_ParseTuple(args, "s", &name))
		return NULL;

	if (iks_parent(self->node) == NULL) {
		PyErr_SetString(NotTag, "Cannot prepend to the top level tag, use insert");
		return NULL;
	}

	node = iks_prepend(self->node, name);

	return new_node(self->doc, node);
}

static PyObject *
Node_insertData(Node *self, PyObject *args)
{
	iks *node;
	char *value;

	if (iks_type(self->node) != IKS_TAG) {
		PyErr_SetNone(NotTag);
		return NULL;
	}

	if (!PyArg_ParseTuple(args, "s", &value))
		return NULL;

	node = iks_insert_cdata(self->node, value, 0);

	return new_node(self->doc, node);
}

static PyObject *
Node_appendSiblingData(Node *self, PyObject *args)
{
	iks *node;
	char *value;

	if (!PyArg_ParseTuple(args, "s", &value))
		return NULL;

	if (iks_parent(self->node) == NULL) {
		PyErr_SetString(NotTag, "Cannot append to the top level tag, use insert");
		return NULL;
	}

	node = iks_append_cdata(self->node, value, strlen(value));

	return new_node(self->doc, node);
}


static PyObject *
Node_prependData(Node *self, PyObject *args)
{
	iks *node;
	char *value;

	if (!PyArg_ParseTuple(args, "s", &value))
		return NULL;

	if (iks_parent(self->node) == NULL) {
		PyErr_SetString(NotTag, "Cannot prepend to the top level tag, use insert");
		return NULL;
	}

	node = iks_prepend_cdata(self->node, value, strlen(value));

	return new_node(self->doc, node);
}

static PyObject *
Node_insertNode(Node *self, PyObject *args)
{
	iks *child;
	Node *node;

	if (iks_type(self->node) != IKS_TAG) {
		PyErr_SetNone(NotTag);
		return NULL;
	}

	if (!PyArg_ParseTuple(args, "O!", &Node_type, &node))
		return NULL;

	child = iks_copy_within(node->node, iks_stack(self->node));
	iks_insert_node(self->node, child);

	return new_node(self->doc, child);
}

static PyObject *
Node_hide(Node *self, PyObject *args)
{
	iks_hide(self->node);

	Py_INCREF(Py_None);
	return Py_None;
}

static PyMethodDef Node_methods[] = {
	{ "type", (PyCFunction)Node_type_func, METH_NOARGS,
	  "Return the type of node." },
	{ "__reduce__", (PyCFunction)Node_reduce, METH_NOARGS,
	  "used by pickle" },
	{ "name", (PyCFunction)Node_name, METH_NOARGS,
	  "Return tag name." },
	{ "data", (PyCFunction)Node_data, METH_NOARGS,
	  "Return node's character data." },
	{ "setData", (PyCFunction)Node_setData, METH_VARARGS,
	  "Set character data child of the tag." },
	{ "attributes", (PyCFunction)Node_attributes, METH_NOARGS,
	  "Return node's attribute names." },
	{ "getAttribute", (PyCFunction)Node_getAttribute, METH_VARARGS,
	  "Return value of a tag attribute." },
	{ "setAttribute", (PyCFunction)Node_setAttribute, METH_VARARGS,
	  "Set the value of a tag attribute." },
	{ "getTag", (PyCFunction)Node_getTag, METH_VARARGS,
	  "Return first child tag with the given name." },
	{ "getTagData", (PyCFunction)Node_getTagData, METH_VARARGS,
	  "Return character data of the child tag with given name." },
	{ "tags", (PyCFunction)Node_tags, METH_VARARGS,
	  "Iterate over all or optionally only matching tags." },
	{ "firstChild", (PyCFunction)Node_firstChild, METH_NOARGS,
	  "Return first child node." },
	{ "parent", (PyCFunction)Node_parent, METH_NOARGS,
	  "Return parent node." },
	{ "root", (PyCFunction)Node_root, METH_NOARGS,
	  "Return topmost parent node." },
	{ "nextTag", (PyCFunction)Node_nextTag, METH_VARARGS,
	  "Return next sibling tag node." },
	{ "next", (PyCFunction)Node_next, METH_NOARGS,
	  "Return next sibling node." },
	{ "previousTag", (PyCFunction)Node_previousTag, METH_VARARGS,
	  "Return previous sibling tag node." },
	{ "previous", (PyCFunction)Node_previous, METH_NOARGS,
	  "Return previous sibling node." },
	{ "toString", (PyCFunction)Node_toString, METH_NOARGS,
	  "Convert a document tree to XML string representation." },
	{ "toPrettyString", (PyCFunction)Node_toPrettyString, METH_NOARGS,
	  "Convert a document tree to indented XML string representation." },
	{ "insertTag", (PyCFunction)Node_insertTag, METH_VARARGS,
	  "Insert a child tag node with given name." },
	{ "appendTag", (PyCFunction)Node_appendSibling, METH_VARARGS,
	  "Append a sibling tag node with given name." },
	{ "prependTag", (PyCFunction)Node_prependTag, METH_VARARGS,
	  "Prepend a sibling tag node with given name." },
	{ "insertData", (PyCFunction)Node_insertData, METH_VARARGS,
	  "Insert a child character data node with given text." },
	{ "appendData", (PyCFunction)Node_appendSiblingData, METH_VARARGS,
	  "Append a sibling character data node with given text." },
	{ "prependData", (PyCFunction)Node_prependData, METH_VARARGS,
	  "Prepend a sibling character data node with given text." },
	{ "insertNode", (PyCFunction)Node_insertNode, METH_VARARGS,
	  "Insert another document as a child." },
	{ "hide", (PyCFunction)Node_hide, METH_VARARGS,
	  "Hide tag from document tree." },
	{ NULL }
};

static PyTypeObject Node_type = {
	PyVarObject_HEAD_INIT(NULL,0)
	"piksemel.Node",	/* tp_name */
	sizeof(Node),		/* tp_basicsize */
	0,			/* tp_itemsize */
	(destructor)Node_dealloc,/* tp_dealloc */
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
	"XML node object",	/* tp_doc */
	0,			/* tp_traverse */
	0,			/* tp_clear */
	0,			/* tp_richcompare */
	0,			/* tp_weaklistoffset */
	(getiterfunc)Node_iter,	/* tp_iter */
	0,			/* tp_iternext */
	Node_methods,		/* tp_methods */
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

/*** Module Functions ***/

static PyObject *
piks_parse(PyObject *self, PyObject *args)
{
	char *file;
	iks *doc;
	int e;

	if (!PyArg_ParseTuple(args, "s", &file))
		return NULL;

	e = iks_load(file, &doc);
	switch (e) {
		case IKS_OK:
			break;
		case IKS_NOMEM:
			return PyErr_NoMemory();
		case IKS_BADXML:
			PyErr_SetNone(ParseError);
			return NULL;
		default:
			return PyErr_SetFromErrnoWithFilename(PyExc_OSError, file);
	}

	return new_node(NULL, doc);
}

static PyObject *
piks_parseString(PyObject *self, PyObject *args)
{
	iks *doc;
	char *str;
	int e;

	if (!PyArg_ParseTuple(args, "s", &str))
		return NULL;

	doc = iks_tree(str, 0, &e);
	if (!doc) {
		if (e == IKS_NOMEM) {
			return PyErr_NoMemory();
		} else {
			PyErr_SetNone(ParseError);
			return NULL;
		}
	}

	return new_node(NULL, doc);
}

static PyObject *
piks_newDocument(PyObject *self, PyObject *args)
{
	iks *doc;
	char *name;

	if (!PyArg_ParseTuple(args, "s", &name))
		return NULL;

	doc = iks_new(name);

	return new_node(NULL, doc);
}

static PyMethodDef methods[] = {
	{ "parse", piks_parse, METH_VARARGS,
	  "Parse given XML file and generate document tree."},
	{ "parseString", piks_parseString, METH_VARARGS,
	  "Parse given XML string and generate document tree."},
	{ "newDocument", piks_newDocument, METH_VARARGS,
	  "Create a new document with given root tag name."},
	{ NULL, NULL, 0, NULL }
};

static struct PyModuleDef piksemelmodule ={
	PyModuleDef_HEAD_INIT,
	"piksemel",
	NULL,
	-1,
	methods
};		

__attribute__((visibility("default")))
PyMODINIT_FUNC 
PyInit_piksemel(void)
{
	PyObject *m;

	m = PyModule_Create(&piksemelmodule);
	/* constants */
	PyModule_AddIntConstant(m, "TAG", IKS_TAG);
	PyModule_AddIntConstant(m, "ATTRIBUTE", IKS_ATTRIBUTE);
	PyModule_AddIntConstant(m, "DATA", IKS_CDATA);
	/* exceptions */
	ParseError = PyErr_NewException("piksemel.ParseError", NULL, NULL);
	Py_INCREF(ParseError);
	PyModule_AddObject(m, "ParseError", ParseError);
	NotTag = PyErr_NewException("piksemel.NotTag", NULL, NULL);
	Py_INCREF(NotTag);
	PyModule_AddObject(m, "NotTag", NotTag);
	NotData = PyErr_NewException("piksemel.NotData", NULL, NULL);
	Py_INCREF(NotData);
	PyModule_AddObject(m, "NotData", NotData);
	/* types */
	Document_type.tp_new = PyType_GenericNew;
	if (PyType_Ready(&Document_type) < 0)
		return 0;
	Py_INCREF(&Document_type);
	Iter_type.tp_new = PyType_GenericNew;
	if (PyType_Ready(&Iter_type) < 0)
		return 0;
	Py_INCREF(&Iter_type);
	Node_type.tp_new = PyType_GenericNew;
	if (PyType_Ready(&Node_type) < 0)
		return 0;
	Py_INCREF(&Node_type);
	PyModule_AddObject(m, "Node", (PyObject *)&Node_type);

	piksemel_module = m;
}
