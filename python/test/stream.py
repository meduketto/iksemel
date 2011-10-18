#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys
import select

import iksemel


class MyStream(iksemel.Stream):
    def on_stanza(self, doc):
        print doc, type(doc)
    
    def on_xml(self,  text,  is_incoming):
        pre = "SEND"
        if is_incoming:
            pre = "RECV"
        print "%s [%s]" % (pre,  text)

    def ask_password(self):
        pw = self.password
        if pw is None:
            pw = raw_input("Password? ")
        return pw


def get_roster(jid,  password):
    my = MyStream()
    my.password = password
    my.connect(jid=iksemel.JID(jid),  tls=False)

    while 1:
        select.select([my], [], [])
        my.recv()

if __name__ == "__main__":
    pw = None
    if len(sys.argv) > 2:
        pw = sys.argv[2]
    get_roster(sys.argv[1],  pw)
