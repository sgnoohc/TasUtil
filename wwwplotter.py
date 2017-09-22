#!/bin/env python

import sys
sys.path.append("/home/users/phchang/public_html/tasutil")
import plotmaker
import ROOT as r
from array import *
import glob
from multiprocessing import Pool as ThreadPool 

histtype = "TH2*"

jobtag = "TEST-v1"

proc_groups = {}
proc_groups["W"]        = [ "wj", "wg" ]
proc_groups["Z"]        = [ "dy", "zg" ]
proc_groups["tt1l"]     = [ "tt1l" ]
proc_groups["tt2l"]     = [ "tt2l" ]
proc_groups["WW"]       = [ "ww", "wwdpi", "vbsww" ]
proc_groups["WZ"]       = [ "wz" ]
proc_groups["ZZ"]       = [ "zz" ]
proc_groups["tX"]       = [ "singletop" ]
proc_groups["ttX"]      = [ "ttz", "ttw", "tth", "ttg" ]
proc_groups["VVV"]      = [ "wwz_incl", "wzz_incl", "zzz_incl"]#, "wwg_incl", "wzg_incl" ]
proc_groups["VH"]       = [ "vh" ]
proc_groups["Fake"]     = [ "fakepred" ]
proc_groups["WH"]       = [ "whwww" ]
proc_groups["WWW"]      = [ "www" ]
proc_groups["Data"]     = [ "data_mm", "data_em", "data_ee", "data_other" ]

proc_colors = {}
proc_colors["W"]        = 2006
proc_colors["Z"]        = 2010
proc_colors["tt1l"]     = 2005
proc_colors["tt2l"]     = 2008
proc_colors["WW"]       = 2007
proc_colors["WZ"]       = 2003
proc_colors["ZZ"]       = 2011
proc_colors["tX"]       = 2009
proc_colors["ttX"]      = 2004
proc_colors["VVV"]      = 616
proc_colors["VH"]       = 2002
proc_colors["Fake"]     = 2005
proc_colors["WH"]       = 4
proc_colors["WWW"]      = 2
proc_colors["Data"]     = 1

proc_categs = {}
proc_categs["W"]        = "bkg"
proc_categs["Z"]        = "bkg"
proc_categs["tt1l"]     = "bkg"
proc_categs["tt2l"]     = "bkg"
proc_categs["WW"]       = "bkg"
proc_categs["WZ"]       = "bkg"
proc_categs["ZZ"]       = "bkg"
proc_categs["tX"]       = "bkg"
proc_categs["ttX"]      = "bkg"
proc_categs["Fake"]     = "bkg"
proc_categs["VVV"]      = "bkg"
proc_categs["VH"]       = "bkg"
proc_categs["WH"]       = "sig"
proc_categs["WWW"]      = "sig"
proc_categs["Data"]     = "data"

bkg_groups = {}
bkg_groups["Prompt"]             = [ "trueSS" ]
bkg_groups["Q-flip"]             = [ "chargeflips" ]
bkg_groups["Lost-lep"]           = [ "SSLL" ]
bkg_groups["#gamma#rightarrowl"] = [ "photonfakes", "photondoublefakes", "photontriplefakes", "fakesphotonfakes", "otherphotonfakes" ]
bkg_groups["Fake"]               = [ "fakes", "doublefakes", "fakepred" ]
bkg_groups["Others"]             = [ "others" ]
bkg_groups["Data"]               = [ "data_mm", "data_em", "data_ee", "data_other" ]
bkg_groups["WWW"]                = [ "www" ]
bkg_groups["WH"]                 = [ "whwww" ]

bkg_colors = {}
bkg_colors["Prompt"]             = 2001
bkg_colors["Q-flip"]             = 2007
bkg_colors["Lost-lep"]           = 2003
bkg_colors["#gamma#rightarrowl"] = 2011
bkg_colors["Fake"]               = 2005
bkg_colors["Others"]             = 616
bkg_colors["Data"]               = 1
bkg_colors["WWW"]                = 2
bkg_colors["WH"]                 = 4

bkg_categs = {}
bkg_categs["Prompt"]             = "bkg"
bkg_categs["Q-flip"]             = "bkg"
bkg_categs["Lost-lep"]           = "bkg"
bkg_categs["#gamma#rightarrowl"] = "bkg"
bkg_categs["Fake"]               = "bkg"
bkg_categs["Others"]             = "bkg"
bkg_categs["Data"]               = "data"
bkg_categs["WWW"]                = "sig"
bkg_categs["WH"]                 = "sig"

