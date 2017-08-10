#!/bin/bash

PACKAGE=package.tar.gz

###################################################################################################
# ProjectMetis/CondorTask specific (Setting up some common environment)
###################################################################################################
#echo "To check whether it's on condor universe Vanilla or Local. The following variables are used."
#echo "If _CONDOR_SLOT is set, it's on Vanilla"
#echo "If X509_USER_PROXY is set, it's either on Vanilla or Local."
#echo "_CONDOR_SLOT" ${_CONDOR_SLOT}
#echo "X509_USER_PROXY" ${X509_USER_PROXY}
# if 
if [ "x${X509_USER_PROXY}" == "x" ]; then
    :
else
    OUTPUTDIR=$1
    OUTPUTNAME=$2
    INPUTFILENAMES=$3
    IFILE=$4
    CMSSWVERSION=$5
    SCRAMARCH=$6
    if [ "x${_CONDOR_SLOT}" == "x" ]; then
        WORKDIR=/tmp/phchang_condor_local_${OUTPUTDIR//\//_}_${OUTPUTNAME}_${IFILE}
        mkdir -p ${WORKDIR}
        ls
        cp package.tar.gz ${WORKDIR}
        cd ${WORKDIR}
        ls
        echo "This is in Condor session with Universe=Local."
        echo "WORKDIR=${WORKDIR}"
        echo "pwd"
        pwd
    fi
    echo "OUTPUTDIR     : $1"
    echo "OUTPUTNAME    : $2"
    echo "INPUTFILENAMES: $3"
    echo "IFILE         : $4"
    echo "CMSSWVERSION  : $5"
    echo "SCRAMARCH     : $6"
    shift 6
    tar xvzf package.tar.gz
    if [ $? -eq 0 ]; then
        echo "Successfully untarred package."
        :
    else
        echo "Failed to untar package."
        exit
    fi
fi
###################################################################################################



#------------=================-----------------===============---------------==============--------
# Begin execution
#------------=================-----------------===============---------------==============--------

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

usage()
{
    echo "Usage: sh $(basename $0) [-p] [-g] [-c] scanchain output.root t nevents cms3_1,cms3_2.root"
    echo ""
    echo "  -g option runs in gdb"
    echo "  -c option forces recompilation"
    echo ""
    exit
}

# Command-line opts
while getopts ":gpch" OPTION; do
  case $OPTION in
    g) DEBUG=true;;
    p) PERF=true;;
    c) FORCERECOMPILE="+";;
    h) usage;;
    :) usage;;
  esac
done

# Setup the same root I want
source ./root.sh "" &> /dev/null

# to shift away the parsed options
shift $(($OPTIND - 1))

# Default arguments
NEVENTS=-1

# Parse arguments
if [ -z "$1" ]; then usage; fi
if [ -z "$2" ]; then usage; fi
if [ -z "$3" ]; then usage; fi
SCANCHAINNAME=$1
OUTPUTROOTNAME=$2
INPUTTTREENAME=$3
if [ -n "$4" ]; then NEVENTS=$4; fi

# Parse the input file names differently depending on whether it's condor job or local job.
if [ "x${X509_USER_PROXY}" == "x" ]; then
    if [ -n "$5" ]; then INPUTFILENAMES=$5; fi
else
    # If condor jobs, touch the .so files to prevent from recompiling
    touch *.so
fi

# echo current settings
echo "==============================================================================="
echo "$(basename $0) $*"
echo "==============================================================================="
echo "SCANCHAINNAME  = $SCANCHAINNAME"
echo "OUTPUTROOTNAME = $OUTPUTROOTNAME"
echo "INPUTTTREENAME = $INPUTTTREENAME"
echo "NEVENTS        = $NEVENTS"
echo "INPUTFILENAMES = $INPUTFILENAMES"
echo "==============================================================================="

# Run the job!
if [ "${DEBUG}" == true ]; then
    COMPILERFLAG=+g
    gdb --args root.exe -l -b -q 'run.C("'${SCANCHAINNAME}'","'${INPUTFILENAMES}'","'${INPUTTTREENAME}'","'${OUTPUTROOTNAME}'","'${NEVENTS}'", "'${COMPILERFLAG}'")'
else

    if [ "${PERF}" == true ]; then
        COMPILERFLAG=+g
        root.exe -l -b -q 'run.C("'${SCANCHAINNAME}'","'${INPUTFILENAMES}'","'${INPUTTTREENAME}'","'${OUTPUTROOTNAME}'","'1'", "'${COMPILERFLAG}'")'
        COMPILERFLAG=g
        igprof -pp -d -z -o igprof.pp.gz root.exe -l -b -q 'run.C("'${SCANCHAINNAME}'","'${INPUTFILENAMES}'","'${INPUTTTREENAME}'","'${OUTPUTROOTNAME}'","'${NEVENTS}'", "'${COMPILERFLAG}'")'
        igprof-analyse --sqlite -d -v -g igprof.pp.gz | sqlite3 igprof.pp.sql3 >& /dev/null
        cp igprof.pp.sql3 ~/public_html/cgi-bin/data/
        echo "http://${HOSTNAME}/~phchang/cgi-bin/igprof-navigator.py/igprof.pp/"
    else
        COMPILERFLAG=${FORCERECOMPILE}O
        touch *.so
        echo root -l -b -q 'run.C("'${SCANCHAINNAME}'","'${INPUTFILENAMES}'","'${INPUTTTREENAME}'","'${OUTPUTROOTNAME}'","'${NEVENTS}'", "'${COMPILERFLAG}'")'
        time root -l -b -q 'run.C("'${SCANCHAINNAME}'","'${INPUTFILENAMES}'","'${INPUTTTREENAME}'","'${OUTPUTROOTNAME}'","'${NEVENTS}'", "'${COMPILERFLAG}'")'
    fi
fi

#------------=================-----------------===============---------------==============--------
# End execution
#------------=================-----------------===============---------------==============--------




###################################################################################################
# ProjectMetis/CondorTask specific (Copying files over to hadoop)
###################################################################################################
if [ "x${X509_USER_PROXY}" == "x" ]; then
    :
else
    if [[ ${OUTPUTDIR} == *"home/users/"* ]]; then
        mkdir -p ${OUTPUTDIR}
        INFILE=${OUTPUTROOTNAME}
        cp ${INFILE} ${OUTPUTDIR}/${OUTPUTNAME}_${IFILE}.root
    else
        echo 'ls -l'
        ls -l
        echo 'gfal-copy'
        INFILE=${OUTPUTROOTNAME}
        echo gfal-copy -p -f -t 4200 --verbose file://`pwd`/${INFILE} gsiftp://gftp.t2.ucsd.edu/${OUTPUTDIR}/${OUTPUTNAME}_${IFILE}.root --checksum ADLER32
        hostname
        uname -a
        date
        whoami
        pwd
        echo "ls'ing hadoop"
        ls -lh /hadoop/cms/store/user/phchang/
        gfal-copy -p -f -t 4200 --verbose file://`pwd`/${INFILE} gsiftp://gftp.t2.ucsd.edu/${OUTPUTDIR}/${OUTPUTNAME}_${IFILE}.root --checksum ADLER32
    fi
    if [ $? -eq 0 ]; then
        echo "Job Success"
    else
        echo "Job Failed"
    fi
fi
###################################################################################################

#eof
