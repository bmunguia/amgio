import sys,os
from distutils.core import setup, Extension


def setup_amgio(argv=[]):
    
    sav_argv = sys.argv;    
    sys.argv = ['', 'build_ext', '--inplace'];
    
    working_dir = os.getcwd();
    file_dir    = os.path.dirname(os.path.abspath(__file__));
    
    os.chdir(file_dir);
    setup(ext_modules=[ \
          Extension("_amgio",
          sources=[ "./sources/libmeshb7.c", \
                    "./su2io/sources/amgio_py.c", \
     				"./su2io/sources/mesh.c", \
     				"./su2io/sources/GMFio.c", \
     				"./su2io/sources/SU2io.c", \
     				"./su2io/sources/option.c", \
                    "./su2io/sources/amgio_py.i", \
     				"./su2io/sources/convert.c"],
           extra_compile_args=["-std=c99","-Wno-unused-variable","-Wno-unused-result"]), 
           ]);
    
    os.chdir(working_dir);
    sys.argv = sav_argv;

if __name__ == '__main__':
    setup_amgio(sys.argv)