tfile = r.TFile( "output.root" )

hists = {}

def drawbyproc( histname, extraoptions="" ):

    # Declare a map to hold the histograms that I access for this round of plotting
    hists = {}

    # Looping over MC sample process (e.g. W, ttbar, WZ, etc.)
    for key in proc_groups:

#            if key != "W": continue

        # the total background for this category is saved (e.g. W, ttbar, WZ, etc.)
        histsum = None

        # Loop over the individual that makes up the grouping.
        for proc in proc_groups[key]:

            # place holder
            hist = None

            # Form the name of the histogram
            histkey = proc + "__" + histname

            # Try to access it
            try :
                hist = hists[histkey]
            # If not get it from the tfile
            except:
                # Open it and save it to the self.hists which persists as long as this object persists.
                hists[histkey] = tfile.Get( histkey )

                # Get the pointer
                hist = hists[histkey]

            # If successfully retrieved
            if hist:

                # If already a clone was created copy the content over
                if histsum:
                    histsum.Add( hist )
                # If a clone has not been created yet, create one
                else:
                    histsum = hist.Clone( key )
                    histsum.SetDirectory( 0 )

        if key.find( "WZ" ) != -1:
            r.setNormSyst( histsum, 0.23 )

        if key.find( "Fake" ) != -1:
            r.setNormSyst( histsum, 0.3 )

        if key.find( "WW" ) != -1:
            r.setNormSyst( histsum, 0.2 )

        if key.find( "ttX" ) != -1:
            r.setNormSyst( histsum, 0.15 )

        # Once done looping over the grouping, set the histogram
        hists[key] = histsum

    # Now, we put them in std::vector<TH1*> so that we can pass it on to plotmaker.cc
    v_bkg_hists = r.vector("TH2*")()
    v_sig_hists = r.vector("TH2*")()
    v_data_hists = r.vector("TH2*")()

    # For the histograms we have stylize a bit
    for key in proc_categs:
#            if key != "W": continue
        if proc_categs[key] == "bkg"  and hists[key]:
            hists[key].SetLineColor( proc_colors[key] )
            hists[key].SetFillColor( proc_colors[key] )
            v_bkg_hists .push_back( hists[key] )
        if proc_categs[key] == "sig"  and hists[key]:
            hists[key].SetLineColor( proc_colors[key] )
            hists[key].SetLineWidth( 2 )
            v_sig_hists .push_back( hists[key] )
        if proc_categs[key] == "data" and hists[key]:
            hists[key].SetLineColor( 1 )
            v_data_hists.push_back( hists[key] )

    if histname.find( "SS" ) == 0:
        extraoptions += " --noData"

    # Then plot
    return r.plotmaker( """
                  --yTitle N leptons
                  --xTitle %s
                  --plotOutputName plots/%s
                  --ratio_Maximum 2
                  --ratio_Minimum 0.
                  --autoStack
                  --legend_NColumns 2
                  --MaximumMultiplier 1.2
                  %s
                  """%(histname, histname, extraoptions),
                  v_data_hists, v_bkg_hists, v_sig_hists )

def drawbyproc_helper(  args ):
    drawbyproc( *args )
    return True


