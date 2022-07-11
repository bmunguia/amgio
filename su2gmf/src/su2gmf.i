%define DOCSTRING
"The `su2gmf` module reads and writes SU2 and GMF meshes
and solutions, and converts between both formats."
%enddef

%module(docstring=DOCSTRING) su2gmf
%{
    #define SWIG_FILE_WITH_INIT
    #include "su2gmf.h"
%}
%include "documentation.i"
%include "su2gmf.h"