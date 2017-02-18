#!/usr/bin/env python

import iksemel as iks

doc_xml = '<lala b="" c="42"/>'

doc = iks.newDocument("lala")
doc.setAttribute("a", "lala")
doc.setAttribute("b", "nanuk")
doc.setAttribute("b", "")
doc.setAttribute("a", None)
doc.setAttribute("c", "42")

assert(doc.toString() == doc_xml)
