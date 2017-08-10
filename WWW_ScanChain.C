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

    // -~-~-~-~-~
    // Set output
    // -~-~-~-~-~
    TasUtil::AutoHist hists;
    while ( looper.nextEvent() )
    {
        setObjectIndices();
        doAnalysis( hists );
    }
    hists.save( output_name );
}

//=================================================================================================
//=================================================================================================
//=================================================================================================

//_________________________________________________________________________________________________
bool passSkim()
{
    int cutidx = -1;

    // If  did not pass two same sign leptons skip
    if (!( passpresel( cutidx ) ))
        return false;

    // Now split based on lepton flavors
    if ( isSSMM() && Mll() > 40. ) return true;
    else if ( isSSEM() && Mll() > 30. ) return true;
    else if ( isSSEE() && Mll() > 40. ) return true;
    else return false;
}

//_________________________________________________________________________________________________
void doAnalysis( TasUtil::AutoHist& hists )
{
    int cutidx = -1;

    // If  did not pass two same sign leptons skip
    if (!( passpresel( cutidx ) ))
        return;

    // Now split based on lepton flavors
    if ( isSSMM() && Mll() > 40. )
    {
        // Fill the histogram for after selecting two same sign muons with Mll > 40 inclusively
        fillHistograms( hists, "SSMM_CutSSMMLep", 0 );

        // If it did not pass the >= 2jet requirements, then skip
        if ( cutidx == 5 ) return;
        fillHistograms( hists, "SSMM_CutNTwoJet", 1 );

        // If it did not pass the n veto lep == 2 cut, then skip
        if ( cutidx == 6 ) return;
        fillHistograms( hists, "SSMM_CutThirdLepVeto", 2 );

        // If it did not pass nisotrack == 0 cut, then skip
        if ( cutidx == 7 ) return;
        fillHistograms( hists, "SSMM_CutIsoTrackVeto", 3 );

        // Run the selection
        passSScommon( 0, cutidx );

        // If it did not pass N-bjet = 0 cut, then skip
        if ( cutidx == 1 ) return;
        fillHistograms( hists, "SSMM_CutBVeto", 4 );

        // If it did not pass W-mass cut, then skip
        if ( cutidx == 2 ) return;
        fillHistograms( hists, "SSMM_CutWMjj", 5 );

        // If it did not pass LowMjj cut, then skip (to kill VBS contributions)
        if ( cutidx == 3 ) return;
        fillHistograms( hists, "SSMM_CutLowMjj", 6 );

        // If it did not pass LowDEtajj cut, then skip (to kill VBS contributions)
        if ( cutidx == 4 ) return;
        fillHistograms( hists, "SSMM_CutLowDEtajj", 7 );
    }
    else if ( isSSEM() && Mll() > 30. )
    {
        // Fill the histogram for after selecting one muon and one electron with same sign muons with Mll > 30 inclusively
        fillHistograms( hists, "SSEM_CutSSEMLep", 10 );

        // If it did not pass the >= 2jet requirements, then skip
        if ( cutidx == 5 ) return;
        fillHistograms( hists, "SSEM_CutNTwoJet", 11 );

        // If it did not pass the n veto lep == 2 cut, then skip
        if ( cutidx == 6 ) return;
        fillHistograms( hists, "SSEM_CutThirdLepVeto", 12 );

        // If it did not pass nisotrack == 0 cut, then skip
        if ( cutidx == 7 ) return;
        fillHistograms( hists, "SSEM_CutIsoTrackVeto", 13 );

        // Run the selection
        passSScommon( 0, cutidx );

        // If it did not pass N-bjet = 0 cut, then skip
        if ( cutidx == 1 ) return;
        fillHistograms( hists, "SSEM_CutBVeto", 14 );

        // If it did not pass W-mass cut, then skip
        if ( cutidx == 2 ) return;
        fillHistograms( hists, "SSEM_CutWMjj", 15 );

        // If it did not pass LowMjj cut, then skip (to kill VBS contributions)
        if ( cutidx == 3 ) return;
        fillHistograms( hists, "SSEM_CutLowMjj", 16 );

        // If it did not pass LowDEtajj cut, then skip (to kill VBS contributions)
        if ( cutidx == 4 ) return;
        fillHistograms( hists, "SSEM_CutLowDEtajj", 17 );

        // If it did not pass MET cut, then skip
        if (!( wwwbaby.met_pt() > 40. )) return;
        fillHistograms( hists, "SSEM_CutMET", 18 );

        // If it did not pass MET cut, then skip
        if (!( MTmax() > 90. )) return;
        fillHistograms( hists, "SSEM_CutMET", 19 );
    }
    else if ( isSSEE() && Mll() > 40. )
    {
        // Fill the histogram for after selecting one muon and one electron with same sign muons with Mll > 30 inclusively
        fillHistograms( hists, "SSEE_CutSSEELep", 20 );

        // If in Z-peak reject
        if (!( Mll() < 80. || Mll() > 100. )) return;
        fillHistograms( hists, "SSEE_CutSSZVeto", 21 );

        // If it did not pass the >= 2jet requirements, then skip
        if ( cutidx == 5 ) return;
        fillHistograms( hists, "SSEE_CutNTwoJet", 22 );

        // If it did not pass the n veto lep == 2 cut, then skip
        if ( cutidx == 6 ) return;
        fillHistograms( hists, "SSEE_CutThirdLepVeto", 23 );

        // If it did not pass nisotrack == 0 cut, then skip
        if ( cutidx == 7 ) return;
        fillHistograms( hists, "SSEE_CutIsoTrackVeto", 24 );

        // Run the selection
        passSScommon( 0, cutidx );

        // If it did not pass N-bjet = 0 cut, then skip
        if ( cutidx == 1 ) return;
        fillHistograms( hists, "SSEE_CutBVeto", 25 );

        // If it did not pass W-mass cut, then skip
        if ( cutidx == 2 ) return;
        fillHistograms( hists, "SSEE_CutWMjj", 26 );

        // If it did not pass LowMjj cut, then skip (to kill VBS contributions)
        if ( cutidx == 3 ) return;
        fillHistograms( hists, "SSEE_CutLowMjj", 27 );

        // If it did not pass LowDEtajj cut, then skip (to kill VBS contributions)
        if ( cutidx == 4 ) return;
        fillHistograms( hists, "SSEE_CutLowDEtajj", 28 );

        // If it did not pass MET cut, then skip
        if (!( wwwbaby.met_pt() > 40. )) return;
        fillHistograms( hists, "SSEM_CutMET", 29 );
    }
}

