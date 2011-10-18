#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys
import select
from optparse import OptionParser

import iksemel


class MyStream(iksemel.Stream):
    def on_stanza(self, doc):
        if doc.name() == "iq" and doc.get("type") == "result" and doc.get("id") == "auth":
            iq = iksemel.Document("iq")
            iq.set("type", "get")
            iq.set("id", "roster")
            query = iq.insert("query")
            query.set("xmlns",  "jabber:iq:roster")
            self.send(iq)
        if doc.name() == "iq" and doc.get("type") == "result" and doc.get("id") == "roster":
            print "Roster:"
            print doc
            # FIXME: exit
    
    def on_xml(self,  text,  is_incoming):
        if not self.options.verbose:
            return
        pre = "SEND"
        if is_incoming:
            pre = "RECV"
        print "%s [%s]" % (pre,  text)

    def ask_password(self):
        pw = self.options.password
        if pw is None:
            pw = raw_input("Password? ")
        return pw


def get_roster(options,  jid):
    my = MyStream()
    my.options = options
    my.connect(jid=iksemel.JID(jid),  tls=False)

    while 1:
        select.select([my], [], [])
        my.recv()

if __name__ == "__main__":
    parser = OptionParser()
    parser.add_option("-v",  "--verbose",  dest="verbose", action="store_true",  default=False,  help="Print XML communication")
    parser.add_option("-p",  "--password",  dest="password", action="store",  default=None,  help="Use given password")
    options,  args = parser.parse_args()

    get_roster(options,  args[0])
