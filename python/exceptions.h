/* iksemel (XML parser for Jabber)
** Copyright (C) 2011 Gurer Ozen
** This code is free software; you can redistribute it and/or
** modify it under the terms of GNU Lesser General Public License.
*/

#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H 1

#include <Python.h>

void exceptions_setup(PyObject *module);
PyObject *exceptions_parse_error(void);
PyObject *exceptions_stream_error(int e);


#endif  /* EXCEPTIONS_H */
