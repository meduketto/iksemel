/* iksemel (XML parser for Jabber)
** Copyright (C) 2011 Gurer Ozen
** This code is free software; you can redistribute it and/or
** modify it under the terms of GNU Lesser General Public License.
*/

#ifndef JID_H
#define JID_H 1

#include <Python.h>
#include "iksemel.h"

#if PY_MAJOR_VERSION >= 3
    #define PyString_AsString PyBytes_AsString
    #define PyString_FromFormat PyUnicode_FromFormat
#endif

#ifndef PyVarObject_HEAD_INIT
    #define PyVarObject_HEAD_INIT(type, size) \
        PyObject_HEAD_INIT(type) size,
#endif

void JID_setup(PyObject *module);


#endif  /* JID_H */
