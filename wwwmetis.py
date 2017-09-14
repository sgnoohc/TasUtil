import time
import json

from metis.Sample import DirectorySample
from metis.CondorTask import CondorTask

from metis.StatsParser import StatsParser

import sys
import os
import glob
import subprocess

default_babyver = "11" # this is str

def usage( errormsg ):
    print "usage: python %s"%( os.path.basename( sys.argv[0] ) )
    print errormsg
    sys.exit()

# Parsing arguments
try:
    # Get git versions
    tasutilgittag = subprocess.check_output( "git log --pretty=format:'%h' -n 1", shell=True )
    os.chdir( "WWW_CORE" )
    wwwcoregittag = subprocess.check_output( "git log --pretty=format:'%h' -n 1", shell=True )
    os.chdir( ".." )
    job_tag = "tasutil_%s_wwwcore_%s"%( tasutilgittag, wwwcoregittag )
    # parse the executable path argument
    baby_version = default_babyver
    exec_path = "run.sh"
    tar_path = "package.tar.gz"
    hadoop_path = "test/wwwlooper/%s"%job_tag
#    hadoop_path = "../../../../../home/users/phchang/public_html/tasutil/outputs/%s"%job_tag
    args = "WWW_ScanChain output.root t -1"
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
                    #location="/nfs-7/userdata/bhashemi/WWW_babies/WWW_v0.1.%s/skim/"%baby_version,
                    location="/home/users/phchang/public_html/tasutil/babies/www/skim/WWW_v0.1.11_v2",
                    globber="*.root"
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
                condor_submit_params = {"sites" : "UAF"}
                )

        task.process()

        # save some information for the dashboard
        total_summary["WWW_v0_1_%s_%s"%(baby_version, job_tag)] = task.get_task_summary()

        # parse the total summary and write out the dashboard
        StatsParser( data=total_summary, webdir="~/public_html/tasutil/Metis_WWW/" ).do()

        if task.complete():
            print ""
            print "Job=%s finished"%job_tag
            print ""
            break

        time.sleep(30)


#eof

#        taskhadoop = CondorTask(
#                sample = DirectorySample(
#                    dataset="/WWW_v0_1_%s_hadoop"%(baby_version),
#                    location="/hadoop/cms/store/user/bhashemi/AutoTwopler_babies/merged/VVV/WWW_v0.1.%s/skim/"%baby_version,
#                    globber="*.root"
#                    ),
#                open_dataset = False,
#                flush = True,
#                files_per_output = 6,
#                output_name = "merged.root",
#                tag = job_tag,
#                #cmssw_version = "CMSSW_9_2_1", # doesn't do anything
#                arguments = args,
#                executable = exec_path,
#                tarfile = tar_path,
#                special_dir = hadoop_path
#                )
#
#        taskhadoop.process()
#
#        # save some information for the dashboard
#        total_summary["WWW_v0_1_%s_hadoop_%s"%(baby_version, job_tag)] = taskhadoop.get_task_summary()

# and taskhadoop.complete():

