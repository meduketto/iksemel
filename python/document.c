
#include "exceptions.h"
#include "document.h"
#include "node.h"

/* Document */

static PyTypeObject Document_type = {
	PyVarObject_HEAD_INIT(NULL,0)
	"iksemel.Node",	/* tp_name */
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

/* Iterator */

static PyTypeObject Iter_type = {
	PyVarObject_HEAD_INIT(NULL,0)
	"iksemel.Iter",	/* tp_name */
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

static void
Document_dealloc(Document *self)
{
	if (self->document) iks_delete(self->document);
	PyTypeObject* ob_type(PyObject *self);
}

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


static PyObject *
Document_parse(PyObject *self, PyObject *args)
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
			exceptions_parse();
		default:
			return PyErr_SetFromErrnoWithFilename(PyExc_OSError, file);
	}

	return new_node(NULL, doc);
}

static PyObject *
Document_parseString(PyObject *self, PyObject *args)
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
			exceptions_parse();
			return NULL;
		}
	}

	return new_node(NULL, doc);
}

static PyObject *
Document_newDocument(PyObject *self, PyObject *args)
{
	iks *doc;
	char *name;

	if (!PyArg_ParseTuple(args, "s", &name))
		return NULL;

	doc = iks_new(name);

	return new_node(NULL, doc);
}

void
Setup_Document(PyObject *module)
{
	Document_type.tp_new = PyType_GenericNew;
	if (PyType_Ready(&Document_type) < 0) return;
	Py_INCREF(&Document_type);
	Iter_type.tp_new = PyType_GenericNew;
	if (PyType_Ready(&Iter_type) < 0) return;
	Py_INCREF(&Iter_type);
}
