#!/usr/bin/env python

import sys,os
from distutils.core import setup, Extension

def setup_su2gmf(argv=[]):
    
    sav_argv = sys.argv;    
    
    working_dir = os.getcwd();
    file_dir    = os.path.dirname(os.path.abspath(__file__));
    
    os.chdir(file_dir);

    #--- Python 3.7 is required
    if sys.version_info < (3, 7):
      raise Exception("Script must be run using Python 3.7 or greater")

    else:
      setup(name="_su2gmf",
            version= "3.1.1",
            description="This is an extension to convert SU2 meshes and solutions to GMF.",
            author="Victorien Menier, Brian C. Munguía",
            author_email="bmunguia@stanford.edu",
            url="https://github.com/bmunguia/amgio",
            ext_modules=[ Extension( "_su2gmf", \
            sources=[ "../libMeshb/sources/libmeshb7.c", \
                      "./sources/su2gmf_py.c", \
                      "./sources/mesh.c", \
                      "./sources/gmfio.c", \
                      "./sources/su2io.c", \
                      "./sources/option.c", \
                      "./sources/su2gmf_py.i", \
                      "./sources/convert.c"],
            include_dirs=['../libMeshb/sources'],
            extra_compile_args=["-std=c99",
                                "-Wno-unused-variable",
                                "-Wno-unused-result",
                                "-O3"]),
          ],);

    os.chdir(working_dir);
    sys.argv = sav_argv;

if __name__ == '__main__':
    setup_su2gmf(sys.argv)
