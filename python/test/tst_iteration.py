#!/usr/bin/python
# -*- coding: utf-8 -*-

import iksemel

test = iksemel.Document("""<test>
text1
    <b>
        <c/>
    </b>
    <a/>
    <a/>
text2
    <b/>
    <a/>
    <d/>
    <a/>
text3
</test>""")

# Try iteration protocols for travelsal

nodes = [x for x in test]
assert len([x for x in nodes if isinstance(x,  iksemel.Document)]) == 15

tags = [x.name() for x in test.tags()]
assert tags == ["b", "a", "a", "b", "a",  "d",  "a"]

atags = [x for x in test.tags("a") if x.name() == "a"]
assert len(atags) == 4
