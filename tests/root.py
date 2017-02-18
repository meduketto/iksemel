#!/usr/bin/env python

import iksemel

doc = iksemel.newDocument("lala")

try:
    doc.appendTag("lala")
except iksemel.NotTag:
    pass

try:
    doc.appendData("lala")
except iksemel.NotTag:
    pass

try:
    doc.prependTag("lala")
except iksemel.NotTag:
    pass

try:
    doc.prependData("lala")
except iksemel.NotTag:
    pass

