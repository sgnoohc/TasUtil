#!/bin/env python

import sys
sys.path.append("/home/users/phchang/public_html/tasutil")
import plotmaker
import ROOT as r
from array import *
import glob

jobtag = "TEST-v1"

class WWWPlotter:

    def __init__( self ):

        self.proc_groups = {}
        self.proc_groups["W"]     = [ "wj" ]
        #self.proc_groups["W"]     = [ ]
        self.proc_groups["Z"]     = [ "dy" ]
        self.proc_groups["tt1l"]  = [ "tt1l" ]
        self.proc_groups["tt2l"]  = [ "tt2l" ]
        self.proc_groups["WW"]    = [ "ww", "wwdpi", "vbsww" ]
        self.proc_groups["WZ"]    = [ "wz" ]
        self.proc_groups["ZZ"]    = [ "zz" ]
        self.proc_groups["tX"]    = [ "singletop" ]
        self.proc_groups["ttX"]   = [ "ttz", "ttw", "tth", "ttg" ]
        self.proc_groups["VVV"]   = [ "wwz_incl", "wzz_incl", "zzz_incl"]#, "wwg_incl", "wzg_incl" ]
        self.proc_groups["vh"]    = [ "vh" ]
        self.proc_groups["whwww"] = [ "whwww" ]
        self.proc_groups["www"]   = [ "www" ]
        self.proc_groups["data"]  = [ "data_mm", "data_em", "data_ee" ]

        self.proc_colors = {}
        self.proc_colors["W"]     = 2006
        self.proc_colors["Z"]     = 2010
        self.proc_colors["tt1l"]  = 2005
        self.proc_colors["tt2l"]  = 2008
        self.proc_colors["WW"]    = 2007
        self.proc_colors["WZ"]    = 2003
        self.proc_colors["ZZ"]    = 2011
        self.proc_colors["tX"]    = 2009
        self.proc_colors["ttX"]   = 2004
        self.proc_colors["VVV"]   = 616
        self.proc_colors["vh"]    = 2002
        self.proc_colors["whwww"] = 4
        self.proc_colors["www"]   = 2
        self.proc_colors["data"]  = 1

        self.proc_categs = {}
        self.proc_categs["W"]     = "bkg"
        self.proc_categs["Z"]     = "bkg"
        self.proc_categs["tt1l"]  = "bkg"
        self.proc_categs["tt2l"]  = "bkg"
        self.proc_categs["WW"]    = "bkg"
        self.proc_categs["WZ"]    = "bkg"
        self.proc_categs["ZZ"]    = "bkg"
        self.proc_categs["tX"]    = "bkg"
        self.proc_categs["ttX"]   = "bkg"
        self.proc_categs["VVV"]   = "bkg"
        self.proc_categs["vh"]    = "bkg"
        self.proc_categs["whwww"] = "sig"
        self.proc_categs["www"]   = "sig"
        self.proc_categs["data"]  = "data"

        self.bkg_groups = {}
        self.bkg_groups["trueSS"]      = [ "trueSS" ]
        self.bkg_groups["chargeflips"] = [ "chargeflips" ]
        self.bkg_groups["SSLL"]        = [ "SSLL" ]
        self.bkg_groups["photon"]      = [ "photonfakes", "photondoublefakes", "photontriplefakes", "fakesphotonfakes", "otherphotonfakes" ]
        self.bkg_groups["fakes"]       = [ "fakes", "doublefakes" ]
        self.bkg_groups["others"]      = [ "others" ]
        self.bkg_groups["data"]        = [ "data_mm", "data_em", "data_ee" ]
        self.bkg_groups["www"]         = [ "www" ]
        self.bkg_groups["whwww"]       = [ "whwww" ]

        self.bkg_colors = {}
        self.bkg_colors["trueSS"]      = 2001
        self.bkg_colors["chargeflips"] = 2007
        self.bkg_colors["SSLL"]        = 2003
        self.bkg_colors["photon"]      = 2011
        self.bkg_colors["fakes"]       = 2005
        self.bkg_colors["others"]      = 616
        self.bkg_colors["data"]        = 1
        self.bkg_colors["www"]         = 2
        self.bkg_colors["whwww"]       = 4

        self.bkg_categs = {}
        self.bkg_categs["trueSS"]      = "bkg"
        self.bkg_categs["chargeflips"] = "bkg"
        self.bkg_categs["SSLL"]        = "bkg"
        self.bkg_categs["photon"]      = "bkg"
        self.bkg_categs["fakes"]       = "bkg"
        self.bkg_categs["others"]      = "bkg"
        self.bkg_categs["data"]        = "data"
        self.bkg_categs["www"]         = "sig"
        self.bkg_categs["whwww"]       = "sig"

        self.tfile = r.TFile( "output.root" )

        self.hists = {}

        #for key in self.tfile.GetListOfKeys():
        #    histname = key.GetName()
        #    if histname.find("counter") != -1:
        #        print histname
        #    #self.hists[histname] = self.tfile.Get( histname )

    def drawbyproc( self, histname, extraoptions="" ):

        # Declare a map to hold the histograms that I access for this round of plotting
        hists = {}

        # Looping over MC sample process (e.g. W, ttbar, WZ, etc.)
        for key in self.proc_groups:

