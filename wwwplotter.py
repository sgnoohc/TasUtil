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

        self.tfile = r.TFile( "output.root" )

        self.hists = {}

        for key in self.tfile.GetListOfKeys():
            histname = key.GetName()
            self.hists[histname] = self.tfile.Get( histname )

    def draw( self, histname, extraoptions="" ):

        hists = {}
        for key in self.proc_groups:
            histsum = None
            for proc in self.proc_groups[key]:
                hist = None
                try :
                    hist = self.hists[proc + "_" + histname]
                except:
                    pass
                if hist:
                    if histsum:
                        histsum.Add( hist )
                    else:
                        histsum = hist.Clone( key )
                        histsum.SetDirectory( 0 )
            hists[key] = histsum

#        for key in hists:
#            if hists[key]:
#                print hists[key].GetName()
#            else:
#                print key, "empty"

        v_bkg_hists = r.vector("TH1*")()
        v_sig_hists = r.vector("TH1*")()
        v_data_hists = r.vector("TH1*")()
        for key in self.proc_categs:
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

        return r.plotmaker( """
                      --yTitle N leptons
                      --xTitle %s
                      --plotOutputName plots/%s
                      --ratio_Maximum 2
                      --ratio_Minimum 0.
                      --showOverflow
                      --autoStack
                      --legend_NColumns 2
                      --MaximumMultiplier 2
                      %s
                      """%(histname, histname, extraoptions),
                      v_data_hists, v_bkg_hists, v_sig_hists )

if __name__ == "__main__":

    wwwplotter = WWWPlotter()

    wwwplotter.proc_groups["data"]  = [ "data_mm", "data_em" ]
    wwwplotter.draw( "Region_counter", "" )
    wwwplotter.draw( "SSMM_CutThirdLepVetonisotrack" )
#    wwwplotter.draw( "SSMM_CutSSMMLepmet", "--xNbin 20" )
#    wwwplotter.draw( "SSMM_CutSSMMLeplep0_pt", "--xNbin 20" )
#    wwwplotter.draw( "SSMM_CutSSMMLeplep1_pt", "--xNbin 20" )
#    wwwplotter.draw( "SSMM_CutSSMMLepjet_size", "" )
#    wwwplotter.draw( "SSMM_CutSSMMLepjetb_size", "" )
#    wwwplotter.draw( "SSMM_CutSSMMLepjet3l_size", "" )

#    wwwplotter.proc_groups["data"]  = [ "data_ee" ]
#    wwwplotter.draw( "ee_met", "--xNbin 20" );
#    wwwplotter.draw( "ee_nvtx", "" );
#    wwwplotter.draw( "ee_dimuon_mass", "--xNbin 20" );
#    wwwplotter.draw( "eee_met", "--xNbin 20" );
#    wwwplotter.draw( "eee_nvtx", "" );
#    wwwplotter.draw( "eee_dimuon_mass", "--xNbin 20" );
#
#    wwwplotter.proc_groups["data"]  = [ "data_mm" ]
#    wwwplotter.draw( "mm_met", "--xNbin 10" );
#    wwwplotter.draw( "mm_nvtx", "" );
#    wwwplotter.draw( "mm_dimuon_mass", "--xNbin 20" );
#    wwwplotter.draw( "mmjetb_size", "" );
#    wwwplotter.draw( "mmjet_size", "" );
#    wwwplotter.draw( "mmjet3l_size", "" );
#    wwwplotter.draw( "mmm_met", "--xNbin 20" );
#    wwwplotter.draw( "mmm_nvtx", "" );
#    wwwplotter.draw( "mmm_dimuon_mass", "--xNbin 20" );
#    wwwplotter.draw( "mmm_loose_met", "--xNbin 20" );
#    wwwplotter.draw( "mmm_loose_nvtx", "" );
#    wwwplotter.draw( "mmm_loose_dimuon_mass", "--xNbin 20" );
#    wwwplotter.draw( "mmm_lbnt_met", "--xNbin 20" );
#    wwwplotter.draw( "mmm_lbnt_nvtx", "" );
#    wwwplotter.draw( "mmm_lbnt_dimuon_mass", "--xNbin 20" );





#    wwwplotter.draw( "Region_counter", "--noData --printYieldsTable" )
#    wwwplotter.draw( "Region_rawcounter", "--noData --printYieldsTable" )

