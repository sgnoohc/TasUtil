#!/bin/bash

PACKAGE=package.tar.gz

###################################################################################################
# ProjectMetis/CondorTask specific (Setting up some common environment)
###################################################################################################
if [ "x${_CONDOR_SLOT}" == "x" ]; then
    :
else
    OUTPUTDIR=$1
    OUTPUTNAME=$2
    INPUTFILENAMES=$3
    IFILE=$4
    CMSSWVERSION=$5
    SCRAMARCH=$6
    shift 6
    tar xvzf package.tar.gz
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
if [ "x${_CONDOR_SLOT}" == "x" ]; then
    if [ -n "$5" ]; then INPUTFILENAMES=$5; fi
else
    :
fi

# echo current settings
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
        time root -l -b -q 'run.C("'${SCANCHAINNAME}'","'${INPUTFILENAMES}'","'${INPUTTTREENAME}'","'${OUTPUTROOTNAME}'","'${NEVENTS}'", "'${COMPILERFLAG}'")'
    fi
fi

#------------=================-----------------===============---------------==============--------
# End execution
#------------=================-----------------===============---------------==============--------




###################################################################################################
# ProjectMetis/CondorTask specific (Copying files over to hadoop)
###################################################################################################
if [ "x${_CONDOR_SLOT}" == "x" ]; then
    :
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
    gfal-copy -p -f -t 4200 --verbose file://`pwd`/${INFILE} gsiftp://gftp.t2.ucsd.edu/${OUTPUTDIR}/${OUTPUTNAME}_${IFILE}.root --checksum ADLER32
    if [ $? -eq 0 ]; then
        echo "Job Success"
    else
        echo "Job Failed"
    fi
fi
###################################################################################################

#eof