#            if key != "W": continue

            # the total background for this category is saved (e.g. W, ttbar, WZ, etc.)
            histsum = None

            # Loop over the individual that makes up the grouping.
            for proc in self.proc_groups[key]:

                # place holder
                hist = None

                # Form the name of the histogram
                histkey = proc + "__" + histname

                # Try to access it
                try :
                    hist = self.hists[histkey]
                # If not get it from the tfile
                except:
                    # Open it and save it to the self.hists which persists as long as this object persists.
                    self.hists[histkey] = self.tfile.Get( histkey )

                    # Get the pointer
                    hist = self.hists[histkey]

                # If successfully retrieved
                if hist:

                    # If already a clone was created copy the content over
                    if histsum:
                        histsum.Add( hist )
                    # If a clone has not been created yet, create one
                    else:
                        histsum = hist.Clone( key )
                        histsum.SetDirectory( 0 )

            # Once done looping over the grouping, set the histogram
            hists[key] = histsum

        # Now, we put them in std::vector<TH1*> so that we can pass it on to plotmaker.cc
        v_bkg_hists = r.vector("TH1*")()
        v_sig_hists = r.vector("TH1*")()
        v_data_hists = r.vector("TH1*")()

        # For the histograms we have stylize a bit
        for key in self.proc_categs:
