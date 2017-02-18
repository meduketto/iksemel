#!/usr/bin/env python3
# -*- coding: utf-8 -*- 
# 
# This program is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the
# Free Software Foundation; either version 2 of the License, or (at your
# option) any later version. Please read the COPYING file.
#

import sys
import os
import glob
import shutil
import subprocess
from distutils.core import setup, Extension
from distutils.command.install import install


version='3.0.1'

distfiles = """
    README
    setup.py
    src/iksemel.c
    src/iksemel.h
    src/pyiks.c
    tests/*.py
"""

if 'dist' in sys.argv:
    distdir = "iksemel-%s" % version
    list = []
    for t in distfiles.split():
        list.extend(glob.glob(t))
    if os.path.exists(distdir):
        shutil.rmtree(distdir)
    os.mkdir(distdir)
    for file_ in list:
        cum = distdir[:]
        for d in os.path.dirname(file_).split('/'):
            dn = os.path.join(cum, d)
            cum = dn[:]
            if not os.path.exists(dn):
                os.mkdir(dn)
        shutil.copy(file_, os.path.join(distdir, file_))
    os.popen("tar -czf %s %s" % ("iksemel-" + version + ".tar.gz", distdir))
    shutil.rmtree(distdir)
    sys.exit(0)

elif 'test' in sys.argv:
    fail = 0
    for test in os.listdir("tests"):
        if test.endswith(".py"):
            if 0 != subprocess.call(["tests/" + test]):
                fail += 1
                print(test, "failed!")
    if not fail:
        print("all tests passed :)")
        sys.exit(0)
    sys.exit(1)


class Install(install):
    def finalize_options(self):
        # NOTE: for Pardus distribution
        if os.path.exists("/etc/pardus-release"):
            self.install_platlib = '$base/lib/pardus'
            self.install_purelib = '$base/lib/pardus'
        install.finalize_options(self)
    
    def run(self):
        install.run(self)


setup(
    name='iksemel',
    version=version,
    ext_modules=[Extension('iksemel',
                            ['src/iksemel.c','src/pyiks.c'],
                            extra_compile_args=["-fvisibility=hidden"])],
    cmdclass = {
        'install' : Install
    }
)
