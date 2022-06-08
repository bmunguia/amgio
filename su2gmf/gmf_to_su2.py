#!/usr/bin/env python

## \file gmf_to_su2.py
#  \brief python script to convert GMF files to SU2
#  \author Brian Mungu\'ia
#

import sys
import su2gmf
from argparse import ArgumentParser

# -------------------------------------------------------------------
#  Main 
# -------------------------------------------------------------------

def main(): 

    # Command Line Options
    parser = ArgumentParser()
    parser.add_argument("-m", "--mesh", 
                        dest="meshfilename", 
                        type=str,
                        help="input GMF mesh (ext required)", 
                        metavar="meshfile")
    parser.add_argument("-s", "--sol", 
                        dest="solfilename", 
                        type=str,
                        help="input GMF solution (ext required)", 
                        metavar="solfile")
    parser.add_argument("-b", "--boundmesh", 
                        dest="boundmeshfilename", 
                        type=str,
                        help="input SU2 format mesh containing boundary marker names (ext required)", 
                        metavar="boundmeshfile")
    parser.add_argument("-o", "--out", 
                        dest="outfilename", 
                        type=str, 
                        default="out",
                        help="output SU2 format mesh/solution (ext NOT required)", 
                        metavar="outfile")
    args=parser.parse_args()

    # Mesh required
    if not args.meshfilename:
        raise Exception('No .meshb file provided. Run with -h for full list of options.\n')

    # Solution not required, but user should know
    if not args.solfilename:
        args.solfilename = ""
        sys.stdout.write("No input solution provided. Only converting mesh.\n")

    # Boundary mesh required
    if not args.boundmeshfilename:
        raise Exception('No .su2 file provided with MARKER_TAG information. Run with -h for full list of options.\n')

    gmf_to_su2( args.meshfilename      ,
    	        args.solfilename       ,
                args.boundmeshfilename ,
    	        args.outfilename       )

#: def main()

# -------------------------------------------------------------------
#  Mesh and Solution Conversion Function
# -------------------------------------------------------------------

def gmf_to_su2( meshfilename      ,
                solfilename       ,
                boundmeshfilename ,
                outfilename       ):

	su2gmf.GMFWithBoundtoSU2(meshfilename, solfilename, boundmeshfilename, outfilename)

#: def gmf_to_su2()

# -------------------------------------------------------------------
#  Run Main Program
# -------------------------------------------------------------------

# this is only accessed if running from command prompt
if __name__ == '__main__':
    main()