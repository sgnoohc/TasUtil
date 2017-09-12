import time
import json

from metis.Sample import DirectorySample
from metis.CondorTask import CondorTask

from metis.StatsParser import StatsParser

import sys
import os
import glob

def usage(errormsg):
    print "usage: python %s JOBTAG"%(os.path.basename(sys.argv[0]))
    print errormsg
    sys.exit()

# Parsing arguments
try:
    # parse the executable path argument
    job_tag = sys.argv[1]
    exec_path = "run.sh"
    tar_path = "package.tar.gz"
    hadoop_path = "test/IsoML_baby/%s"%job_tag
#    hadoop_path = "../../../../../home/users/phchang/public_html/tasutil/outputs/%s"%job_tag
    args = "IsoML_ScanChain output.root Events -1"
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
                    dataset="/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
                    location="/hadoop/cms/store/group/snt/run2_moriond17/TT_TuneCUETP8M2T4_13TeV-powheg-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/V08-00-16/"
                    ),
                open_dataset = False,
                flush = True,
                files_per_output = 1,
                output_name = "merged.root",
                tag = job_tag,
                #cmssw_version = "CMSSW_9_2_1", # doesn't do anything
                arguments = args,
                executable = exec_path,
                tarfile = tar_path,
                special_dir = hadoop_path,
                #condor_submit_params = {"universe" : "local"}
                #condor_submit_params = {"sites" : "UAF"}
                )

        task.process()

        # save some information for the dashboard
        total_summary["IsoML_baby"] = task.get_task_summary()

        # parse the total summary and write out the dashboard
        StatsParser( data=total_summary, webdir="~/public_html/tasutil/Metis_IsoML/" ).do()

        if task.complete():
            print ""
            print "Job=%s finished"%job_tag
            print ""
            break

        time.sleep(30)


#eof