def drawbytype(  histname, extraoptions="" ):

    # Declare a map to hold the histograms that I access for this round of plotting
    hists = {}

    # Looping over based on background categorization
    for bkgtype in bkg_groups:

        # the total background for this category is saved (e.g. trueSS, LL, fakes, etc.)
        histsum = None

        # Loop over the individual bkg type
        for bkg in bkg_groups[bkgtype]:

            # place holder
            hist = None

            # Form the name of the histogram
            histkey = "_" + bkg + "_" + histname

            # Try to access it
            try :
                hist = hists[histkey]
            # If not get it from the tfile
            except:
                # Open it and save it to the hists which persists as long as this object persists.
                hists[histkey] = tfile.Get( histkey )

                # Get the pointer
                hist = hists[histkey]

            # If successfully retrieved
            if hist:

                # If already a clone was created copy the content over
                if histsum:
                    histsum.Add( hist )
                # If a clone has not been created yet, create one
                else:
                    histsum = hist.Clone( bkgtype )
                    histsum.SetDirectory( 0 )

        if bkgtype.find( "Lost" ) != -1:
            r.setNormSyst( histsum, 0.23 )

        if bkgtype.find( "Fake" ) != -1:
            r.setNormSyst( histsum, 0.3 )

        if bkgtype.find( "Prompt" ) != -1:
            r.setNormSyst( histsum, 0.25 )

        # Once done looping over the grouping, set the histogram
        hists[bkgtype] = histsum

    # Now, we put them in std::vector<TH1*> so that we can pass it on to plotmaker.cc
    v_bkg_hists = r.vector("TH2*")()
    v_sig_hists = r.vector("TH2*")()
    v_data_hists = r.vector("TH2*")()

    # For the histograms we have stylize a bit
    for key in bkg_groups:
        if bkg_categs[key] == "bkg"  and hists[key]:
            hists[key].SetLineColor( bkg_colors[key] )
            hists[key].SetFillColor( bkg_colors[key] )
            v_bkg_hists .push_back( hists[key] )
        if bkg_categs[key] == "sig"  and hists[key]:
            hists[key].SetLineColor( bkg_colors[key] )
            hists[key].SetLineWidth( 2 )
            v_sig_hists .push_back( hists[key] )
        if bkg_categs[key] == "data" and hists[key]:
            hists[key].SetLineColor( 1 )
            v_data_hists.push_back( hists[key] )

    if histname.find( "SS" ) == 0:
        extraoptions += " --noData"

    # Then plot
    return r.plotmaker( """
                  --yTitle N leptons
                  --xTitle %s
                  --plotOutputName plots/%s_bytype
                  --ratio_Maximum 2
                  --ratio_Minimum 0.
                  --autoStack
                  --legend_NColumns 2
                  %s
                  --MaximumMultiplier 1.2
                  """%(histname, histname, extraoptions),
                  v_data_hists, v_bkg_hists, v_sig_hists )

def drawbytype_helper(  args ):
    drawbytype( *args )
    return True


def drawbyprocandtype(  histname, extraoptions="" ):

    # Declare a map to hold the histograms that I access for this round of plotting
    hists = {}

    # Looping over based on background categorization
    for proctype in proc_groups:


        # Looping over based on background categorization
        for bkgtype in bkg_groups:

            # the total background for this category is saved (e.g. trueSS, LL, fakes, etc.)
            histsum = None

            # Loop over the individual proc type
            for proc in proc_groups[proctype]:

                # Loop over the individual bkg type
                for bkg in bkg_groups[bkgtype]:

                    # place holder
                    hist = None

                    # Form the name of the histogram
                    histkey = proc + "_" + bkg + "_" + histname

                    # Try to access it
                    try :
                        hist = hists[histkey]
                    # If not get it from the tfile
                    except:
                        # Open it and save it to the hists which persists as long as this object persists.
                        hists[histkey] = tfile.Get( histkey )

                        # Get the pointer
                        hist = hists[histkey]

                    # If successfully retrieved
                    if hist:

                        # If already a clone was created copy the content over
                        if histsum:
                            histsum.Add( hist )
                        # If a clone has not been created yet, create one
                        else:
                            histsum = hist.Clone( proctype + "_" + bkgtype )
                            histsum.SetDirectory( 0 )

            # Once done looping over the grouping, set the histogram
            hists[proctype + "_" + bkgtype] = histsum

    # Now, we put them in std::vector<TH1*> so that we can pass it on to plotmaker.cc
    v_bkg_hists = r.vector("TH2*")()
    v_sig_hists = r.vector("TH2*")()
    v_data_hists = r.vector("TH2*")()

    # For the histograms we have stylize a bit
    for proc in proc_groups:
        for bkg in bkg_groups:
            key = proc + "_" + bkg
            if bkg_categs[bkg] == "bkg"  and hists[key]:
                hists[key].SetLineColor( bkg_colors[bkg] )
                hists[key].SetFillColor( bkg_colors[bkg] )
                v_bkg_hists .push_back( hists[key] )
            if bkg_categs[bkg] == "sig"  and hists[key]:
                hists[key].SetLineColor( bkg_colors[bkg] )
                hists[key].SetLineWidth( 2 )
                v_sig_hists .push_back( hists[key] )
            if bkg_categs[bkg] == "data" and hists[key]:
                hists[key].SetLineColor( 1 )
                v_data_hists.push_back( hists[key] )

    if histname.find( "SS" ) == 0:
        extraoptions += " --noData"

    # Then plot
    return r.plotmaker( """
                  --yTitle N leptons
                  --xTitle %s
                  --plotOutputName plots/%s_bytype
                  --ratio_Maximum 2
                  --ratio_Minimum 0.
                  --legend_NColumns 1
                  %s
                  --MaximumMultiplier 1.2
                  """%(histname, histname, extraoptions),
                  v_data_hists, v_bkg_hists, v_sig_hists )


