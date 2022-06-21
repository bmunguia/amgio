#!/usr/bin/env python

"""! @file gmf_to_su2.py
@brief python script to convert GMF files to SU2
@author Brian Mungu\'ia
"""

import sys
import su2gmf
from argparse import ArgumentParser, RawTextHelpFormatter

def main(): 

    # Command Line Options
    parser = ArgumentParser(formatter_class=RawTextHelpFormatter)
    parser.add_argument("-m", "--msh", 
                        dest="meshfilename", 
                        type=str,
                        help="input GMF mesh with ext", 
                        metavar="MSH")
    parser.add_argument("-s", "--sol", 
                        dest="solfilename", 
                        type=str,
                        help="input GMF solution with ext", 
                        metavar="SOL")
    parser.add_argument("-b", "--bndmsh", 
                        dest="boundmeshfilename", 
                        type=str,
                        help="input SU2 mesh with ext\n"
                              "contains boundary marker names", 
                        metavar="BND")
    parser.add_argument("-o", "--out", 
                        dest="outfilename", 
                        type=str, 
                        default="out",
                        help="base filename for output SU2 mesh/solution\n"
                             "outputs OUT.dat and OUT.su2",
                        metavar="OUT")
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

def gmf_to_su2( meshfilename      ,
                solfilename       ,
                boundmeshfilename ,
                outfilename       ):

	su2gmf.GMFWithBoundtoSU2(meshfilename, solfilename, boundmeshfilename, outfilename)

if __name__ == '__main__':
    main()