#    for i in xrange( 2, 13 ):
#        if i != 4:
#            continue
#        wwwplotter.draw( "SSmm_Cut%02dmet" % i );
#        wwwplotter.draw( "SSmm_Cut%02djet_size" % i );
#        wwwplotter.draw( "SSmm_Cut%02djetb_size" % i );
#        wwwplotter.draw( "SSmm_Cut%02dlep_size" % i );
#        wwwplotter.draw( "SSmm_Cut%02dMllSS" % i );
#        wwwplotter.draw( "SSmm_Cut%02dlep0_pt" % i );
#        wwwplotter.draw( "SSmm_Cut%02dlep0_eta" % i );
#        wwwplotter.draw( "SSmm_Cut%02dlep0_phi" % i );

#    wwwplotter.draw( "SSARmm_DEtajjLead"    , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_DPhi3lMET"     , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_MjjLead"       , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_MjjW"          , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_MllSS"         , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_Pt3l"          , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_get0SFOSMee"   , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_get0SFOSMll"   , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_get1SFOSMll"   , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_get2SFOSMll0"  , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_get2SFOSMll1"  , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_getMTmax"      , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_jet0_E"        , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_jet0_eta"      , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_jet0_phi"      , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_jet0_pt"       , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_jet1_E"        , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_jet1_eta"      , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_jet1_phi"      , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_jet1_pt"       , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_jet2_E"        , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_jet2_eta"      , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_jet2_phi"      , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_jet2_pt"       , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_jet3_E"        , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_jet3_eta"      , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_jet3_phi"      , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_jet3_pt"       , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_jet4_E"        , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_jet4_eta"      , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_jet4_phi"      , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_jet4_pt"       , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_jet5_E"        , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_jet5_eta"      , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_jet5_phi"      , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_jet5_pt"       , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_jet6_E"        , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_jet6_eta"      , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_jet6_phi"      , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_jet6_pt"       , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_jetwtag0_E"    , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_jetwtag0_eta"  , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_jetwtag0_phi"  , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_jetwtag0_pt"   , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_jetwtag1_E"    , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_jetwtag1_eta"  , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_jetwtag1_phi"  , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_jetwtag1_pt"   , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_lep0_E"        , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_lep0_eta"      , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_lep0_phi"      , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_lep0_pt"       , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_lep1_E"        , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_lep1_eta"      , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_lep1_phi"      , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_lep1_pt"       , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_leplbnt0_E"    , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_leplbnt0_eta"  , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_leplbnt0_phi"  , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_leplbnt0_pt"   , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_leploose0_E"   , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_leploose0_eta" , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_leploose0_phi" , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_leploose0_pt"  , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_leploose1_E"   , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_leploose1_eta" , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_leploose1_phi" , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_leploose1_pt"  , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_leploose2_E"   , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_leploose2_eta" , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_leploose2_phi" , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_leploose2_pt"  , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_leptight0_E"   , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_leptight0_eta" , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_leptight0_phi" , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_leptight0_pt"  , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_leptight1_E"   , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_leptight1_eta" , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_leptight1_phi" , "--xNbin 20" )
#    wwwplotter.draw( "SSARmm_leptight1_pt"  , "--xNbin 20" )
#
#    wwwplotter.draw( "SSmm_DEtajjLead"    , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_DPhi3lMET"     , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_MjjLead"       , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_MjjW"          , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_MllSS"         , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_Pt3l"          , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_met"           , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_get0SFOSMee"   , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_get0SFOSMll"   , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_get1SFOSMll"   , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_get2SFOSMll0"  , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_get2SFOSMll1"  , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_getMTmax"      , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_jet0_E"        , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_jet0_eta"      , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_jet0_phi"      , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_jet0_pt"       , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_jet1_E"        , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_jet1_eta"      , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_jet1_phi"      , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_jet1_pt"       , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_jet2_E"        , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_jet2_eta"      , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_jet2_phi"      , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_jet2_pt"       , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_jet3_E"        , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_jet3_eta"      , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_jet3_phi"      , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_jet3_pt"       , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_jet4_E"        , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_jet4_eta"      , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_jet4_phi"      , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_jet4_pt"       , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_jet5_E"        , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_jet5_eta"      , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_jet5_phi"      , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_jet5_pt"       , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_jet6_E"        , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_jet6_eta"      , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_jet6_phi"      , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_jet6_pt"       , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_jetwtag0_E"    , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_jetwtag0_eta"  , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_jetwtag0_phi"  , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_jetwtag0_pt"   , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_jetwtag1_E"    , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_jetwtag1_eta"  , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_jetwtag1_phi"  , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_jetwtag1_pt"   , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_lep0_E"        , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_lep0_eta"      , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_lep0_phi"      , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_lep0_pt"       , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_lep1_E"        , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_lep1_eta"      , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_lep1_phi"      , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_lep1_pt"       , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_leplbnt0_E"    , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_leplbnt0_eta"  , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_leplbnt0_phi"  , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_leplbnt0_pt"   , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_leploose0_E"   , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_leploose0_eta" , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_leploose0_phi" , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_leploose0_pt"  , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_leploose1_E"   , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_leploose1_eta" , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_leploose1_phi" , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_leploose1_pt"  , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_leploose2_E"   , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_leploose2_eta" , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_leploose2_phi" , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_leploose2_pt"  , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_leptight0_E"   , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_leptight0_eta" , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_leptight0_phi" , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_leptight0_pt"  , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_leptight1_E"   , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_leptight1_eta" , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_leptight1_phi" , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_leptight1_pt"  , "--xNbin 20" )

