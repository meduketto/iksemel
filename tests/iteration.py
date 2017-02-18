#!/usr/bin/env python

import iksemel as iks

doc_xml = """
<test>
    <a/>
    <item id="1"/>
    <a/>
    <foo/>
    <item id="2">
        <tuktuk>blah &amp; bleh</tuktuk>
        <a foo="bleh"/>
        <nanuk/>
    </item>
    <a/>
    <bar/>
    <item/>
    <item id="4"/>
    <a/>
</test>
"""

doc = iks.parseString(doc_xml)

count = 0
for node in doc:
    assert(node.type() == iks.TAG or node.type() == iks.DATA)
    count += 1
assert(count == 21)

count = 0
for node in doc.tags():
    assert(node.type() == iks.TAG)
    count += 1
assert(count == 10)

count = 0
for node in doc.tags("a"):
    assert(node.type() == iks.TAG)
    assert(node.name() == "a")
    assert(node.getAttribute("foo") == None)
    count += 1
assert(count == 4)

count = 0
for node in doc.tags("item"):
    assert(node.type() == iks.TAG)
    assert(node.name() == "item")
    count += 1
assert(count == 4)
