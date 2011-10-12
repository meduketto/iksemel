#!/usr/bin/python
# -*- coding: utf-8 -*-

import sys
import os
import subprocess

def runtests():
    fail = 0
    for test in os.listdir("."):
        if test.endswith(".py") and test != "runtests.py":
            if 0 != subprocess.call(["./" + test]):
                fail += 1
                print test,  "failed!"
    if not fail:
        return 0
    return 1

if __name__ == "__main__":
    sys.exit(runtests())
