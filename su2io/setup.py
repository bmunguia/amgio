import sys,os
from distutils.core import setup, Extension
from distutils import sysconfig


def setup_amgio(argv=[]):
    
    sav_argv = sys.argv;    
    
    working_dir = os.getcwd();
    file_dir    = os.path.dirname(os.path.abspath(__file__));
    
    os.chdir(file_dir);

    #--- Python 2.7
    if sys.version_info[0] < 3:
      setup(name="_amgio",
            version= "2.1.0",
            description="This is an extension to convert SU2 meshes and solutions to GMF.",
            author="Victorien Menier, Brian C. Munguía",
            author_email="bmunguia@stanford.edu",
            url="https://github.com/bmunguia/AMGIO",
            ext_modules=[ Extension( "_amgio", \
            sources=[ "./sources/Python2.7/amgio_py.c", \
                      "./sources/Python2.7/mesh.c", \
                      "./sources/Python2.7/GMFio.c", \
                      "./sources/Python2.7/SU2io.c", \
                      "./sources/Python2.7/option.c", \
                      "./sources/Python2.7/libmesh6.c", \
                      "./sources/Python2.7/amgio_py.i", \
                      "./sources/Python2.7/convert.c"],
            extra_compile_args=["-std=c99",
                                "-Wno-unused-variable",
                                "-Wno-unused-result"]),
          ],);

    #--- Python 3.7
    elif sys.version_info >= (3, 7):
      setup(name="_amgio",
            version= "2.1.0",
            description="This is an extension to convert SU2 meshes and solutions to GMF.",
            author="Victorien Menier, Brian C. Munguía",
            author_email="bmunguia@stanford.edu",
            url="https://github.com/bmunguia/AMGIO",
            ext_modules=[ Extension( "_amgio", \
            sources=[ "./sources/Python3.7/amgio_py.c", \
                      "./sources/Python3.7/mesh.c", \
                      "./sources/Python3.7/GMFio.c", \
                      "./sources/Python3.7/SU2io.c", \
                      "./sources/Python3.7/option.c", \
                      "./sources/Python3.7/libmesh6.c", \
                      "./sources/Python3.7/amgio_py.i", \
                      "./sources/Python3.7/convert.c"],
            extra_compile_args=["-std=c99",
                                "-Wno-unused-variable",
                                "-Wno-unused-result",
                                "-O3"]),
          ],);

    os.chdir(working_dir);
    sys.argv = sav_argv;

if __name__ == '__main__':
    setup_amgio(sys.argv)