import time
import json

from Sample import DirectorySample
from CondorTask import CondorTask

from StatsParser import StatsParser

import sys
import os
import glob

default_babyver = "11" # this is str

def usage(errormsg):
    print "usage: python %s JOBTAG [BABYVERSION=%s]"%(os.path.basename(sys.argv[0]), default_babyver)
    print errormsg
    sys.exit()

# Parsing arguments
try:
    # parse the executable path argument
    job_tag = sys.argv[1]
    if len( sys.argv ) == 2:
        baby_version = default_babyver
    exec_path = "run.sh"
    tar_path = "condor.tgz"
    hadoop_path = "test/wwwlooper/%s"%job_tag
    args = "ScanChain_WWW output.root t -1"
except:
    usage("error: check your arguments.")

def parseSampleList(pathtolist):
    samplelist = {}
    dsname = ""
    filelist = open(pathtolist)
    lines = filelist.readlines()
    for line in lines:
        lstr = line.strip()
        if len(lstr) == 0:
            continue
        if lstr.find("#") != -1:
            dsname = lstr[1:].strip()
            samplelist[dsname] = []
        else:
            samplelist[dsname].append([lstr.split()[0], lstr.split()[1]])
    return samplelist


if __name__ == "__main__":

    total_summary = {}

    while True:

        task = CondorTask(
                sample = DirectorySample(
                    dataset="/WWW_v0_1_%s"%(baby_version),
                    location="/nfs-7/userdata/bhashemi/WWW_babies/WWW_v0.1.%s/skim/"%baby_version,
                    globber="*.root"
                    ),
                open_dataset = False,
                flush = True,
                files_per_output = 9,
                output_name = "merged.root",
                tag = job_tag,
                cmssw_version = "CMSSW_9_2_1", # doesn't do anything
                arguments = args,
                executable = exec_path,
                tarfile = tar_path,
                special_dir = hadoop_path
                )

        task.process()

        # save some information for the dashboard
        total_summary["WWW_v0_1_%s_%s"%(baby_version, job_tag)] = task.get_task_summary()

        # parse the total summary and write out the dashboard
        StatsParser( data=total_summary, web_summary_fname="www_web_summary.json", webdir="~/public_html/tasutil/Metis_WWW/" ).do()

        if task.complete():
            print ""
            print "Job=%s finished"%job_tag
            print ""
            break

        time.sleep(2*60)