def exit():
    import os
    os.system( "chmod 755 plots/*pdf plots/*png plots/*txt" )
    os.chdir( "plots" )
    os.system( "ln -s ../syncfiles/miscfiles/index.php ." )
    os.system( "chmod 755 index.php" )

    sys.exit()

if __name__ == "__main__":

    drawbyproc( "SSPred_counter"         , "--noSyst --printExpSignif --onlyLin --printYieldsTable --Minimum 0.1 --ratio_Maximum 2" )
    drawbytype( "SSPred_counter"         , "--noSyst --printExpSignif --onlyLin --printYieldsTable --Minimum 0.1 --ratio_Maximum 2" )
    drawbyproc( "MjjSBPRVRSSPred_counter", "--printObsSignif --onlyLin --printYieldsTable --Minimum 0.1 --ratio_Maximum 2" )
    drawbytype( "MjjSBPRVRSSPred_counter", "--printObsSignif --onlyLin --printYieldsTable --Minimum 0.1 --ratio_Maximum 2" )
    drawbyproc( "MjjSBVRSSPred_counter"  , "--printObsSignif --onlyLin --printYieldsTable --Minimum 0.1 --ratio_Maximum 2" )
    drawbytype( "MjjSBVRSSPred_counter"  , "--printObsSignif --onlyLin --printYieldsTable --Minimum 0.1 --ratio_Maximum 2" )
    drawbyproc( "BTagVRSSPred_counter"   , "--printObsSignif --onlyLin --printYieldsTable --Minimum 0.1 --ratio_Maximum 2" )
    drawbytype( "BTagVRSSPred_counter"   , "--printObsSignif --onlyLin --printYieldsTable --Minimum 0.1 --ratio_Maximum 2" )
    exit()








    cuts = [  "BTagVRSSPred", "MjjSBVRSSPred", "SSPred" ]
    bins = [  10            , 10             , 10       ]

    funcs = [ drawbyproc, drawbytype ]

    arguments_list = []
    for cut, nbin in zip(cuts, bins):
        for func in funcs:
            func( "%s_met"    % cut , "--xNbin %d" % nbin )

    proc_groups["data"]  = [ "data_mm", "data_em", "data_ee", "data_other" ]
    drawbyprocandtype( "SS_counter", "--printYieldsTable --Minimum 0.1 --ratio_Maximum 2" )
    drawbyproc( "SS_counter", "--printYieldsTable --Minimum 0.1 --ratio_Maximum 2" )
    drawbytype( "SS_counter", "--printYieldsTable --Minimum 0.1 --ratio_Maximum 2" )
    drawbyproc( "BTagVRSS_counter", "--printYieldsTable --Minimum 0.1 --ratio_Maximum 2" )
    drawbytype( "BTagVRSS_counter", "--printYieldsTable --Minimum 0.1 --ratio_Maximum 2" )
    drawbyproc( "MjjSBVRSS_counter", "--printYieldsTable --Minimum 0.1 --ratio_Maximum 2" )
    drawbytype( "MjjSBVRSS_counter", "--printYieldsTable --Minimum 0.1 --ratio_Maximum 2" )
    drawbyproc( "SSPred_counter", "--printYieldsTable --Minimum 0.1 --ratio_Maximum 2" )
    drawbytype( "SSPred_counter", "--printYieldsTable --Minimum 0.1 --ratio_Maximum 2" )
    drawbyproc( "BTagVRSSPred_counter", "--printYieldsTable --Minimum 0.1 --ratio_Maximum 2" )
    drawbytype( "BTagVRSSPred_counter", "--printYieldsTable --Minimum 0.1 --ratio_Maximum 2" )
    drawbyproc( "MjjSBVRSSPred_counter", "--printYieldsTable --Minimum 0.1 --ratio_Maximum 2" )
    drawbytype( "MjjSBVRSSPred_counter", "--printYieldsTable --Minimum 0.1 --ratio_Maximum 2" )
    #drawbyproc( "BTagVRSS_counter", "--printYieldsTable --Minimum 0.1 --ratio_Maximum 4" )
    #drawbyproc( "BTagARSS_counter", "--printYieldsTable --Minimum 0.1 --ratio_Maximum 4" )
    #drawbyproc( "BTagARSSPred_counter", "--printYieldsTable --Minimum 0.1 --ratio_Maximum 4" )
    #drawbyproc( "MjjSBVRSS_counter", "--printYieldsTable --Minimum 0.1 --ratio_Maximum 4" )
    #drawbyproc( "MjjSBARSS_counter", "--printYieldsTable --Minimum 0.1 --ratio_Maximum 4" )
    #drawbyproc( "MjjSBARSSPred_counter", "--printYieldsTable --Minimum 0.1 --ratio_Maximum 4" )
    #drawbyproc( "SignalRegion_rawcounter", "--printYieldsTable --Minimum 0.1 --ratio_Maximum 4" )
    #drawbyproc( "BTagVRSS_rawcounter", "--printYieldsTable --Minimum 0.1 --ratio_Maximum 4" )
    #drawbyproc( "BTagARSS_rawcounter", "--printYieldsTable --Minimum 0.1 --ratio_Maximum 4" )
    #drawbyproc( "BTagARSSPred_rawcounter", "--printYieldsTable --Minimum 0.1 --ratio_Maximum 4" )
    #drawbyproc( "MjjSBVRSS_rawcounter", "--printYieldsTable --Minimum 0.1 --ratio_Maximum 4" )
    #drawbyproc( "MjjSBARSS_rawcounter", "--printYieldsTable --Minimum 0.1 --ratio_Maximum 4" )
    #drawbyproc( "MjjSBARSSPred_rawcounter", "--printYieldsTable --Minimum 0.1 --ratio_Maximum 4" )
    exit()

