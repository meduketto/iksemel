#ifndef DOCUMENT_H
#define DOCUMENT_H 1

#include <Python.h>
#include "iksemel.h"

/*** Document ***/

typedef struct {
	PyObject_HEAD
	iks *document;
} Document;

static PyTypeObject Document_type;
static void Document_dealloc(Document *self);
static PyObject *Document_parse(PyObject *self, PyObject *args);
static PyObject *Document_parseString(PyObject *self, PyObject *args);
static PyObject *Document_newDocument(PyObject *self, PyObject *args);

/*** Iterator ***/

typedef struct {
	PyObject_HEAD
	Document *doc;
	iks *node;
	int tags;
	char *tagname;
} Iter;

static PyTypeObject Iter_type;
static PyObject *Iter_iter(Iter *self);
static PyObject *Iter_next(Iter *self);
static PyObject *new_node(Document *doc, iks *xml);

/*** Setup ***/
void Setup_Document(PyObject *module);

#endif  /* DOCUMENT_H */