#            if key != "W": continue
            if self.proc_categs[key] == "bkg"  and hists[key]:
                hists[key].SetLineColor( self.proc_colors[key] )
                hists[key].SetFillColor( self.proc_colors[key] )
                v_bkg_hists .push_back( hists[key] )
            if self.proc_categs[key] == "sig"  and hists[key]:
                hists[key].SetLineColor( self.proc_colors[key] )
                hists[key].SetLineWidth( 2 )
                v_sig_hists .push_back( hists[key] )
            if self.proc_categs[key] == "data" and hists[key]:
                hists[key].SetLineColor( 1 )
                v_data_hists.push_back( hists[key] )

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

    def drawbytype( self, histname, extraoptions="" ):

        # Declare a map to hold the histograms that I access for this round of plotting
        hists = {}

        # Looping over based on background categorization
        for bkgtype in self.bkg_groups:

            # the total background for this category is saved (e.g. trueSS, LL, fakes, etc.)
            histsum = None

            # Loop over the individual bkg type
            for bkg in self.bkg_groups[bkgtype]:

                # place holder
                hist = None

                # Form the name of the histogram
                histkey = "_" + bkg + "_" + histname

                # Try to access it
                try :
                    hist = self.hists[histkey]
                # If not get it from the tfile
                except:
                    # Open it and save it to the self.hists which persists as long as this object persists.
                    self.hists[histkey] = self.tfile.Get( histkey )

                    # Get the pointer
                    hist = self.hists[histkey]

                # If successfully retrieved
                if hist:

                    # If already a clone was created copy the content over
                    if histsum:
                        histsum.Add( hist )
                    # If a clone has not been created yet, create one
                    else:
                        histsum = hist.Clone( bkgtype )
                        histsum.SetDirectory( 0 )

            # Once done looping over the grouping, set the histogram
            hists[bkgtype] = histsum

        # Now, we put them in std::vector<TH1*> so that we can pass it on to plotmaker.cc
        v_bkg_hists = r.vector("TH1*")()
        v_sig_hists = r.vector("TH1*")()
        v_data_hists = r.vector("TH1*")()

        # For the histograms we have stylize a bit
        for key in self.bkg_groups:
            if self.bkg_categs[key] == "bkg"  and hists[key]:
                hists[key].SetLineColor( self.bkg_colors[key] )
                hists[key].SetFillColor( self.bkg_colors[key] )
                v_bkg_hists .push_back( hists[key] )
            if self.bkg_categs[key] == "sig"  and hists[key]:
                hists[key].SetLineColor( self.bkg_colors[key] )
                hists[key].SetLineWidth( 2 )
                v_sig_hists .push_back( hists[key] )
            if self.bkg_categs[key] == "data" and hists[key]:
                hists[key].SetLineColor( 1 )
                v_data_hists.push_back( hists[key] )

        # Then plot
        return r.plotmaker( """
                      --yTitle N leptons
                      --xTitle %s
                      --plotOutputName plots/%s_bytype
                      --ratio_Maximum 2
                      --ratio_Minimum 0.
                      --autoStack
                      --showOverflow
                      --legend_NColumns 1
                      %s
                      --MaximumMultiplier 1.2
                      """%(histname, histname, extraoptions),
                      v_data_hists, v_bkg_hists, v_sig_hists )

if __name__ == "__main__":

    wwwplotter = WWWPlotter()

    wwwplotter.proc_groups["data"]  = [ "data_mm", "data_em", "data_ee" ]
    wwwplotter.drawbyproc( "SignalRegion_counter", "--printYieldsTable --Minimum 0.1 --ratio_Maximum 4" )
    wwwplotter.drawbyproc( "BTagVRSS_counter", "--printYieldsTable --Minimum 0.1 --ratio_Maximum 4" )
    wwwplotter.drawbyproc( "BTagARSS_counter", "--printYieldsTable --Minimum 0.1 --ratio_Maximum 4" )
    wwwplotter.drawbyproc( "SignalRegion_rawcounter", "--printYieldsTable --Minimum 0.1 --ratio_Maximum 4" )
    wwwplotter.drawbyproc( "BTagVRSS_rawcounter", "--printYieldsTable --Minimum 0.1 --ratio_Maximum 4" )
    wwwplotter.drawbyproc( "BTagARSS_rawcounter", "--printYieldsTable --Minimum 0.1 --ratio_Maximum 4" )

    sys.exit()

#    cuts = [ "SSMM_CutSSMMLep", "SSMM_CutNTwoJet", "SSMM_CutThirdLepVeto", "SSMM_CutIsoTrackVeto", "SSMM_CutBVeto", "SSMM_CutWMjj", "SSMM_CutLowMjj", "SSMM_CutLowDEtajj" ]
#    bins = [ 20               , 20               , 20                    , 20                    , 10             , 10            , 10              , 10                  ]