//_________________________________________________________________________________________________
/* Two SS lepton */
bool passpresel( int& cutidx )
{
    passSSpresel( 0, cutidx );
    if ( cutidx < 5 )
        return false;
    else
        return true;
}

//_________________________________________________________________________________________________
void fillHistograms( TasUtil::AutoHist& hists, TString prefix, int regionid )
{
    TString sample_category = sampleCategory();
    TString bkg_category = bkgCategory();
    TString empty = "";
    fillHistogramsFull( hists, sample_category, bkg_category, prefix, regionid );
    fillHistogramsFull( hists, sample_category, empty       , prefix, regionid );
}

//_________________________________________________________________________________________________
void fillHistogramsFull( TasUtil::AutoHist& hists, TString sample_category, TString bkg_category, TString prefix, int regionid )
{
    TString procprefix = sample_category + "_" + bkg_category;
    hists.fill( regionid, Form( "%s_Region_counter"   , procprefix.Data() ), weight(), 40, 0, 40 );
    hists.fill( regionid, Form( "%s_Region_rawcounter", procprefix.Data() ),        1, 40, 0, 40 );
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
        hists.fill( wwwbaby.lep_pdgId()[ilep]      , Form( "%slep%s%d_pid", prefix.Data(), name.Data(), i ), weight(),   40,  -20     ,  20      );
        hists.fill( wwwbaby.lep_p4()[ilep].pt()    , Form( "%slep%s%d_pt" , prefix.Data(), name.Data(), i ), weight(), 1080,    0     , 250.     );
        hists.fill( wwwbaby.lep_p4()[ilep].eta()   , Form( "%slep%s%d_eta", prefix.Data(), name.Data(), i ), weight(), 1080,   -3     ,   3      );
        hists.fill( wwwbaby.lep_p4()[ilep].phi()   , Form( "%slep%s%d_phi", prefix.Data(), name.Data(), i ), weight(), 1080,   -3.1416,   3.1416 );
        hists.fill( wwwbaby.lep_p4()[ilep].energy(), Form( "%slep%s%d_E"  , prefix.Data(), name.Data(), i ), weight(), 1080,    0     , 250.     );
        hists.fill( wwwbaby.lep_relIso03EA()[ilep] , Form( "%slep%s%d_iso", prefix.Data(), name.Data(), i ), weight(), 1080,    0     ,   0.1    );
        hists.fill( wwwbaby.lep_ip3d()[ilep]       , Form( "%slep%s%d_ip3", prefix.Data(), name.Data(), i ), weight(), 1080,    0     ,   0.05   );
    }
}

//_________________________________________________________________________________________________
void fillJetHistograms( TasUtil::AutoHist& hists, TString categ, TString name, TString prefix )
{
    hists.fill( jetidx[categ].size() , Form( "%sjet%s_size" , prefix.Data(), name.Data() ), weight(), 7, 0, 7 );
    for ( unsigned int i = 0; i < jetidx[categ].size() && i < MAXOBJ; ++i )
    {
        int ijet = jetidx[categ][i];
        hists.fill( wwwbaby.jets_p4()[ijet].pt()    , Form( "%sjet%s%d_pt" , prefix.Data(), name.Data(), i ), weight(), 180,  0     , 180      );
        hists.fill( wwwbaby.jets_p4()[ijet].eta()   , Form( "%sjet%s%d_eta", prefix.Data(), name.Data(), i ), weight(), 180, -3     ,   3      );
        hists.fill( wwwbaby.jets_p4()[ijet].phi()   , Form( "%sjet%s%d_phi", prefix.Data(), name.Data(), i ), weight(), 180, -3.1416,   3.1416 );
        hists.fill( wwwbaby.jets_p4()[ijet].energy(), Form( "%sjet%s%d_E"  , prefix.Data(), name.Data(), i ), weight(), 180,  0     , 250      );
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
//    hists.fill( Pt3l()           , Form( "%sPt3l"         , prefix.Data() ) , weight() , 180 , 0. , 180.   );
//    hists.fill( DPhi3lMET()      , Form( "%sDPhi3lMET"    , prefix.Data() ) , weight() , 180 , 0. , 3.1416 );
//    hists.fill( get0SFOSMll()    , Form( "%sget0SFOSMll"  , prefix.Data() ) , weight() , 180 , 0. , 180.   );
//    hists.fill( get0SFOSMee()    , Form( "%sget0SFOSMee"  , prefix.Data() ) , weight() , 180 , 0. , 180.   );
//    hists.fill( get1SFOSMll()    , Form( "%sget1SFOSMll"  , prefix.Data() ) , weight() , 180 , 0. , 180.   );
//    hists.fill( get2SFOSMll0()   , Form( "%sget2SFOSMll0" , prefix.Data() ) , weight() , 180 , 0. , 180.   );
//    hists.fill( get2SFOSMll1()   , Form( "%sget2SFOSMll1" , prefix.Data() ) , weight() , 180 , 0. , 180.   );
}

// eof
