//  .
// ..: P. Chang, philip@physics.ucsd.edu

#include "WWW_ScanChain.h"

//_________________________________________________________________________________________________
void ScanChain( TChain* chain, TString output_name, TString base_optstr, int nevents )
{
    // -~-~-~-~-~-~
    // Event Looper
    // -~-~-~-~-~-~
    Looper<WWWTree> looper( chain, &wwwbaby, nevents );
    chain->GetEntry( 0 );
    wwwbaby.Init( chain->GetTree() );

    // -~-~-~-~-~-~-~-~-~-~-
    // Parse option settings
    // -~-~-~-~-~-~-~-~-~-~-
    TString option = base_optstr;
    TasUtil::print( "the base option string = " + base_optstr );
    int babyver = getBabyVersion( base_optstr );
    std::cout << "baby version = " << babyver << std::endl;

    TasUtil::EventList event_list( "list.txt" );

    // -~-~-~-~-~
    // Set output
    // -~-~-~-~-~
    TasUtil::AutoHist hists;
    while ( looper.nextEvent() )
    {
        if ( wwwbaby.isData() )
        {
            duplicate_removal::DorkyEventIdentifier id( wwwbaby.run(), wwwbaby.evt(), wwwbaby.lumi() );
            if ( is_duplicate( id ) )
                continue;
        }
        setObjectIndices();
        doAnalysis( hists, event_list );
        //doTmpAnalysis( hists );
    }
    hists.save( output_name );
}

//_________________________________________________________________________________________________
void doTmpAnalysis( TasUtil::AutoHist& hists )
{
    lepidx["SignalLepton"] = lepidx["LooseLepton"];
    if (!( lepidx["TightLepton"].size()                     ==   1   )) return;
    if (!( lepidx["LooseLepton"].size()                     ==   2   )) return;
    if (!( wwwbaby.lep_p4()[lepidx["SignalLepton"][0]].pt()  >   30. )) return;
    if (!( wwwbaby.lep_p4()[lepidx["SignalLepton"][1]].pt()  >   30. )) return;
    if (!( isSS()                                                    )) return;
    if ( isSSMM() && Mll() > 40. )
    {
        fillHistograms( hists, "SSMM_CutSSMMLep", 0 );
        if ( jetidx["GoodSSJet"].size() == 0 )
            fillHistograms( hists, "SSMM_CutNjet", 0 );
    }
    if ( isSSEM() && Mll() > 40. && abs(wwwbaby.lep_pdgId()[lepidx["LbntLepton"][0]]) == 13 )
        fillHistograms( hists, "SSEM_CutSSEMLep", 1 );
    if ( isSSEE() && Mll() > 40. )
        fillHistograms( hists, "SSEE_CutSSEELep", 2 );
}

//_________________________________________________________________________________________________
void doAnalysis( TasUtil::AutoHist& hists, TasUtil::EventList& event_list )
{
    int cutidx = 0;
    if ( passSSMM() ) fillHistograms( hists, "SSMM", 0 );
    if ( passSSEM() ) fillHistograms( hists, "SSEM", 1 );
    if ( passSSEE() ) fillHistograms( hists, "SSEE", 2 );
//    if ( pass3L0SFOS() ) fillHistograms( hists, "3L0SFOS", 3 );
//    if ( pass3L1SFOS() ) fillHistograms( hists, "3L1SFOS", 4 );
//    if ( pass3L2SFOS() ) fillHistograms( hists, "3L2SFOS", 5 );
    if ( passBTagVRSSMM() ) fillHistograms( hists, "BTagVRSSMM",  6 );
    if ( passBTagVRSSEM() ) fillHistograms( hists, "BTagVRSSEM",  7 );
    if ( passBTagVRSSEE() ) fillHistograms( hists, "BTagVRSSEE",  8 );
    if ( passBTagARSSMM() ) fillHistograms( hists, "BTagARSSMM",  9 );
    if ( passBTagARSSEM() ) fillHistograms( hists, "BTagARSSEM", 10 );
    if ( passBTagARSSEE() ) fillHistograms( hists, "BTagARSSEE", 11 );
}

//=================================================================================================
//=================================================================================================
//=================================================================================================

//_________________________________________________________________________________________________
void fillHistograms( TasUtil::AutoHist& hists, TString prefix, int regionid )
{
    int sample_priority = -1;
    TString sample_category = sampleCategory( sample_priority );
    TString bkg_category = bkgCategory();
    TString empty = "";
    fillHistogramsFull( hists, sample_category, empty, prefix, regionid );
    if ( sample_priority == 1 )
        fillHistogramsFull( hists, empty, bkg_category, prefix, regionid );
}

