#!/usr/bin/python
# -*- coding: utf-8 -*-

import iksemel

a = iksemel.Document("""<a>
    text1
    <b>
        text2
            <c/>
            <d>text3</d>
    </b>
    text4
    <e/>
    text5
    <f/>
</a>""")

# Try simple tree traversal interfaces

assert a.is_tag()
assert a.name() == "a"
assert a.parent() is None

t1 = a.child()
assert t1.is_text()
assert not t1.is_tag()
assert t1.text().strip() == "text1"

b = t1.next()
assert b.is_tag()
assert not b.is_text()
assert b.name() == "b"

e = b.next_tag()
assert e.name() == "e"

f = b.next_tag("f")
assert f.name() == "f"
assert f.next_tag() is None
assert f.next().next() is None

assert f.prev_tag("b").name() == "b"
assert f.prev().text().strip() == "text5"
assert f.prev().prev().name() == "e"

c = b.find("c")
assert c.name() == "c"
assert c.parent().name() == "b"
assert c.root().name() == "a"

assert b.find_text("d") == "text3"
