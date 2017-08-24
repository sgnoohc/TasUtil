#!/bin/bash

# vim: tabstop=2:softtabstop=2:shiftwidth=2:expandtab

#  .
# ..: P. Chang, philip@physics.ucsd.edu

cores=36

# must provide the job
if [ "x${1}" == "x" ]; then
  echo "Must provide the job commands txt file"
  exit
fi

JOBTXTFILE=$1

# https://stackoverflow.com/questions/30053734/remove-newline-from-end-of-string-in-bash-line-continuations
perl -pe 's/\\$// and chomp' < $1 > /tmp/tmpjobs.txt

# filter some jobs
if [ "x${2}" != "x" ]; then
  cat /tmp/tmpjobs.txt | grep -v \# | grep $2 > /tmp/jobs.txt
else
  cat /tmp/tmpjobs.txt | grep -v \# > /tmp/jobs.txt
fi

# run the job in parallel
xargs --arg-file=/tmp/jobs.txt \
      --max-procs=$cores  \
      --replace \
      --verbose \
      /bin/sh -c "{}"

#eof
