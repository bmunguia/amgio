#!/usr/bin/env python

## \file su2_to_gmf.py
#  \brief python script to convert SU2 files to GMF
#  \author Brian Mungu\'ia
#

import sys
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
                        help="input SU2 mesh with ext", 
                        metavar="MSH")
    parser.add_argument("-s", "--sol", 
                        dest="solfilename", 
                        type=str,
                        help="input SU2 solution with ext", 
                        metavar="SOL")
    parser.add_argument("-o", "--out", 
                        dest="outfilename", 
                        type=str, 
                        default="out",
                        help="base filename for output GMF mesh/solution\n"
                             "outputs OUT.solb and OUT.meshb",
                        metavar="OUT")
    parser.add_argument("-f", "--fld", 
                        dest="fieldname", 
                        type=str, 
                        default="all",
                        help="fields to write, supported options:\n"  
                             "all (D), <sensor name>, metric", 
                        metavar="FLD")
    args=parser.parse_args()

    # Mesh required
    if not args.meshfilename:
        raise Exception('No .meshb file provided. Run with -h for full list of options.\n')

    # Solution not required, but user should know
    if not args.solfilename:
        args.solfilename = ""
        sys.stdout.write("No input solution provided. Only converting mesh.\n")
        sys.stdout.flush()

    args.fieldname = args.fieldname.lower()

    su2_to_gmf( args.meshfilename ,
    	        args.solfilename  ,
    	        args.outfilename  ,
                args.fieldname    )

#: def main()

# -------------------------------------------------------------------
#  Mesh and Solution Conversion Function
# -------------------------------------------------------------------

def su2_to_gmf( meshfilename ,
                solfilename  ,
                outfilename  ,
                fieldname    ):

    # if (fieldname == 'metric'): # TODO
    #     su2gmf.SU2toGMF(meshfilename, solfilename, outfilename)
    # else:
    su2gmf.SU2toGMF(meshfilename, solfilename, outfilename, fieldname)

#: def su2_to_gmf()

# -------------------------------------------------------------------
#  Run Main Program
# -------------------------------------------------------------------

# this is only accessed if running from command prompt
if __name__ == '__main__':
    main()