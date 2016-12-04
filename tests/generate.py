#!/usr/bin/python

import piksemel as iks

doc_xml = """
<test>
    <a><b><c/></b></a>
    <item foo="lala">
        <tuktuk>blah &amp; bleh</tuktuk>
        <nanuk/>
    </item>
    <a/>
</test>
""".strip()

doc = iks.newDocument("test")
doc.insertData("\n    ")
a = doc.insertTag("a")
a.insertTag("b").insertTag("c")
doc.insertData("\n    ")
item = doc.insertTag("item")
item.setAttribute("foo", "lala")
item.insertData("\n        ")
item.insertTag("tuktuk").insertData("blah & bleh")
item.insertData("\n        ")
item.insertTag("nanuk")
item.insertData("\n    ")
doc.insertData("\n    ")
a = doc.insertTag("a")
doc.insertData("\n")
assert(doc.toString() == doc_xml)

# append test

doc = iks.newDocument("lala")
a = doc.insertTag("nanik")
b = a.appendData("lala")
a = b.appendTag("lolo")
a = a.appendTag("iki")
b.appendData("hede")
doc.firstChild().appendData("fii")
assert(doc.toString() == "<lala><nanik/>fiilalahede<lolo/><iki/></lala>")

# prepend test

doc = iks.newDocument("lala")
a = doc.insertTag("foo")
a = a.prependTag("a")
a = a.prependTag("b")
a.prependData("hihi").prependData("oh").appendData("i")
a.appendTag("c")
assert(doc.toString() == "<lala>ohihihi<b/><c/><a/><foo/></lala>")

# insertNode test

doc = iks.newDocument("a")
b = iks.newDocument("b")
b.setAttribute("lal", "foo")
b = doc.insertNode(b)
b.setAttribute("lal", "foofff")
assert(doc.toString() == '<a><b lal="foofff"/></a>')

# now with append, prepend and insertNode

doc = iks.newDocument("test")

a = iks.newDocument("a")
a.insertTag("b").insertTag("c")

item = iks.newDocument("item")
node = item.insertData("\n        ")
node.appendTag("tuktuk").insertData("blah & bleh")
node = item.insertData("\n        ")
node.appendTag("nanuk").appendData("\n    ")

doc.insertNode(a).appendData("\n    ")
doc.getTag("a").prependData("\n    ")
del a
node = doc.insertNode(item)
node.setAttribute("foo", "lala")
del item
node = doc.insertData("\n    ")
node.appendTag("a").appendData("\n")
assert(doc.toString() == doc_xml)

# setData test
doc = iks.newDocument("a")
x = doc.insertTag("b")
x.insertTag("lala")
y = x.insertTag("bibi")
y.prependData("hodo")
assert(x.toString() == "<b><lala/>hodo<bibi/></b>")
x.setData("merhaba")
assert(x.toString() == "<b>merhaba</b>")
