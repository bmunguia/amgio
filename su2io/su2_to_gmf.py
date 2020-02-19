#!/usr/bin/env python

## \file su2_to_gmf.py
#  \brief python script to convert SU2 files to GMF
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

    su2_to_gmf(options.meshfilename,
    	       options.solfilename,
    	       options.outfilename)

#: def main()

# -------------------------------------------------------------------
#  Mesh and Solution Conversion Function
# -------------------------------------------------------------------

def su2_to_gmf( meshfilename ,
                solfilename  ,
                outfilename  ):

	amgio.py_ConvertSU2toInria(meshfilename, solfilename, outfilename)

#: def su2_to_gmf()

# -------------------------------------------------------------------
#  Run Main Program
# -------------------------------------------------------------------

# this is only accessed if running from command prompt
if __name__ == '__main__':
    main()