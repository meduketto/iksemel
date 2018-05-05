/* iksemel (XML parser for Jabber)
** Copyright (C) 2011 Gurer Ozen
** This code is free software; you can redistribute it and/or
** modify it under the terms of GNU Lesser General Public License.
*/

#ifndef JID_H
#define JID_H 1

#include <Python.h>
#include "iksemel.h"

typedef struct {
	PyObject_HEAD
	PyObject *local;
	PyObject *domain;
	PyObject *resource;
} JID;

static PyTypeObject JID_type;
static int JID_init(JID *self, PyObject *args, PyObject *kwargs);
static int JID_init(JID *self, PyObject *args, PyObject *kwargs);
static PyObject *JID_str(JID *self);
static void JID_dealloc(JID *self);

/*** Setup ***/
void Setup_JID(PyObject *module);


#endif  /* JID_H */
