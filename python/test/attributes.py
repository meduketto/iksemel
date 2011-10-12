#!/usr/bin/python
# -*- coding: utf-8 -*-

import iksemel

a = iksemel.Document('<a x="" y="şğıdfsf" z="e2e4tgg"><b/></a>')

# Try get() interface
assert a.get("x") == ""
assert a.get("y") == "şğıdfsf"
assert a.get("g") is None

# Try set() interface
a.set("x",  None)
a.set("g",  "lala")
a.set("p",  None)
# Check that it worked
assert a.get("x") is None
assert a.get("y") == "şğıdfsf"
assert a.get("g") == "lala"
assert a.get("p") is None
assert a.get("z") == "e2e4tgg"

# Try attributes() interface
x = a.attributes()
assert len(x.keys()) == 3
y = list(x.items())
y.sort()
assert y == [("g", "lala"), ("y", "şğıdfsf"), ("z", "e2e4tgg")]