//_________________________________________________________________________________________________
void fillHistogramsFull( TasUtil::AutoHist& hists, TString sample_category, TString bkg_category, TString prefix, int regionid )
{
    TString procprefix = sample_category + "_" + bkg_category;
    hists.fill( regionid     , Form( "%s_SignalRegion_counter"    , procprefix.Data() ) , weight() , 6  , 0 , 6 );
    hists.fill( regionid     , Form( "%s_SignalRegion_rawcounter" , procprefix.Data() ) , 1        , 6  , 0 , 6 );
    hists.fill( regionid - 6 , Form( "%s_BTagVRSS_counter"        , procprefix.Data() ) , weight() , 3  , 0 , 3 );
    hists.fill( regionid - 6 , Form( "%s_BTagVRSS_rawcounter"     , procprefix.Data() ) , 1        , 3  , 0 , 3 );
    hists.fill( regionid - 9 , Form( "%s_BTagARSS_counter"        , procprefix.Data() ) , weight() , 3  , 0 , 3 );
    hists.fill( regionid - 9 , Form( "%s_BTagARSS_rawcounter"     , procprefix.Data() ) , 1        , 3  , 0 , 3 );

    TString fullprefix = sample_category + "_" + bkg_category + "_" + prefix + "_";
    fillLepHistograms( hists, "SignalLepton", ""      , fullprefix );
    fillLepHistograms( hists, "TightLepton" , "tight" , fullprefix );
    fillLepHistograms( hists, "LooseLepton" , "loose" , fullprefix );
    fillLepHistograms( hists, "LbntLepton"  , "lbnt"  , fullprefix );
    fillJetHistograms( hists, "GoodSSJet"   , ""      , fullprefix );
    fillJetHistograms( hists, "LooseBJet"   , "b"     , fullprefix );
    fillJetHistograms( hists, "Good3LJet"   , "3l"    , fullprefix );
    fillJetHistograms( hists, "GoodSSWJet"  , "wtag"  , fullprefix );
    fillWWWHistograms( hists, fullprefix );
}

//_________________________________________________________________________________________________
void fillLepHistograms( TasUtil::AutoHist& hists, TString categ, TString name, TString prefix )
{
    hists.fill( lepidx[categ].size() , Form( "%slep%s_size" , prefix.Data(), name.Data() ), weight(), 5, 0, 5 );
    for ( unsigned int i = 0; i < lepidx[categ].size() && i < MAXOBJ; ++i )
    {
        int ilep = lepidx[categ][i];
        hists.fill( wwwbaby.lep_pdgId()[ilep]      , Form( "%slep%s%d_pid"       , prefix.Data(), name.Data(), i ), weight(),   40,  -20     ,  20      );
        hists.fill( wwwbaby.lep_p4()[ilep].pt()    , Form( "%slep%s%d_pt"        , prefix.Data(), name.Data(), i ), weight(), 1080,    0     , 250.     );
        hists.fill( wwwbaby.lep_p4()[ilep].eta()   , Form( "%slep%s%d_eta"       , prefix.Data(), name.Data(), i ), weight(), 1080,   -3     ,   3      );
        hists.fill( wwwbaby.lep_p4()[ilep].phi()   , Form( "%slep%s%d_phi"       , prefix.Data(), name.Data(), i ), weight(), 1080,   -3.1416,   3.1416 );
        hists.fill( wwwbaby.lep_p4()[ilep].energy(), Form( "%slep%s%d_E"         , prefix.Data(), name.Data(), i ), weight(), 1080,    0     , 250.     );
        hists.fill( wwwbaby.lep_relIso03EA()[ilep] , Form( "%slep%s%d_iso"       , prefix.Data(), name.Data(), i ), weight(), 1080,    0     ,   0.1    );
        hists.fill( wwwbaby.lep_ip3d()[ilep]       , Form( "%slep%s%d_ip3"       , prefix.Data(), name.Data(), i ), weight(), 1080,   -0.05  ,   0.05   );
        hists.fill( wwwbaby.lep_ip3derr()[ilep]    , Form( "%slep%s%d_ip3err"    , prefix.Data(), name.Data(), i ), weight(), 1080,   -0.5   ,   0.5    );
        hists.fill( wwwbaby.lep_ip3d()[ilep]       , Form( "%slep%s%d_ip3_wide"  , prefix.Data(), name.Data(), i ), weight(), 1080,   -0.5   ,   0.5    );
        hists.fill( wwwbaby.lep_ip3d()[ilep]       , Form( "%slep%s%d_ip3_widepp", prefix.Data(), name.Data(), i ), weight(), 1080,   -2.5   ,   2.5    );
        hists.fill( wwwbaby.lep_ip3d()[ilep]       , Form( "%slep%s%d_ip3calc"   , prefix.Data(), name.Data(), i ), weight(), 1080,   -0.05  ,   0.05   );
        hists.fill( wwwbaby.lep_dxy ()[ilep]       , Form( "%slep%s%d_dxy"       , prefix.Data(), name.Data(), i ), weight(), 1080,   -0.5   ,   0.5    );
        hists.fill( wwwbaby.lep_dz  ()[ilep]       , Form( "%slep%s%d_dz"        , prefix.Data(), name.Data(), i ), weight(), 1080,   -0.5   ,   0.5    );
        if ( wwwbaby.lep_ip3d()[ilep] > 0.5 )
            std::cout << wwwbaby.lep_ip3d()[ilep] << std::endl;
    }
}

//_________________________________________________________________________________________________
void fillJetHistograms( TasUtil::AutoHist& hists, TString categ, TString name, TString prefix )
{
    hists.fill( jetidx[categ].size() , Form( "%sjet%s_size" , prefix.Data(), name.Data() ), weight(), 5, 0, 5 );
    for ( unsigned int i = 0; i < jetidx[categ].size() && i < MAXOBJ; ++i )
    {
        int ijet = jetidx[categ][i];
        hists.fill( wwwbaby.jets_p4()[ijet].pt()    , Form( "%sjet%s%d_pt" , prefix.Data(), name.Data(), i ), weight(), 180,  0     , 180      );
        hists.fill( wwwbaby.jets_p4()[ijet].eta()   , Form( "%sjet%s%d_eta", prefix.Data(), name.Data(), i ), weight(), 180, -3     ,   3      );
        hists.fill( wwwbaby.jets_p4()[ijet].phi()   , Form( "%sjet%s%d_phi", prefix.Data(), name.Data(), i ), weight(), 180, -3.1416,   3.1416 );
        hists.fill( wwwbaby.jets_p4()[ijet].energy(), Form( "%sjet%s%d_E"  , prefix.Data(), name.Data(), i ), weight(), 180,  0     , 250      );
        hists.fill( wwwbaby.jets_csv()[ijet]        , Form( "%sjet%s%d_csv", prefix.Data(), name.Data(), i ), weight(), 180, -1     ,   1      );
    }
}

