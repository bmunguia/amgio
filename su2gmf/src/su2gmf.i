%define DOCSTRING
"The `su2gmf` module converts mesh and solution files
between SU2 and GMF formats."
%enddef

%module(docstring=DOCSTRING) su2gmf
%{
    #define SWIG_FILE_WITH_INIT
    #include "su2gmf.h" 
%}
%include "su2gmf.h"

