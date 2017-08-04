#!/bin/env python

import sys
import os
import datetime
import ROOT as r

r.gROOT.SetBatch( True )

def usage():
    print "usage: python %s NLASTJOBS PATTERN"
    print ""
    print "   NLASTJOBS = to limit number of jobs when querying condor_history. (see condor_history option '-l')"
    print "   PATTERN   = grep pattern"
    sys.exit()

try:
    njobs = sys.argv[1]
    pttn = sys.argv[2]
except:
    usage()

def get_time( date, time ):
    # in condor history format is
    # date = MM/DD time = HH:MM
    # no year provided (just assume same year)
    datesplit = date.split( '/' )
    month = int( datesplit[0] )
    day = int( datesplit[1] )
    timesplit = time.split( ':' )
    hour = int( timesplit[0] )
    minute = int( timesplit[1] )
    return datetime.datetime( 2000, month, day, hour, minute )

def get_run_time( run_time_str ):
    tokens = run_time_str[2:].split( ':' )
    day = int( run_time_str[0] )
    hour = int( tokens[0] )
    minute = int( tokens[1] )
    second = int( tokens[2] )
    t_hour = hour * 60
    t_minute = minute
    t_second = float( second / 60. )
    run_time_in_mins = t_hour + t_minute + t_second
    return run_time_in_mins


if __name__ == "__main__":

    h_total_time_mins = r.TH1F( "TotalMins", "Total Time in Minutes", 100, 0, 5 )
    h_total_time_mins.GetXaxis().SetTitle( "Total time in minutes" )
    h_run_time_mins = r.TH1F( "RunMins", "Run Time in Minutes", 100, 0, 5 )
    h_run_time_mins.GetXaxis().SetTitle( "Run time in minutes" )
    
    lines = os.popen( "condor_history -limit %s %s | grep %s"%( njobs, os.getenv( "USER" ), pttn ) ).read().split( '\n' )
    
    for line in lines:
        if len( line ) == 0:
            continue
        ls = line.split()
        starttime = get_time( ls[2], ls[3] )
        endtime = get_time( ls[6], ls[7] )
        length = endtime - starttime
        total_time_mins = length.total_seconds() / 60.
        h_total_time_mins.Fill( total_time_mins )
        h_run_time_mins.Fill( get_run_time( ls[4] ) )
        get_run_time( ls[4] )
    
        print ls[2], ls[3], ls[4], ls[5], ls[6], ls[7]

    c1 = r.TCanvas( "", "", 0, 0, 800, 800 )
    h_total_time_mins.Draw( "hist" )
    c1.SaveAs( "condor_total_time.png" )
    h_run_time_mins.Draw( "hist" )
    c1.SaveAs( "condor_run_time.png" )

    os.system("chmod 644 *.png")
