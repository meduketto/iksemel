#!/usr/bin/python
# -*- coding: utf-8 -*-

import iksemel

# Try JID class

jid = iksemel.JID("xmpp:guybrush@example.com/lala")
assert jid.local == "guybrush"
assert jid.domain == "example.com"
assert jid.resource == "lala"
assert str(jid) == "guybrush@example.com/lala"

jid = iksemel.JID("guybrush@example.com")
assert jid.local == "guybrush"
assert jid.domain == "example.com"
assert jid.resource is None

jid = iksemel.JID("example.com")
assert jid.local is None
assert jid.domain == "example.com"
assert jid.resource is None
