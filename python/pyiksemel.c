/* iksemel (XML parser for Jabber)
** Copyright (C) 2011 Gurer Ozen
** This code is free software; you can redistribute it and/or
** modify it under the terms of GNU Lesser General Public License.
*/

#include "exceptions.h"
#include "reference.h"
#include "document.h"
#include "jid.h"
#include "stream.h"
#include "iksemel.h"

PyObject *iksemel_module;

PyMODINIT_FUNC
initiksemel(void)
{
	PyObject *m;

	m = Py_InitModule("iksemel", NULL);

	exceptions_setup(m);
	Reference_setup();
	Document_setup(m);
	JID_setup(m);
	Stream_setup(m);

	iksemel_module = m;
}
