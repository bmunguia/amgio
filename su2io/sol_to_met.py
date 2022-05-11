#!/usr/bin/env python

## \file sol_to_met.py
#  \brief python script to convert GMF solution files to metric files
#  \author Brian Mungu\'ia
#

import sys
import _amgio as amgio
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
                        metavar="SOLFILE")
    parser.add_argument("-o", "--out", 
                        dest="outfilename", 
                        type=str, 
                        default="out",
                        help="output GMF metric (ext NOT required)", 
                        metavar="OUTFILE")
    args=parser.parse_args()

    # Mesh required
    if not args.meshfilename:
        raise Exception('No .meshb file provided. Run with -h for full list of options.\n')

    # Solution required
    if not args.solfilename:
        raise Exception('No .solb file provided. Run with -h for full list of options.\n')
        sys.stdout.flush()

    sol_to_met( args.meshfilename ,
                args.solfilename  ,
    	        args.outfilename  )

#: def main()

# -------------------------------------------------------------------
#  Mesh and Solution Conversion Function
# -------------------------------------------------------------------

def sol_to_met( mshfilename ,
                solfilename ,
                outfilename ):

	amgio.py_ConvertGMFSoltoMet(mshfilename, solfilename, outfilename)

#: def sol_to_met()

# -------------------------------------------------------------------
#  Run Main Program
# -------------------------------------------------------------------

# this is only accessed if running from command prompt
if __name__ == '__main__':
    main()