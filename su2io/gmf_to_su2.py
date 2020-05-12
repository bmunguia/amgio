#!/usr/bin/env python

## \file gmf_to_su2.py
#  \brief python script to convert GMF files to SU2
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
    parser.add_option("-s", "--sol", dest="solfilename", type="string",
                      help="read sol from .solb SOLFILE (ext required)", metavar="SOLFILE")
    parser.add_option("-b", "--boundmesh", dest="boundmeshfilename", type="string",
                      help="read boundary marker names from .su2 BOUNDMESHFILE (ext required)", metavar="BOUNDMESHFILE")
    parser.add_option("-o", "--out", dest="outfilename", type="string", default="out",
                      help="write output to .csv/.su2 OUTFILE (ext NOT required)", metavar="OUTFILE")
    (options, args)=parser.parse_args()

    # Mesh
    if not options.meshfilename:
        raise Exception('No .meshb file provided. Run with -h for full list of options.\n')
    else:
        options.meshfilename = str(options.meshfilename)

    # Solution
    if not options.solfilename:
        options.solfilename = ""
        sys.stdout.write("No input solution provided. Only converting mesh.\n")
        sys.stdout.flush()
    else:
        options.solfilename = str(options.solfilename)

    # Boundary mesh
    if not options.boundmeshfilename:
        raise Exception('No .su2 file provided with MARKER_TAG information. Run with -h for full list of options.\n')
    else:
        options.boundmeshfilename = str(options.boundmeshfilename)

    # Output
    options.outfilename = str(options.outfilename)

    gmf_to_su2(options.meshfilename,
    	       options.solfilename,
               options.boundmeshfilename,
    	       options.outfilename)

#: def main()

# -------------------------------------------------------------------
#  Mesh and Solution Conversion Function
# -------------------------------------------------------------------

def gmf_to_su2( meshfilename      ,
                solfilename       ,
                boundmeshfilename ,
                outfilename       ):

	amgio.py_ConvertInriaWithBoundtoSU2(meshfilename, solfilename, boundmeshfilename, outfilename)

#: def gmf_to_su2()

# -------------------------------------------------------------------
#  Run Main Program
# -------------------------------------------------------------------

# this is only accessed if running from command prompt
if __name__ == '__main__':
    main()