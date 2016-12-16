# iksemel
My XML Parser for my personal project.
This project has been forked from Pardus-Linux github repo but original iksemel has been written to use with python2. I have rewritten pyiks.c for using iksemel in python3.


Install:
--------

You don't need anything other than an installed Python framework.
iksemel-lite source is included and compiled built-in to the module.

To compile and install: 
python3 setup.py install 
then you can import the iksemel module in your python programs.


Documentation:
--------------

===> Module Constants:

* TAG
* ATTRIBUTE
* DATA

These enumeration constants are used to report the type of the Node objects.

===> Module Exceptions

* ParseError

Raised when given string or file contains invalid XML.

* NotTag

Raised when a function accepting a tag node got another type of object.

* NotData

Raised when a function accepting a data node got another type of object.

===> Module Functions:

* parse(filename)

Parses the given XML file, and returns the top level tag's node object.

* parseString(string)

Parses the given XML string and returns the top level tag's node object.

* newDocument(tagname)

Creates a new top level tag node object with given tag name.

===> Module Objects:

* Node

This object represents a node of the XML document tree. If you iterate
over a node object, you'll get the list of its child nodes.

===> Node Object Methods:

* type()

Returns the type of the node.

* data()

Returns character data for data nodes.

* name()

Returns the name of the tag for tag nodes.

* attributes()

Returns a list of node's attribute names. Returns an empty list
if there aren't any.

* getAttribute(attributename)

Returns the value of the given attribute for tag nodes. If such
an attribute doesn't exists, returns None.

* setAttribute(attributename, value)

Sets the given attribute's value for tag nodes. You can give
the value None for deleting an attribute.

* getTag(tagname)

Returns the first tag node child with the given name. If such a child
doesn't exists, returns None.

* getTagData(tagname)

Returns the contained character data from first tag node child with the
given name.

* tags(tagname=None)

Iterates over child tag nodes. If optional tagname is given, only
iterates over tags with the given name.

* firstChild()

Returns the first child node.

* parent()

Returns the parent node.

* root()

Returns the top level node.

* next()

Returns the next node. If this is the last sibling, returns None.

* nextTag(tagname=None)

Returns the next tag node. If optional tagname is given, returns the
next sibling tag node with the given name. Returns None if this is
the last such sibling.

* previous()

Returns the previous node. If this is the first sibling, returns None.

* previousTag(tagname=None)

Returns the previous tag node. If optional tagname is given, returns
the previous sibling tag node with the given name. Returns None if
this is the first such sibling.

* insertTag(tagname)

Inserts a new child tag node with given name to the child nodes.
Returns the newly created tag node object.

* insertData(data)

Inserts given character data to the child nodes. Returns the newly
created data node object.

* setData(data)

Deletes given tag's current child nodes, then inserts the data as
its child.

* insertNode(node)

Inserts given node tree to the child nodes. Returns the newly created
node object.

* appendTag(tagname)

Create a tag node with given name and append it as the next sibling.
Returns the newly created tag node object.

* appendData(data)

Appends given character data as the next sibling. Returns the newly
created data node object.

* prependTag(tagname)

Create a tag node with given name and append it as the previous
sibling. Returns the newly created tag node object.

* prependData(data)

Prepends given character data as the previous sibling. Returns the
newly created data node object.

* hide()

Deletes the node from document tree.

* toString()

Returns the XML string representation of node tree.

* toPrettyString()

Returns the XML string representation of node tree. Line feeds and
tabs are inserted into the document to make it look pretty.
