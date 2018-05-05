#ifndef NODE_H
#define NODE_H 1

#include <Python.h>
#include "iksemel.h"

/*** Setup ***/
void Setup_Node(PyObject *module);

/*** Types ***/

typedef struct {
	PyObject_HEAD
	Document *doc;
	iks *node;
} Node;

static PyTypeObject Node_type;

#endif  /* NODE_H */
