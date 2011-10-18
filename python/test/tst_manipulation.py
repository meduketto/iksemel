#!/usr/bin/python
# -*- coding: utf-8 -*-

import iksemel

# Test build and manipulation methods

test = iksemel.Document("test")
a = test.insert("a")
a.insert_text("text1")
b = test.insert("b")
b.insert_text("ab")
b.insert_text("cd")
b.insert_text("2")
test.insert("c")
assert str(b) == "<b>abcd2</b>"
assert str(test) == "<test><a>text1</a><b>abcd2</b><c/></test>"

b.delete()
assert str(test) == "<test><a>text1</a><c/></test>"

ijk = a.prepend_text("ijk")
ijk.prepend("A")
ijk.append("B")
b = a.append("b")
t = b.insert_text("t")
t.append_text("r")
t.prepend_text("s")
assert str(test) == "<test><A/>ijk<B/><a>text1</a><b>str</b><c/></test>"

t.delete()
assert str(b) == "<b>sr</b>"