//_________________________________________________________________________________________________
void fillWWWHistograms( TasUtil::AutoHist& hists, TString prefix )
{
    hists.fill( wwwbaby.met_pt()                                  , Form( "%smet"        , prefix.Data() ) , weight() , 180 , 0. , 250.   );
    hists.fill( MjjW()                                            , Form( "%sMjjW"       , prefix.Data() ) , weight() , 180 , 0. , 160.   );
    hists.fill( MjjLead()                                         , Form( "%sMjjLead"    , prefix.Data() ) , weight() , 180 , 0. , 800.   );
    hists.fill( DEtajjLead()                                      , Form( "%sDEtajjLead" , prefix.Data() ) , weight() , 180 , 0. , 9.     );
    hists.fill( DPhill()                                          , Form( "%sDPhill"     , prefix.Data() ) , weight() , 180 , 0. , 3.1416 );
    hists.fill( DEtall()                                          , Form( "%sDEtall"     , prefix.Data() ) , weight() , 180 , 0. , 9.     );
    hists.fill( Mll()                                             , Form( "%sMll"        , prefix.Data() ) , weight() , 180 , 0. , 180.   );
    hists.fill( Mll()                                             , Form( "%sMll250"     , prefix.Data() ) , weight() , 180 , 0. , 250.   );
    hists.fill( Mll()                                             , Form( "%sMll500"     , prefix.Data() ) , weight() , 180 , 0. , 500.   );
    hists.fill( MTmax()                                           , Form( "%sMTmax"      , prefix.Data() ) , weight() , 180 , 0. , 180.   );
    hists.fill( M4()                                              , Form( "%sm4"         , prefix.Data() ) , weight() , 180 , 0. , 180.   );
    hists.fill( M4()                                              , Form( "%sm4wide"     , prefix.Data() ) , weight() , 150 , 0. , 1500.  );
    hists.fill( wwwbaby.nisoTrack_mt2_cleaned_VVV_cutbased_veto() , Form( "%snisotrack"  , prefix.Data() ) , weight() , 5   , 0  , 5      );
    hists.fill( wwwbaby.nlep_VVV_cutbased_veto()                  , Form( "%snvetolep"   , prefix.Data() ) , weight() , 5   , 0  , 5      );
    hists.fill( wwwbaby.nVert()                                   , Form( "%snvtx"       , prefix.Data() ) , weight() , 70  , 0  , 70.    );
    if ( lepidx["SignalLepton"].size() == 3 )
    {
        hists.fill( Pt3l()           , Form( "%sPt3l"         , prefix.Data() ) , weight() , 180 , 0. , 180.   );
        hists.fill( DPhi3lMET()      , Form( "%sDPhi3lMET"    , prefix.Data() ) , weight() , 180 , 0. , 3.1416 );
        if ( pass3L0SFOS() )
        {
            hists.fill( get0SFOSMll()    , Form( "%sget0SFOSMll"  , prefix.Data() ) , weight() , 180 , 0. , 180.   );
            hists.fill( get0SFOSMee()    , Form( "%sget0SFOSMee"  , prefix.Data() ) , weight() , 180 , 0. , 180.   );
        }
        if ( pass3L1SFOS() )
        {
            hists.fill( get1SFOSMll()    , Form( "%sget1SFOSMll"  , prefix.Data() ) , weight() , 180 , 0. , 180.   );
        }
        if ( pass3L2SFOS() )
        {
            hists.fill( get2SFOSMll0()   , Form( "%sget2SFOSMll0" , prefix.Data() ) , weight() , 180 , 0. , 180.   );
            hists.fill( get2SFOSMll1()   , Form( "%sget2SFOSMll1" , prefix.Data() ) , weight() , 180 , 0. , 180.   );
        }
    }
}

// eof

