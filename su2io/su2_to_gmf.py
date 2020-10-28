#!/usr/bin/env python

## \file su2_to_gmf.py
#  \brief python script to convert SU2 files to GMF
#  \author Brian Mungu\'ia
#

import sys
import _amgio as amgio
from argparse import ArgumentParser, RawTextHelpFormatter

# -------------------------------------------------------------------
#  Main 
# -------------------------------------------------------------------

def main(): 

    # Command Line Options
    parser = ArgumentParser(formatter_class=RawTextHelpFormatter)
    parser.add_argument("-m", "--mesh", 
                        dest="meshfilename", 
                        type=str,
                        help="input SU2 format mesh (ext required)", 
                        metavar="meshfile")
    parser.add_argument("-s", "--sol", 
                        dest="solfilename", 
                        type=str,
                        help="input SU2 format solution (ext required)", 
                        metavar="solfile")
    parser.add_argument("-o", "--out", 
                        dest="outfilename", 
                        type=str, 
                        default="out",
                        help="output GMF mesh/solution (ext NOT required)",
                        metavar="outfile")
    parser.add_argument("-f", "--field", 
                        dest="fieldname", 
                        type=str, 
                        default="all",
                        help="fields to write, supported options:\n"  
                             "all (D), <sensor name>, metric", 
                        metavar="field")
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

    su2_to_gmf(args.meshfilename,
    	       args.solfilename,
    	       args.outfilename,
               args.fieldname)

#: def main()

# -------------------------------------------------------------------
#  Mesh and Solution Conversion Function
# -------------------------------------------------------------------

def su2_to_gmf( meshfilename ,
                solfilename  ,
                outfilename  ,
                fieldname    ):

    # if (fieldname == 'metric'): # TODO
    #     amgio.py_ConvertSU2toGMF(meshfilename, solfilename, outfilename)
    # else:
    amgio.py_ConvertSU2toGMF(meshfilename, solfilename, outfilename, fieldname)

#: def su2_to_gmf()

# -------------------------------------------------------------------
#  Run Main Program
# -------------------------------------------------------------------

# this is only accessed if running from command prompt
if __name__ == '__main__':
    main()