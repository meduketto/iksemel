/* iksemel (XML parser for Jabber)
** Copyright (C) 2011 Gurer Ozen
** This code is free software; you can redistribute it and/or
** modify it under the terms of GNU Lesser General Public License.
*/

#ifndef DOCUMENT_H
#define DOCUMENT_H 1

#include <Python.h>
#include "iksemel.h"

void Document_setup(PyObject *module);
PyObject *Document_from_iks(PyObject *ref, iks *doc);


#endif  /* DOCUMENT_H */