#    cuts = [ "SSMM_CutSSMMLep", "SSMM_CutNTwoJet", "SSMM_CutThirdLepVeto", "SSMM_CutIsoTrackVeto", "SSMM_CutBVeto", "SSMM_CutWMjj", "SSMM_CutLowMjj", "SSMM_CutLowDEtajj" ]
#    bins = [ 20               , 20               , 20                    , 20                    , 10             , 10            , 10              , 10                  ]

#    cuts = [  "SSMM_CutSSMMLep", "SSEM_CutSSEMLep", "SSEE_CutSSEELep", "SSMM_CutNjet" ]
#    bins = [  20               , 20               , 20               , 20             ]

    cuts = [  "BTagVRSSPred", "MjjSBVRSSPred", "SSPred" ]
    bins = [  10            , 10             , 10       ]

    funcs = [ drawbyproc, drawbytype ]

    arguments_list = []
    for cut, nbin in zip(cuts, bins):
        for func in funcs:
            func( "%s_met"    % cut , "--xNbin %d" % nbin )
            func( "%s_nvtx"    % cut , "--xNbin %d" % 70 )
            #func( "%s_m4"     % cut , "--xNbin %d" % nbin )
            #func( "%s_m4wide" % cut , "--xNbin %d" % nbin )
            func( "%s_Mll"    % cut , "--xNbin %d" % nbin )
            func( "%s_MjjW"   % cut , "--xNbin %d" % nbin )
            func( "%s_MjjLead"% cut , "--xNbin %d" % nbin )
            func( "%s_DEtajjLead"% cut , "--xNbin %d" % nbin )
            func( "%s_Mll250" % cut , "--xNbin %d" % nbin )
            func( "%s_Mll500" % cut , "--xNbin %d" % nbin )
            func( "%s_jetb_size" % cut , "" )
            func( "%s_jet_size" % cut , "" )
            func( "%s_jet3l_size" % cut , "" )
            func( "%s_jet0_csv" % cut , "--xNbin %d" % nbin )
            func( "%s_jet1_csv" % cut , "--xNbin %d" % nbin )
            func( "%s_lep0_pid" % cut , "--xNbin %d" % nbin )
            func( "%s_lep1_pid" % cut , "--xNbin %d" % nbin )
            func( "%s_lep0_pt" % cut , "--xNbin %d" % nbin )
            func( "%s_lep1_pt" % cut , "--xNbin %d" % nbin )
            func( "%s_lep0_eta" % cut , "--xNbin %d" % nbin )
            func( "%s_lep1_eta" % cut , "--xNbin %d" % nbin )
            func( "%s_lep0_phi" % cut , "--xNbin %d" % nbin )
            func( "%s_lep1_phi" % cut , "--xNbin %d" % nbin )
            func( "%s_lep0_iso" % cut , "--xNbin %d" % nbin )
            func( "%s_lep1_iso" % cut , "--xNbin %d" % nbin )
            func( "%s_lep0_ip3" % cut , "--xNbin %d" % 36 )
            func( "%s_lep1_ip3" % cut , "--xNbin %d" % 36 )
            func( "%s_DPhill" % cut , "--xNbin %d" % nbin )
            func( "%s_DEtall" % cut , "--xNbin %d" % nbin )
            func( "%s_met"    % cut , "--xNbin %d" % nbin )

    exit()

#