#    cuts = [  "SSMM_CutSSMMLep", "SSEM_CutSSEMLep", "SSEE_CutSSEELep", "SSMM_CutNjet" ]
#    bins = [  20               , 20               , 20               , 20             ]

    cuts = [  "SSMM" ]
    bins = [  10     ]

    funcs = [ wwwplotter.drawbyproc, wwwplotter.drawbytype ]

    for cut, nbin in zip(cuts, bins):
        for func in funcs:
            func( "%s_met"    % cut , "--xNbin %d" % nbin )
            func( "%s_nvtx"    % cut , "--xNbin %d" % 70 )
            func( "%s_Mll"    % cut , "--xNbin %d" % nbin )
            func( "%s_Mll250" % cut , "--xNbin %d" % nbin )
            func( "%s_Mll500" % cut , "--xNbin %d" % nbin )
            func( "%s_jetb_size" % cut , "" )
            func( "%s_jet_size" % cut , "" )
            func( "%s_jet3l_size" % cut , "" )
            func( "%s_jet0_csv" % cut , "--xNbin %d" % nbin )
            func( "%s_jet1_csv" % cut , "--xNbin %d" % nbin )
            func( "%s_lep0_pid" % cut , "--xNbin %d" % nbin )
            func( "%s_lep1_pid" % cut , "--xNbin %d" % nbin )
            func( "%s_leplbnt0_pid" % cut , "--xNbin %d" % nbin )
            func( "%s_lep0_pt" % cut , "--xNbin %d" % nbin )
            func( "%s_lep1_pt" % cut , "--xNbin %d" % nbin )
            func( "%s_leptight0_pt" % cut , "--xNbin %d" % nbin )
            func( "%s_leplbnt0_pt" % cut , "--xNbin %d" % nbin )
            func( "%s_lep0_eta" % cut , "--xNbin %d" % nbin )
            func( "%s_lep1_eta" % cut , "--xNbin %d" % nbin )
            func( "%s_leptight0_eta" % cut , "--xNbin %d" % nbin )
            func( "%s_leplbnt0_eta" % cut , "--xNbin %d" % nbin )
            func( "%s_lep0_phi" % cut , "--xNbin %d" % nbin )
            func( "%s_lep1_phi" % cut , "--xNbin %d" % nbin )
            func( "%s_leptight0_phi" % cut , "--xNbin %d" % nbin )
            func( "%s_leplbnt0_phi" % cut , "--xNbin %d" % nbin )
            func( "%s_lep0_iso" % cut , "--xNbin %d" % nbin )
            func( "%s_lep1_iso" % cut , "--xNbin %d" % nbin )
            func( "%s_leptight0_iso" % cut , "--xNbin %d" % nbin )
            func( "%s_leplbnt0_iso" % cut , "--xNbin %d" % nbin )
            func( "%s_lep0_ip3" % cut , "--xNbin %d" % 36 )
            func( "%s_lep1_ip3" % cut , "--xNbin %d" % 36 )
            func( "%s_leptight0_ip3" % cut , "--xNbin %d" % nbin )
            func( "%s_leplbnt0_ip3" % cut , "--xNbin %d" % nbin )
            func( "%s_leplbnt0_ip3_wide" % cut , "--xNbin %d" % 60 )
            func( "%s_leplbnt0_ip3_widepp" % cut , "--xNbin %d" % 60 )
            func( "%s_leplbnt0_ip3err" % cut , "--xNbin %d" % 60 )
            func( "%s_leplbnt0_dxy" % cut , "--xNbin %d" % 60 )
            func( "%s_leplbnt0_dz" % cut , "--xNbin %d" % 60 )
            func( "%s_leptight0_ip3_wide" % cut , "--xNbin %d" % 60 )
            func( "%s_leptight0_ip3err" % cut , "--xNbin %d" % 60 )
            func( "%s_leptight0_dxy" % cut , "--xNbin %d" % 60 )
            func( "%s_leptight0_dz" % cut , "--xNbin %d" % 60 )
            func( "%s_DPhill" % cut , "--xNbin %d" % nbin )
            func( "%s_DEtall" % cut , "--xNbin %d" % nbin )
            func( "%s_met"    % cut , "--xNbin %d" % nbin )

    import os
    os.system( "chmod 755 plots/*pdf plots/*png" )

#
