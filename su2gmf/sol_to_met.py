#!/usr/bin/env python

## \file sol_to_met.py
#  \brief python script to convert GMF solution files to metric files
#  \author Brian Mungu\'ia
#

import su2gmf
from argparse import ArgumentParser, RawTextHelpFormatter

# -------------------------------------------------------------------
#  Main 
# -------------------------------------------------------------------

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
    parser.add_argument("-o", "--out", 
                        dest="outfilename", 
                        type=str, 
                        default="out",
                        help="base filename for output GMF metric\n"
                             "outputs OUT.solb",
                        metavar="OUT")
    args=parser.parse_args()

    # Mesh required
    if not args.meshfilename:
        raise Exception('No .meshb file provided. Run with -h for full list of options.\n')

    # Solution required
    if not args.solfilename:
        raise Exception('No .solb file provided. Run with -h for full list of options.\n')

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

	su2gmf.GMFSoltoMet(mshfilename, solfilename, outfilename)

#: def sol_to_met()

# -------------------------------------------------------------------
#  Run Main Program
# -------------------------------------------------------------------

# this is only accessed if running from command prompt
if __name__ == '__main__':
    main()