//    // If  did not pass two same sign leptons skip
//    if ( passpresel( cutidx ) )
//    {
//
//        doSSAnalysis( hists, cutidx );
//
////        // Now split based on lepton flavors
////        if ( isSSMM() && Mll() > 40. )
////        {
////            // Fill the histogram for after selecting two same sign muons with Mll > 40 inclusively
////            fillHistograms( hists, "SSMM_CutSSMMLep", 0 );
////            if ( isPlusPlus() ) fillHistograms( hists, "SSppMM_CutSSMMLep", 0 + 30 );
////            if ( isMinusMinus() ) fillHistograms( hists, "SSmmMM_CutSSMMLep", 0 + 60 );
////
////            // If it did not pass the >= 2jet requirements, then skip
////            if ( cutidx == 5 ) return;
////            fillHistograms( hists, "SSMM_CutNTwoJet", 1 );
////            if ( isPlusPlus() ) fillHistograms( hists, "SSppMM_CutNTwoJet", 1 + 30 );
////            if ( isMinusMinus() ) fillHistograms( hists, "SSmmMM_CutNTwoJet", 1 + 60 );
////
////            // If it did not pass the n veto lep == 2 cut, then skip
////            if ( cutidx == 6 ) return;
////            fillHistograms( hists, "SSMM_CutThirdLepVeto", 2 );
////            if ( isPlusPlus() ) fillHistograms( hists, "SSppMM_CutThirdLepVeto", 2 + 30 );
////            if ( isMinusMinus() ) fillHistograms( hists, "SSmmMM_CutThirdLepVeto", 2 + 60 );
////
////            // If it did not pass nisotrack == 0 cut, then skip
////            if ( cutidx == 7 ) return;
////            fillHistograms( hists, "SSMM_CutIsoTrackVeto", 3 );
////            if ( isPlusPlus() ) fillHistograms( hists, "SSppMM_CutIsoTrackVeto", 3 + 30 );
////            if ( isMinusMinus() ) fillHistograms( hists, "SSmmMM_CutIsoTrackVeto", 3 + 60 );
////
////            // Run the selection
////            passSScommon( 0, cutidx );
////
////            // If it did not pass N-bjet = 0 cut, then skip
////            if ( cutidx == 1 ) return;
////            fillHistograms( hists, "SSMM_CutBVeto", 4 );
////            if ( isPlusPlus() ) fillHistograms( hists, "SSppMM_CutBVeto", 4 + 30 );
////            if ( isMinusMinus() ) fillHistograms( hists, "SSmmMM_CutBVeto", 4 + 60 );
////
////            // If it did not pass W-mass cut, then skip
////            if ( cutidx == 2 ) return;
////            fillHistograms( hists, "SSMM_CutWMjj", 5 );
////            if ( isPlusPlus() ) fillHistograms( hists, "SSppMM_CutWMjj", 5 + 30 );
////            if ( isMinusMinus() ) fillHistograms( hists, "SSmmMM_CutWMjj", 5 + 60 );
////
////            // If it did not pass LowMjj cut, then skip (to kill VBS contributions)
////            if ( cutidx == 3 ) return;
////            fillHistograms( hists, "SSMM_CutLowMjj", 6 );
////            if ( isPlusPlus() ) fillHistograms( hists, "SSppMM_CutLowMjj", 6 + 30 );
////            if ( isMinusMinus() ) fillHistograms( hists, "SSmmMM_CutLowMjj", 6 + 60 );
////
////            // If it did not pass LowDEtajj cut, then skip (to kill VBS contributions)
////            if ( cutidx == 4 ) return;
////            fillHistograms( hists, "SSMM_CutLowDEtajj", 7 );
////            if ( isPlusPlus() ) fillHistograms( hists, "SSppMM_CutLowDEtajj", 7 + 30 );
////            if ( isMinusMinus() ) fillHistograms( hists, "SSmmMM_CutLowDEtajj", 7 + 60 );
////        }
////        else if ( isSSEM() && Mll() > 30. )
////        {
////            // Fill the histogram for after selecting one muon and one electron with same sign muons with Mll > 30 inclusively
////            fillHistograms( hists, "SSEM_CutSSEMLep", 10 );
////            if ( isPlusPlus() ) fillHistograms( hists, "SSppEM_CutSSEMLep", 10 + 30 );
////            if ( isMinusMinus() ) fillHistograms( hists, "SSmmEM_CutSSEMLep", 10 + 60 );
////
////            // If it did not pass the >= 2jet requirements, then skip
////            if ( cutidx == 5 ) return;
////            fillHistograms( hists, "SSEM_CutNTwoJet", 11 );
////            if ( isPlusPlus() ) fillHistograms( hists, "SSppEM_CutNTwoJet", 11 + 30 );
////            if ( isMinusMinus() ) fillHistograms( hists, "SSmmEM_CutNTwoJet", 11 + 60 );
////
////            // If it did not pass the n veto lep == 2 cut, then skip
////            if ( cutidx == 6 ) return;
////            fillHistograms( hists, "SSEM_CutThirdLepVeto", 12 );
////            if ( isPlusPlus() ) fillHistograms( hists, "SSppEM_CutThirdLepVeto", 12 + 30 );
////            if ( isMinusMinus() ) fillHistograms( hists, "SSmmEM_CutThirdLepVeto", 12 + 60 );
////
////            // If it did not pass nisotrack == 0 cut, then skip
////            if ( cutidx == 7 ) return;
////            fillHistograms( hists, "SSEM_CutIsoTrackVeto", 13 );
////            if ( isPlusPlus() ) fillHistograms( hists, "SSppEM_CutIsoTrackVeto", 13 + 30 );
////            if ( isMinusMinus() ) fillHistograms( hists, "SSmmEM_CutIsoTrackVeto", 13 + 60 );
////
////            // Run the selection
////            passSScommon( 0, cutidx );
////
////            // If it did not pass N-bjet = 0 cut, then skip
////            if ( cutidx == 1 ) return;
////            fillHistograms( hists, "SSEM_CutBVeto", 14 );
////            if ( isPlusPlus() ) fillHistograms( hists, "SSppEM_CutBVeto", 14 + 30 );
////            if ( isMinusMinus() ) fillHistograms( hists, "SSmmEM_CutBVeto", 14 + 60 );
////
////            // If it did not pass W-mass cut, then skip
////            if ( cutidx == 2 ) return;
////            fillHistograms( hists, "SSEM_CutWMjj", 15 );
////            if ( isPlusPlus() ) fillHistograms( hists, "SSppEM_CutWMjj", 15 + 30 );
////            if ( isMinusMinus() ) fillHistograms( hists, "SSmmEM_CutWMjj", 15 + 60 );
////
////            // If it did not pass LowMjj cut, then skip (to kill VBS contributions)
////            if ( cutidx == 3 ) return;
////            fillHistograms( hists, "SSEM_CutLowMjj", 16 );
////            if ( isPlusPlus() ) fillHistograms( hists, "SSppEM_CutLowMjj", 16 + 30 );
////            if ( isMinusMinus() ) fillHistograms( hists, "SSmmEM_CutLowMjj", 16 + 60 );
////
////            // If it did not pass LowDEtajj cut, then skip (to kill VBS contributions)
////            if ( cutidx == 4 ) return;
////            fillHistograms( hists, "SSEM_CutLowDEtajj", 17 );
////            if ( isPlusPlus() ) fillHistograms( hists, "SSppEM_CutLowDEtajj", 17 + 30 );
////            if ( isMinusMinus() ) fillHistograms( hists, "SSmmEM_CutLowDEtajj", 17 + 60 );
////
////            // If it did not pass MET cut, then skip
////            if (!( wwwbaby.met_pt() > 40. )) return;
////            fillHistograms( hists, "SSEM_CutMET", 18 );
////            if ( isPlusPlus() ) fillHistograms( hists, "SSppEM_CutMET", 18 + 30 );
////            if ( isMinusMinus() ) fillHistograms( hists, "SSmmEM_CutMET", 18 + 60 );
////
////            // If it did not pass MET cut, then skip
////            if (!( MTmax() > 90. )) return;
////            fillHistograms( hists, "SSEM_CutMET", 19 );
////            if ( isPlusPlus() ) fillHistograms( hists, "SSppEM_CutMET", 19 + 30 );
////            if ( isMinusMinus() ) fillHistograms( hists, "SSmmEM_CutMET", 19 + 60 );
////        }
////        else if ( isSSEE() && Mll() > 40. )
////        {
////            // Fill the histogram for after selecting one muon and one electron with same sign muons with Mll > 30 inclusively
////            fillHistograms( hists, "SSEE_CutSSEELep", 20 );
////            if ( isPlusPlus() ) fillHistograms( hists, "SSppEE_CutSSEELep", 20 + 30 );
////            if ( isMinusMinus() ) fillHistograms( hists, "SSmmEE_CutSSEELep", 20 + 60 );
////
////            // If in Z-peak reject
////            if (!( Mll() < 60. || Mll() > 100. )) return;
////            fillHistograms( hists, "SSEE_CutSSZVeto", 21 );
////            if ( isPlusPlus() ) fillHistograms( hists, "SSppEE_CutSSZVeto", 21 + 30 );
////            if ( isMinusMinus() ) fillHistograms( hists, "SSmmEE_CutSSZVeto", 21 + 60 );
////
////            // If it did not pass the >= 2jet requirements, then skip
////            if ( cutidx == 5 ) return;
////            fillHistograms( hists, "SSEE_CutNTwoJet", 22 );
////            if ( isPlusPlus() ) fillHistograms( hists, "SSppEE_CutNTwoJet", 22 + 30 );
////            if ( isMinusMinus() ) fillHistograms( hists, "SSmmEE_CutNTwoJet", 22 + 60 );
////
////            // If it did not pass the n veto lep == 2 cut, then skip
////            if ( cutidx == 6 ) return;
////            fillHistograms( hists, "SSEE_CutThirdLepVeto", 23 );
////            if ( isPlusPlus() ) fillHistograms( hists, "SSppEE_CutThirdLepVeto", 23 + 30 );
////            if ( isMinusMinus() ) fillHistograms( hists, "SSmmEE_CutThirdLepVeto", 23 + 60 );
////
////            // If it did not pass nisotrack == 0 cut, then skip
////            if ( cutidx == 7 ) return;
////            fillHistograms( hists, "SSEE_CutIsoTrackVeto", 24 );
////            if ( isPlusPlus() ) fillHistograms( hists, "SSppEE_CutIsoTrackVeto", 24 + 30 );
////            if ( isMinusMinus() ) fillHistograms( hists, "SSmmEE_CutIsoTrackVeto", 24 + 60 );
////
////            // Run the selection
////            passSScommon( 0, cutidx );
////
////            // If it did not pass N-bjet = 0 cut, then skip
////            if ( cutidx == 1 ) return;
////            fillHistograms( hists, "SSEE_CutBVeto", 25 );
////            if ( isPlusPlus() ) fillHistograms( hists, "SSppEE_CutBVeto", 25 + 30 );
////            if ( isMinusMinus() ) fillHistograms( hists, "SSmmEE_CutBVeto", 25 + 60 );
////
////            // If it did not pass W-mass cut, then skip
////            if ( cutidx == 2 ) return;
////            fillHistograms( hists, "SSEE_CutWMjj", 26 );
////            if ( isPlusPlus() ) fillHistograms( hists, "SSppEE_CutWMjj", 26 + 30 );
////            if ( isMinusMinus() ) fillHistograms( hists, "SSmmEE_CutWMjj", 26 + 60 );
////
////            // If it did not pass LowMjj cut, then skip (to kill VBS contributions)
////            if ( cutidx == 3 ) return;
////            fillHistograms( hists, "SSEE_CutLowMjj", 27 );
////            if ( isPlusPlus() ) fillHistograms( hists, "SSppEE_CutLowMjj", 27 + 30 );
////            if ( isMinusMinus() ) fillHistograms( hists, "SSmmEE_CutLowMjj", 27 + 60 );
////
////            // If it did not pass LowDEtajj cut, then skip (to kill VBS contributions)
////            if ( cutidx == 4 ) return;
////            fillHistograms( hists, "SSEE_CutLowDEtajj", 28 );
////            if ( isPlusPlus() ) fillHistograms( hists, "SSppEE_CutLowDEtajj", 28 + 30 );
////            if ( isMinusMinus() ) fillHistograms( hists, "SSmmEE_CutLowDEtajj", 28 + 60 );
////
////            // If it did not pass MET cut, then skip
////            if (!( wwwbaby.met_pt() > 40. )) return;
////            fillHistograms( hists, "SSEM_CutMET", 29 );
////            if ( isPlusPlus() ) fillHistograms( hists, "SSppEM_CutMET", 29 + 30 );
////            if ( isMinusMinus() ) fillHistograms( hists, "SSmmEM_CutMET", 29 + 60 );
////        }
//
//    }
////    else if ( passARpresel() )
////    {
////    }
//    else if ( lepidx["SignalLepton"].size() == 3)
//    {
//        if ( pass3L0SFOS() )
//            fillHistograms( hists, "3L0SFOS", 90 );
//
//        if ( pass3L1SFOS() )
//            fillHistograms( hists, "3L1SFOS", 91 );
//
//        if ( pass3L2SFOS() )
//            fillHistograms( hists, "3L2SFOS", 92 );
//    }

