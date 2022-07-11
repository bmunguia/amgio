#!/usr/bin/env python

import sys
from setuptools import setup, Extension

#--- Python 3.7 is required
if sys.version_info < (3, 7):
    raise Exception("Script must be run using Python 3.7 or greater")

else:
    sources = [ "../libMeshb/sources/libmeshb7.c", \
                "src/su2gmf.i", \
                "src/su2gmf.c", \
                "src/mesh.c", \
                "src/gmfio.c", \
                "src/su2io.c", \
                "src/option.c", \
                "src/convert.c" ]
    include_dirs = [ "../libMeshb/sources" ]
    extra_compile_args = [ "-std=c99",
                            "-Wno-unused-variable",
                            "-Wno-unused-result",
                            "-O3" ]

    ext_modules = [ Extension("_su2gmf",
                              sources=sources,
                              include_dirs=include_dirs,
                              extra_compile_args=extra_compile_args) ]
    py_modules = [ 'su2gmf' ]

    packages = [ '' ]
    package_dir = {'': 'src'}
    package_data = {'': ['_su2gmf.so']}

    setup(name='su2gmf',
          version= '3.3.1',
          description='''Extension to convert SU2 meshes and solutions to GMF.''',
          author='Victorien Menier, Brian C. Munguía',
          author_email='bmunguia@stanford.edu',
          url='https://github.com/bmunguia/amgio',
          ext_modules=ext_modules,
          py_modules=py_modules,
          packages=packages,
          package_dir=package_dir,
          package_data=package_data);