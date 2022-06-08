/*  Example of wrapping cos function from math.h using SWIG. */

%module su2gmf
%{
    /* the resulting C file should be built as a python extension */
    #define SWIG_FILE_WITH_INIT
    /*  Includes the header in the wrapper code */
    #include "su2gmf.h" 
%}

/*  Parse the header file to generate wrappers */
%include "su2gmf.h"

