#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

usage()
{
    echo "Usage: sh $(basename $0) scanchain lepbaby.root t output.root nevents [HADOOPDIRFORCONDOR]"
    exit
}

if [ -z $1 ]; then usage; fi
if [ -z $2 ]; then usage; fi
if [ -z $3 ]; then usage; fi
if [ -z $4 ]; then usage; fi
if [ -z $5 ]; then usage; fi

source $DIR/root.sh ""

if [ "x${_CONDOR_SLOT}" == "x" ]; then
    :
else
    tar xvzf condor.tgz
fi

root -l -b -q 'run.C("'$1'","'$2'","'$3'","'$4'","'$5'")'

if [ "x${_CONDOR_SLOT}" == "x" ]; then
    :
else
    echo 'ls -l'
    ls -l
    echo 'gfal-copy'
    INFILE=$4
    OUTDIR=$6
    OUTFILE=${OUTDIR}/${INFILE}
    HADOOPDIR=/hadoop/cms/store/user/phchang/
    echo gfal-copy -p -f -t 4200 --verbose file://\`pwd\`/${INFILE} gsiftp://gftp.t2.ucsd.edu/${HADOOPDIR}/${OUTFILE} --checksum ADLER32
    hostname
    uname -a
    date
    whoami
    pwd
    gfal-copy -p -f -t 4200 --verbose file://`pwd`/${INFILE} gsiftp://gftp.t2.ucsd.edu/${HADOOPDIR}/${OUTFILE} --checksum ADLER32
    if [ $? -eq 0 ]; then
        echo "Job Success"
    else
        echo "Job Failed"
    fi
fi

#eof
