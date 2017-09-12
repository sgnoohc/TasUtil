#!/bin/env python

# Load Utils package.
# If can't, then git checkout.
import os
import sys
os.system('mkdir -p /tmp/condor_concierge')
sys.path.append('/tmp/condor_concierge')
try:
    import Utils
except:
    os.system('wget http://raw.githubusercontent.com/aminnj/ProjectMetis/master/metis/Utils.py -O /tmp/condor_concierge/Utils.py &> /dev/null')
    import Utils

def check_argument(arg):
    print "condor_concierge.py: error found in the provided arguments."
    if arg:
        print "                  [Hint:] Could not find " + arg
    print "Try 'condor_concierge.py -h' for more information"
    sys.exit()

import argparse
parser = argparse.ArgumentParser(description=
"""
###################################################################
#                                                                 #
#                   Condor Submission Concierge                   #
#                        ----------------                         #
#                                                                 #
###################################################################
""",
formatter_class=argparse.RawTextHelpFormatter,
usage="%(prog)s action [options]\nTry 'condor_concierge.py -h' for more information."
)

# Positional arguments
#______________________________________________________________________________________________
parser.add_argument('action', action='store', help=
"""Available actions:

===========================================
                 submit
===========================================

1. Submitting one job.

     --executable, -e
    [--arguments, -a]
    [--package, -p]
    [--logdirpath, -l]
    [--taskname, -t]
    [--jobindex, -i]
    [--force, -f]

    e.g. $> condor_concierge.py submit \\
            --executable exec.sh \\
           [--arguments '/input/file.root,/output/file.root'] \\
           [--package package.tgz,extra.cfg] \\
           [--logdirpath ./] \\
           [--jobindex 117]

2. Submitting a list of jobs.

     --executable, -e
     --joblist, -j
    [--package, -p]
    [--logdirpath, -l]
    [--excludepattern, -x]
    [--requirepattern, -q]
    [--force, -f]
    
    e.g. $> condor_concierge.py submit_batch \\
            --executable exec.sh \\
            --joblist jobs.txt \\
           [--package package.tgz,extra.cfg] \\
           [--logdirpath ./condor_log] \\
           [-x ttbar] \\
           [-q wjets]

""")

# Options
#______________________________________________________________________________________________
parser.add_argument('--executable', '-e', action='store', dest='executable', help=
"""
Provide the path to executable file for the condor job.

""")

parser.add_argument('--arguments', '-a', action='store', default='', dest='arguments', help=
"""
Comma separated list of arguments to the executable for the condor job.

""")

parser.add_argument('--joblist', '-j', action='store', dest='joblist', help=
"""
Comma separated list of text files that list each job arguments with space separation.
e.g.
$> cat job.txt
/hadoop/cms/store/user/me/ntuples/file_1.root output_1.root do_analysis make_plot
/hadoop/cms/store/user/me/ntuples/file_2.root output_2.root do_analysis make_plot
...
...
...
/hadoop/cms/store/user/me/ntuples/file_117.root output_117.root do_analysis make_plot
...
...

""")

parser.add_argument('--excludepattern', '-x', action='store', default='', dest='excludepattern', help=
"""
Pattern to exclude from the --joblist.
If a line in the --joblist file mathces the pattern provided by -x option, skip the job.

""")

parser.add_argument('--requirepattern', '-q', action='store', default='', dest='requirepattern', help=
"""
Pattern to requirefrom the --joblist.
If a line in the --joblist file mathces the pattern provided by -x option, submit the job.
Lines not matching our not submitted.

""")

parser.add_argument('--package', '-p', action='store', default='', dest='package', help=
"""
Comma separated list of input files to be sent to the condor nodes.

""")

parser.add_argument('--logdirpath', '-l', action='store', default='./condor_logs', dest='logdirpath', help=
"""
Path to the directory here condor log files will be saved.

""")

parser.add_argument('--taskname', '-t', action='store', default='job', dest='taskname', help=
"""
Name of the task

""")

parser.add_argument('--jobindex', '-i', action='store', default='0', dest='jobindex', help=
"""
Job index for this task

""")

parser.add_argument('--force', '-f', action='store_true', default=False, dest='force', help=
"""
To force the submission and not check condor_logs path

""")

# Option 
args = parser.parse_args()





#______________________________________________________________________________________________
# condor_submit
if args.action == 'submit':

    # Sanity checks on minimal argument requirement
    if not args.executable    : check_argument("--executable")
    if not args.package : check_argument("--package")

    # If no logdirpath provided check whether something already exists
    if not args.force:
        if os.path.isdir(args.logdirpath):
            print "condor_concierge.py: error in the `--logdirpath="+args.logdirpath+"` option."
            print "A directory called "+args.logdirpath+" already exists. Please delete the directory before proceeding. (e.g. $> rm -r "+args.logdirpath+")"
            print "Try 'condor_concierge.py -h' for more information."
            print "Cowardly exiting..."
            sys.exit(-1)

    ###########################################################################################
    # If --joblist string is empty it means, we're submitting single job
    if not args.joblist:

        print "Submitting a single job..."
        condorjobid = Utils.condor_submit(
            executable=args.executable,
            arguments=args.arguments.split(','),
            package=args.package.split(','),
            logdir=args.logdirpath,
            selection_pairs=[["taskname",args.taskname],["jobnum",args.jobindex]],
            sites="UAF,T2_US_UCSD",
            fake=False)

    ###########################################################################################
    # If --joblist string is not empty, we're submitting a list of jobs.
    else:

        print "Submitting a list of jobs..."
        # First open the --joblist file
        for joblistfilepath in args.joblist.split(','):

            joblist_file = file(joblistfilepath)
            joblist = [ line for line in (l.strip() for l in joblist_file.readlines()) if line ]

            taskname = os.path.splitext(os.path.basename(joblistfilepath))[0]

            print "  Submitting a list of jobs from a list of arguments in file=",joblistfilepath
            for index, argset in enumerate(joblist):

                skip = False
                if args.excludepattern:
                    for patt in args.excludepattern.split(','):
                        if argset.find(patt) != -1:
                            skip = True

                if args.requirepattern:
                    for patt in args.requirepattern.split(','):
                        if argset.find(patt) == -1:
                            skip = True

                if skip:
                    continue

                condorjobid = Utils.condor_submit(
                    executable=args.executable,
                    arguments=argset.split(),
                    inputfiles=args.package.split(','),
                    logdir=args.logdirpath,
                    selection_pairs=[["taskname",taskname],["jobnum",index]],
                    sites="UAF,T2_US_UCSD",
                    fake=False)

#eof
