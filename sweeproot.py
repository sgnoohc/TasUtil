#!/cvmfs/cms.cern.ch/slc6_amd64_gcc530/cms/cmssw/CMSSW_8_0_18/external/slc6_amd64_gcc530/bin/python

# Rigorous sweeproot which checks ALL branches for ALL events.
# If GetEntry() returns -1, then there was an I/O problem, so we will delete it

import argparse
parser = argparse.ArgumentParser(description=
"""
###################################################################
#                                                                 #
#                      Rigorous Sweep Root                        #
#                        ----------------                         #
#                                                                 #
###################################################################
""",
formatter_class=argparse.RawTextHelpFormatter,
usage="%(prog)s action [options]\nTry 'sweeproot.py -h' for more information."
)

# Positional arguments
parser.add_argument('filepath', action='store', help="The root file to check integrity.")
parser.add_argument('treename', action='store', help="The TTree name in the given ROOT file to check")

# Parse
args = parser.parse_args()

import os
import ROOT as r

foundBad = False

try:
    f1 = r.TFile(args.filepath)
    t = f1.Get(args.treename)
    nevts = t.GetEntries()
    for i in range(0, t.GetEntries(), 1):
        if t.GetEntry(i) < 0:
            foundBad = True
            print "[RSR] found bad event %i" % i
            break
except:
    foundBad = True

if foundBad:
    print "[RSR] removing output file because it does not deserve to live"
    os.system("rm %s" % args.filepath)
else:
    print "[RSR] passed the rigorous sweeproot"
