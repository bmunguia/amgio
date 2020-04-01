#!/usr/bin/env python

## \file sol_to_met.py
#  \brief python script to convert GMF solution files to metric files
#  \author Brian Mungu\'ia
#

import sys
import _amgio as amgio
from optparse import OptionParser

# -------------------------------------------------------------------
#  Main 
# -------------------------------------------------------------------

def main(): 

    # Command Line Options
    parser = OptionParser()
    parser.add_option("-m", "--mesh", dest="meshfilename", type="string",
                      help="read mesh from .meshb MESHFILE (ext required)", metavar="MESHFILE")
    parser.add_option("-s", "--sol", dest="solfilename", type="string", default=None,
                      help="read sol from .solb SOLFILE (ext required)", metavar="SOLFILE")
    parser.add_option("-o", "--out", dest="outfilename", type="string", default="out",
                      help="write output to .meshb OUTFILE (ext NOT required)", metavar="OUTFILE")
    (options, args)=parser.parse_args()

    options.meshfilename = str(options.meshfilename)
    if options.solfilename == None:
        options.solfilename = ""
        sys.stdout.write("No input solution provided. \n")
        sys.stdout.flush()
    else:
        options.solfilename = str(options.solfilename)
    options.outfilename = str(options.outfilename)

    sol_to_met(options.meshfilename,
               options.solfilename,
    	       options.outfilename)

#: def main()

# -------------------------------------------------------------------
#  Mesh and Solution Conversion Function
# -------------------------------------------------------------------

def sol_to_met( mshfilename       ,
                solfilename       ,
                outfilename       ):

	amgio.py_ConvertInriaSoltoMet(mshfilename, solfilename, outfilename)

#: def sol_to_met()

# -------------------------------------------------------------------
#  Run Main Program
# -------------------------------------------------------------------

# this is only accessed if running from command prompt
if __name__ == '__main__':
    main()