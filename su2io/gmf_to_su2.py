#!/usr/bin/env python

## \file gmf_to_su2.py
#  \brief python script to convert GMF files to SU2
#  \author Brian Mungu\'ia
#

import _amgio as amgio
from optparse import OptionParser

# -------------------------------------------------------------------
#  Main 
# -------------------------------------------------------------------

def main(): 

    # Command Line Options
    parser = OptionParser()
    parser.add_option("-m", "--mesh", dest="meshfilename",
                      help="read mesh from MESHFILE", metavar="MESHFILE")
    parser.add_option("-s", "--sol", dest="solfilename", default=None,
                      help="read sol from SOLFILE", metavar="SOLFILE")
    parser.add_option("-o", "--out", dest="outfilename", default="out",
                      help="write output to OUTFILE", metavar="OUTFILE")
    (options, args)=parser.parse_args()

    if options.solfilename == None:
        options.solfilename = ""

    gmf_to_su2(options.meshfilename,
    	       options.solfilename,
    	       options.outfilename)

#: def main()

# -------------------------------------------------------------------
#  Mesh and Solution Conversion Function
# -------------------------------------------------------------------

def gmf_to_su2( meshfilename ,
                solfilename  ,
                outfilename  ):

	amgio.py_ConvertInriatoSU2(meshfilename, solfilename, outfilename)

#: def gmf_to_su2()

# -------------------------------------------------------------------
#  Run Main Program
# -------------------------------------------------------------------

# this is only accessed if running from command prompt
if __name__ == '__main__':
    main()