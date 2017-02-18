#!/usr/bin/env python

import iksemel as iks

doc_xml = """
<test>
    <item id="1"/>
    <foo/>
    <item id="2">
        <tuktuk>blah &amp; bleh</tuktuk>
        <nanuk/>
    </item>
    <bar/>
    <item/>
    <item id="4"/>
</test>
"""

doc = iks.parseString(doc_xml)

# sibling nodes

node = doc.firstChild()
last = None
count = 0
while node:
    last = node
    count += 1
    node = node.next()
assert(count == 13)

node = last
while node:
    count -= 1
    node = node.previous()
assert(count == 0)

# sibling tags

item = doc.getTag("item")
last = None
while item:
    last = item
    assert(item.type() == iks.TAG)
    assert(item.name() == "item")
    count += 1
    item = item.nextTag("item")
assert(count == 4)

item = last
while item:
    assert(item.type() == iks.TAG)
    assert(item.name() == "item")
    count -= 1
    item = item.previousTag("item")
assert(count == 0)

node = doc.getTag("item")
last = None
while node:
    last = node
    assert(node.type() == iks.TAG)
    count += 1
    node = node.nextTag()
assert(count == 6)

node = last
while node:
    assert(node.type() == iks.TAG)
    count -= 1
    node = node.previousTag()
assert(count == 0)

# parent/child

node = doc.getTag("item").nextTag().nextTag().getTag("nanuk")
assert(node.type() == iks.TAG)
assert(node.name() == "nanuk")

item = node.parent()
assert(item.type() == iks.TAG)
assert(item.name() == "item")

top = node.root()
assert(top.type() == iks.TAG)
assert(top.name() == "test")
