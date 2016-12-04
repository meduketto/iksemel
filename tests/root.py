#!/usr/bin/python

import piksemel

doc = piksemel.newDocument("lala")

try:
    doc.appendTag("lala")
except piksemel.NotTag:
    pass

try:
    doc.appendData("lala")
except piksemel.NotTag:
    pass

try:
    doc.prependTag("lala")
except piksemel.NotTag:
    pass

try:
    doc.prependData("lala")
except piksemel.NotTag:
    pass