////_________________________________________________________________________________________________
//void doSSAnalysis( TasUtil::AutoHist& hists, int cutidx )
//{
//
//    // Now split based on lepton flavors
//    if ( isSSMM() && Mll() > 40. )
//    {
//        // Fill the histogram for after selecting two same sign muons with Mll > 40 inclusively
//        fillHistograms( hists, "SSMM_CutSSMMLep", 0 );
//        if ( isPlusPlus() ) fillHistograms( hists, "SSppMM_CutSSMMLep", 0 + 30 );
//        if ( isMinusMinus() ) fillHistograms( hists, "SSmmMM_CutSSMMLep", 0 + 60 );
//
//        // If it did not pass the >= 2jet requirements, then skip
//        if ( cutidx == 5 ) return;
//        fillHistograms( hists, "SSMM_CutNTwoJet", 1 );
//        if ( isPlusPlus() ) fillHistograms( hists, "SSppMM_CutNTwoJet", 1 + 30 );
//        if ( isMinusMinus() ) fillHistograms( hists, "SSmmMM_CutNTwoJet", 1 + 60 );
//
//        // If it did not pass the n veto lep == 2 cut, then skip
//        if ( cutidx == 6 ) return;
//        fillHistograms( hists, "SSMM_CutThirdLepVeto", 2 );
//        if ( isPlusPlus() ) fillHistograms( hists, "SSppMM_CutThirdLepVeto", 2 + 30 );
//        if ( isMinusMinus() ) fillHistograms( hists, "SSmmMM_CutThirdLepVeto", 2 + 60 );
//
//        // If it did not pass nisotrack == 0 cut, then skip
//        if ( cutidx == 7 ) return;
//        fillHistograms( hists, "SSMM_CutIsoTrackVeto", 3 );
//        if ( isPlusPlus() ) fillHistograms( hists, "SSppMM_CutIsoTrackVeto", 3 + 30 );
//        if ( isMinusMinus() ) fillHistograms( hists, "SSmmMM_CutIsoTrackVeto", 3 + 60 );
//
//        // Run the selection
//        passSScommon( 0, cutidx );
//
//        // If it did not pass N-bjet = 0 cut, then skip
//        if ( cutidx == 1 ) return;
//        fillHistograms( hists, "SSMM_CutBVeto", 4 );
//        if ( isPlusPlus() ) fillHistograms( hists, "SSppMM_CutBVeto", 4 + 30 );
//        if ( isMinusMinus() ) fillHistograms( hists, "SSmmMM_CutBVeto", 4 + 60 );
//
//        if ( bkgCategory().EqualTo( "fakes" ) )
//        {
//            if (
//                    ( wwwbaby.lep_isFromB()[lepidx["TightLepton"][0]] && wwwbaby.lep_isFromW()[lepidx["TightLepton"][1]] ) ||
//                    ( wwwbaby.lep_isFromB()[lepidx["TightLepton"][1]] && wwwbaby.lep_isFromW()[lepidx["TightLepton"][0]] )
//               )
//            {
//                std::cout << std::endl;
//                std::cout << wwwbaby.lep_isFromB()[lepidx["TightLepton"][0]] << std::endl;
//                std::cout << wwwbaby.lep_isFromB()[lepidx["TightLepton"][1]] << std::endl;
//                std::cout << wwwbaby.lep_isFromW()[lepidx["TightLepton"][0]] << std::endl;
//                std::cout << wwwbaby.lep_isFromW()[lepidx["TightLepton"][1]] << std::endl;
//                std::cout << wwwbaby.lep_relIso03EA()[lepidx["TightLepton"][0]] << std::endl;
//                std::cout << wwwbaby.lep_relIso03EA()[lepidx["TightLepton"][1]] << std::endl;
//                std::cout << wwwbaby.lep_p4()[lepidx["TightLepton"][0]].pt() << std::endl;
//                std::cout << wwwbaby.lep_p4()[lepidx["TightLepton"][1]].pt() << std::endl;
//                std::cout << wwwbaby.lep_p4()[lepidx["TightLepton"][0]].eta() << std::endl;
//                std::cout << wwwbaby.lep_p4()[lepidx["TightLepton"][1]].eta() << std::endl;
//                std::cout << wwwbaby.lep_p4()[lepidx["TightLepton"][0]].phi() << std::endl;
//                std::cout << wwwbaby.lep_p4()[lepidx["TightLepton"][1]].phi() << std::endl;
//                printEventID();
//            }
//        }
//
//        // If it did not pass W-mass cut, then skip
//        if ( cutidx == 2 ) return;
//        fillHistograms( hists, "SSMM_CutWMjj", 5 );
//        if ( isPlusPlus() ) fillHistograms( hists, "SSppMM_CutWMjj", 5 + 30 );
//        if ( isMinusMinus() ) fillHistograms( hists, "SSmmMM_CutWMjj", 5 + 60 );
//
//        // If it did not pass LowMjj cut, then skip (to kill VBS contributions)
//        if ( cutidx == 3 ) return;
//        fillHistograms( hists, "SSMM_CutLowMjj", 6 );
//        if ( isPlusPlus() ) fillHistograms( hists, "SSppMM_CutLowMjj", 6 + 30 );
//        if ( isMinusMinus() ) fillHistograms( hists, "SSmmMM_CutLowMjj", 6 + 60 );
//
//        // If it did not pass LowDEtajj cut, then skip (to kill VBS contributions)
//        if ( cutidx == 4 ) return;
//        fillHistograms( hists, "SSMM_CutLowDEtajj", 7 );
//        if ( isPlusPlus() ) fillHistograms( hists, "SSppMM_CutLowDEtajj", 7 + 30 );
//        if ( isMinusMinus() ) fillHistograms( hists, "SSmmMM_CutLowDEtajj", 7 + 60 );
//    }
//    else if ( isSSEM() && Mll() > 30. )
//    {
//        // Fill the histogram for after selecting one muon and one electron with same sign muons with Mll > 30 inclusively
//        fillHistograms( hists, "SSEM_CutSSEMLep", 10 );
//        if ( isPlusPlus() ) fillHistograms( hists, "SSppEM_CutSSEMLep", 10 + 30 );
//        if ( isMinusMinus() ) fillHistograms( hists, "SSmmEM_CutSSEMLep", 10 + 60 );
//
//        // If it did not pass the >= 2jet requirements, then skip
//        if ( cutidx == 5 ) return;
//        fillHistograms( hists, "SSEM_CutNTwoJet", 11 );
//        if ( isPlusPlus() ) fillHistograms( hists, "SSppEM_CutNTwoJet", 11 + 30 );
//        if ( isMinusMinus() ) fillHistograms( hists, "SSmmEM_CutNTwoJet", 11 + 60 );
//
//        // If it did not pass the n veto lep == 2 cut, then skip
//        if ( cutidx == 6 ) return;
//        fillHistograms( hists, "SSEM_CutThirdLepVeto", 12 );
//        if ( isPlusPlus() ) fillHistograms( hists, "SSppEM_CutThirdLepVeto", 12 + 30 );
//        if ( isMinusMinus() ) fillHistograms( hists, "SSmmEM_CutThirdLepVeto", 12 + 60 );
//
//        // If it did not pass nisotrack == 0 cut, then skip
//        if ( cutidx == 7 ) return;
//        fillHistograms( hists, "SSEM_CutIsoTrackVeto", 13 );
//        if ( isPlusPlus() ) fillHistograms( hists, "SSppEM_CutIsoTrackVeto", 13 + 30 );
//        if ( isMinusMinus() ) fillHistograms( hists, "SSmmEM_CutIsoTrackVeto", 13 + 60 );
//
//        // Run the selection
//        passSScommon( 0, cutidx );
//
//        // If it did not pass N-bjet = 0 cut, then skip
//        if ( cutidx == 1 ) return;
//        fillHistograms( hists, "SSEM_CutBVeto", 14 );
//        if ( isPlusPlus() ) fillHistograms( hists, "SSppEM_CutBVeto", 14 + 30 );
//        if ( isMinusMinus() ) fillHistograms( hists, "SSmmEM_CutBVeto", 14 + 60 );
//
//        // If it did not pass W-mass cut, then skip
//        if ( cutidx == 2 ) return;
//        fillHistograms( hists, "SSEM_CutWMjj", 15 );
//        if ( isPlusPlus() ) fillHistograms( hists, "SSppEM_CutWMjj", 15 + 30 );
//        if ( isMinusMinus() ) fillHistograms( hists, "SSmmEM_CutWMjj", 15 + 60 );
//
//        // If it did not pass LowMjj cut, then skip (to kill VBS contributions)
//        if ( cutidx == 3 ) return;
//        fillHistograms( hists, "SSEM_CutLowMjj", 16 );
//        if ( isPlusPlus() ) fillHistograms( hists, "SSppEM_CutLowMjj", 16 + 30 );
//        if ( isMinusMinus() ) fillHistograms( hists, "SSmmEM_CutLowMjj", 16 + 60 );
//
//        // If it did not pass LowDEtajj cut, then skip (to kill VBS contributions)
//        if ( cutidx == 4 ) return;
//        fillHistograms( hists, "SSEM_CutLowDEtajj", 17 );
//        if ( isPlusPlus() ) fillHistograms( hists, "SSppEM_CutLowDEtajj", 17 + 30 );
//        if ( isMinusMinus() ) fillHistograms( hists, "SSmmEM_CutLowDEtajj", 17 + 60 );
//
//        // If it did not pass MET cut, then skip
//        if (!( wwwbaby.met_pt() > 40. )) return;
//        fillHistograms( hists, "SSEM_CutMET", 18 );
//        if ( isPlusPlus() ) fillHistograms( hists, "SSppEM_CutMET", 18 + 30 );
//        if ( isMinusMinus() ) fillHistograms( hists, "SSmmEM_CutMET", 18 + 60 );
//
//        // If it did not pass MET cut, then skip
//        if (!( MTmax() > 90. )) return;
//        fillHistograms( hists, "SSEM_CutMET", 19 );
//        if ( isPlusPlus() ) fillHistograms( hists, "SSppEM_CutMET", 19 + 30 );
//        if ( isMinusMinus() ) fillHistograms( hists, "SSmmEM_CutMET", 19 + 60 );
//    }
//    else if ( isSSEE() && Mll() > 40. )
//    {
//        // Fill the histogram for after selecting one muon and one electron with same sign muons with Mll > 30 inclusively
//        fillHistograms( hists, "SSEE_CutSSEELep", 20 );
//        if ( isPlusPlus() ) fillHistograms( hists, "SSppEE_CutSSEELep", 20 + 30 );
//        if ( isMinusMinus() ) fillHistograms( hists, "SSmmEE_CutSSEELep", 20 + 60 );
//
//        // If in Z-peak reject
//        if (!( Mll() < 60. || Mll() > 100. )) return;
//        fillHistograms( hists, "SSEE_CutSSZVeto", 21 );
//        if ( isPlusPlus() ) fillHistograms( hists, "SSppEE_CutSSZVeto", 21 + 30 );
//        if ( isMinusMinus() ) fillHistograms( hists, "SSmmEE_CutSSZVeto", 21 + 60 );
//
//        // If it did not pass the >= 2jet requirements, then skip
//        if ( cutidx == 5 ) return;
//        fillHistograms( hists, "SSEE_CutNTwoJet", 22 );
//        if ( isPlusPlus() ) fillHistograms( hists, "SSppEE_CutNTwoJet", 22 + 30 );
//        if ( isMinusMinus() ) fillHistograms( hists, "SSmmEE_CutNTwoJet", 22 + 60 );
//
//        // If it did not pass the n veto lep == 2 cut, then skip
//        if ( cutidx == 6 ) return;
//        fillHistograms( hists, "SSEE_CutThirdLepVeto", 23 );
//        if ( isPlusPlus() ) fillHistograms( hists, "SSppEE_CutThirdLepVeto", 23 + 30 );
//        if ( isMinusMinus() ) fillHistograms( hists, "SSmmEE_CutThirdLepVeto", 23 + 60 );
//
//        // If it did not pass nisotrack == 0 cut, then skip
//        if ( cutidx == 7 ) return;
//        fillHistograms( hists, "SSEE_CutIsoTrackVeto", 24 );
//        if ( isPlusPlus() ) fillHistograms( hists, "SSppEE_CutIsoTrackVeto", 24 + 30 );
//        if ( isMinusMinus() ) fillHistograms( hists, "SSmmEE_CutIsoTrackVeto", 24 + 60 );
//
//        // Run the selection
//        passSScommon( 0, cutidx );
//
//        // If it did not pass N-bjet = 0 cut, then skip
//        if ( cutidx == 1 ) return;
//        fillHistograms( hists, "SSEE_CutBVeto", 25 );
//        if ( isPlusPlus() ) fillHistograms( hists, "SSppEE_CutBVeto", 25 + 30 );
//        if ( isMinusMinus() ) fillHistograms( hists, "SSmmEE_CutBVeto", 25 + 60 );
//
//        // If it did not pass W-mass cut, then skip
//        if ( cutidx == 2 ) return;
//        fillHistograms( hists, "SSEE_CutWMjj", 26 );
//        if ( isPlusPlus() ) fillHistograms( hists, "SSppEE_CutWMjj", 26 + 30 );
//        if ( isMinusMinus() ) fillHistograms( hists, "SSmmEE_CutWMjj", 26 + 60 );
//
//        // If it did not pass LowMjj cut, then skip (to kill VBS contributions)
//        if ( cutidx == 3 ) return;
//        fillHistograms( hists, "SSEE_CutLowMjj", 27 );
//        if ( isPlusPlus() ) fillHistograms( hists, "SSppEE_CutLowMjj", 27 + 30 );
//        if ( isMinusMinus() ) fillHistograms( hists, "SSmmEE_CutLowMjj", 27 + 60 );
//
//        // If it did not pass LowDEtajj cut, then skip (to kill VBS contributions)
//        if ( cutidx == 4 ) return;
//        fillHistograms( hists, "SSEE_CutLowDEtajj", 28 );
//        if ( isPlusPlus() ) fillHistograms( hists, "SSppEE_CutLowDEtajj", 28 + 30 );
//        if ( isMinusMinus() ) fillHistograms( hists, "SSmmEE_CutLowDEtajj", 28 + 60 );
//
//        // If it did not pass MET cut, then skip
//        if (!( wwwbaby.met_pt() > 40. )) return;
//        fillHistograms( hists, "SSEM_CutMET", 29 );
//        if ( isPlusPlus() ) fillHistograms( hists, "SSppEM_CutMET", 29 + 30 );
//        if ( isMinusMinus() ) fillHistograms( hists, "SSmmEM_CutMET", 29 + 60 );
//    }
//
//}
////_________________________________________________________________________________________________
///* Two SS lepton */
//bool passpresel( int& cutidx )
//{
//    passSSpresel( 0, cutidx );
//    if ( cutidx < 5 )
//        return false;
//    else
//        return true;
//}
////_________________________________________________________________________________________________
//bool passSkim()
//{
//    int cutidx = -1;
//
//    // If  did not pass two same sign leptons skip
//    if (!( passpresel( cutidx ) ))
//        return false;
//
//    // Now split based on lepton flavors
//    if ( isSSMM() && Mll() > 40. ) return true;
//    else if ( isSSEM() && Mll() > 30. ) return true;
//    else if ( isSSEE() && Mll() > 40. ) return true;
//    else return false;
//}
