/* iksemel (XML parser for Jabber)
** Copyright (C) 2011 Gurer Ozen
** This code is free software; you can redistribute it and/or
** modify it under the terms of GNU Lesser General Public License.
*/

/*
** Generic reference object
** Does nothing except keeping the object count and being referenced by
** counted objects. 
*/

#ifndef REFERENCE_H
#define REFERENCE_H 1

#include <Python.h>

void Reference_setup(void);
PyObject *Reference_new(void);


#endif  /* REFERENCE_H */
