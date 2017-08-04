import time
import json

from Sample import DirectorySample
from CondorTask import CondorTask

from statsparser import write_web_summary

import sys
import os

def usage(errormsg):
    print "usage: python babymetis.py EXECPATH TARPATH RELATIVEHADOOPPATH JOBTAG ARG1TOEXEC ARG2TOEXEC ... "
    print errormsg
    sys.exit()

# Parsing arguments
try:

    # parse the executable path argument
    exec_path = sys.argv[1]
    print "EXECPATH=%s"%(exec_path)
    print "Printing help information for exec_path"
    print "==============================================="
    os.system("sh %s -h"%exec_path) # to print argument help
    print "==============================================="

    # parse the rest
    tar_path = sys.argv[2]
    hadoop_path = sys.argv[3]
    job_tag = sys.argv[4]
    args = " ".join(sys.argv[5:])
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
            dsname = datasetname[1:]
            samplelist[dsname] = []
        else:
            samplelist[dsname].append([lstr.split()[0], lstr.split()[1]])
    return samplelist


if __name__ == "__main__":

    print parseSampleList("configs/samplelist/Run2Samples25nsMoriond17.cms3.list")
    sys.exit()

    dslocs = [
            ["/EWK_LNuJJ_MJJ-120_13TeV-madgraph-pythia8/RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/MINIAODSIM",
                "/hadoop/cms/store/group/snt/run2_moriond17/EWK_LNuJJ_MJJ-120_13TeV-madgraph-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/V08-00-16/"]
            ]

    total_summary = {}

    for ds,loc in dslocs:
        task = CondorTask(
                sample = DirectorySample( dataset=ds, location=loc ),
                open_dataset = True,
                flush = True,
                files_per_output = 1,
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
        total_summary[ds] = task.get_task_summary()

    # parse the total summary and write out the dashboard
    write_web_summary(data=total_summary, webdir="~/public_html/dump/metis_test/")