#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_DEtajjLead"    , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_DPhi3lMET"     , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_MjjLead"       , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_MjjW"          , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_MllSS"         , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_Pt3l"          , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_met"           , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_get0SFOSMee"   , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_get0SFOSMll"   , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_get1SFOSMll"   , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_get2SFOSMll0"  , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_get2SFOSMll1"  , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_getMTmax"      , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_jet0_E"        , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_jet0_eta"      , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_jet0_phi"      , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_jet0_pt"       , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_jet1_E"        , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_jet1_eta"      , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_jet1_phi"      , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_jet1_pt"       , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_jet2_E"        , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_jet2_eta"      , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_jet2_phi"      , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_jet2_pt"       , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_jet3_E"        , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_jet3_eta"      , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_jet3_phi"      , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_jet3_pt"       , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_jet4_E"        , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_jet4_eta"      , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_jet4_phi"      , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_jet4_pt"       , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_jet5_E"        , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_jet5_eta"      , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_jet5_phi"      , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_jet5_pt"       , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_jet6_E"        , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_jet6_eta"      , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_jet6_phi"      , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_jet6_pt"       , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_jetwtag0_E"    , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_jetwtag0_eta"  , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_jetwtag0_phi"  , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_jetwtag0_pt"   , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_jetwtag1_E"    , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_jetwtag1_eta"  , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_jetwtag1_phi"  , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_jetwtag1_pt"   , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_lep0_E"        , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_lep0_eta"      , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_lep0_phi"      , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_lep0_pt"       , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_lep1_E"        , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_lep1_eta"      , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_lep1_phi"      , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_lep1_pt"       , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_leplbnt0_E"    , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_leplbnt0_eta"  , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_leplbnt0_phi"  , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_leplbnt0_pt"   , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_leploose0_E"   , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_leploose0_eta" , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_leploose0_phi" , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_leploose0_pt"  , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_leploose1_E"   , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_leploose1_eta" , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_leploose1_phi" , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_leploose1_pt"  , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_leploose2_E"   , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_leploose2_eta" , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_leploose2_phi" , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_leploose2_pt"  , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_leptight0_E"   , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_leptight0_eta" , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_leptight0_phi" , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_leptight0_pt"  , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_leptight1_E"   , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_leptight1_eta" , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_leptight1_phi" , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_leptight1_pt"  , "--xNbin 20" )
#    wwwplotter.draw( "SSmm_LowMET_MjjSideBand_m4wide"  , "--xNbin 15" )

#
