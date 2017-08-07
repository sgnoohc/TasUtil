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
    bool issignal = isSignal( base_optstr );
    bool isdata = isData( base_optstr );

    std::cout << "baby version = " << babyver << std::endl;
    std::cout << "is signal = " << issignal << std::endl;
    std::cout << "is data = " << isdata << std::endl;

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
void doAnalysis( TasUtil::AutoHist& hists )
{
    int cutidx = -1;

    // If  did not pass two same sign leptons skip
    if (!( passpresel( cutidx ) ))
        return;

    // Now split based on lepton flavors
    if ( isSSMM() && MllSS() > 40. )
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
    else if ( isSSEM() && MllSS() > 30. )
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
        fillHistograms( hists, "SSEM_CutMET", 17 );

        // If it did not pass MET cut, then skip
        if (!( getMTmax() > 90. )) return;
        fillHistograms( hists, "SSEM_CutMET", 18 );
    }
    else if ( isSSEE() && MllSS() > 40. )
    {
        // Fill the histogram for after selecting one muon and one electron with same sign muons with Mll > 30 inclusively
        fillHistograms( hists, "SSEE_CutSSEELep", 20 );

        // If in Z-peak reject
        if (!( MllSS() < 80. || MllSS() > 100. )) return;
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
void mumuexcessstudy( TasUtil::AutoHist& hists )
{
    if ( lepidx["SignalLepton"].size() == 2 )
    {
        if (!( wwwbaby.lep_p4()[lepidx["SignalLepton"][0]].pt() > 30 )) return;
        if (!( wwwbaby.lep_p4()[lepidx["SignalLepton"][1]].pt() > 30 )) return;
        if ( LepFlavProduct() == 169 )
        {
            if (!( wwwbaby.HLT_DoubleMu_noiso() || wwwbaby.HLT_DoubleMu() )) return;
            fillHist( hists, "mm" );
        }
        else if ( LepFlavProduct() == 121 )
        {
            if (!( wwwbaby.HLT_DoubleEl_noiso() || wwwbaby.HLT_DoubleEl_DZ() || wwwbaby.HLT_DoubleEl_DZ_2() )) return;
            fillHist( hists, "ee" );
        }
    }
    if ( lepidx["SignalLepton"].size() == 2 && wwwbaby.nlep_VVV_cutbased_veto() == 2 )
    {
        if (!( wwwbaby.lep_p4()[lepidx["SignalLepton"][0]].pt() > 30 )) return;
        if (!( wwwbaby.lep_p4()[lepidx["SignalLepton"][1]].pt() > 30 )) return;
        if ( LepFlavProduct() == 169 )
        {
            if (!( wwwbaby.HLT_DoubleMu_noiso() || wwwbaby.HLT_DoubleMu() )) return;
            fillHist( hists, "mm_veto" );
        }
        else if ( LepFlavProduct() == 121 )
        {
            if (!( wwwbaby.HLT_DoubleEl_noiso() || wwwbaby.HLT_DoubleEl_DZ() || wwwbaby.HLT_DoubleEl_DZ_2() )) return;
            fillHist( hists, "ee_veto" );
        }
    }
    if ( lepidx["SignalLepton"].size() == 3 )
        WZStudy( hists, "SignalLepton", "" );
    if ( lepidx["SignalLepton"].size() >= 2 && lepidx["LooseLepton"].size() == 3 )
        WZStudy( hists, "LooseLepton", "_loose" );
    if ( lepidx["SignalLepton"].size() == 2 && lepidx["LooseLepton"].size() == 3 )
        WZStudy( hists, "LooseLepton", "_lbnt" );
}

//_________________________________________________________________________________________________
void WZStudy( TasUtil::AutoHist& hists, TString thirdlepcateg, TString suffix )
{
    if (!( wwwbaby.lep_p4()[lepidx["SignalLepton"][0]].pt() > 30 )) return;
    if (!( wwwbaby.lep_p4()[lepidx["SignalLepton"][1]].pt() > 30 )) return;
    if (!( wwwbaby.lep_p4()[lepidx[thirdlepcateg][2]].pt() > 10 )) return;
    int lepflavprod = wwwbaby.lep_pdgId()[lepidx["SignalLepton"][0]] * wwwbaby.lep_pdgId()[lepidx["SignalLepton"][1]];
    float mll = -999;
    if ( wwwbaby.lep_pdgId()[lepidx["SignalLepton"][0]] == -wwwbaby.lep_pdgId()[lepidx[thirdlepcateg][2]] )
        mll = ( wwwbaby.lep_p4()[lepidx["SignalLepton"][0]] + wwwbaby.lep_p4()[lepidx[thirdlepcateg][2]] ).mass();
    else if ( wwwbaby.lep_pdgId()[lepidx["SignalLepton"][1]] == -wwwbaby.lep_pdgId()[lepidx[thirdlepcateg][2]] )
        mll = ( wwwbaby.lep_p4()[lepidx["SignalLepton"][1]] + wwwbaby.lep_p4()[lepidx[thirdlepcateg][2]] ).mass();
    if ( fabs( mll - 91.1876 ) < 15. )
    {
        if ( lepflavprod == 169 )
        {
            if (!( wwwbaby.HLT_DoubleMu_noiso() || wwwbaby.HLT_DoubleMu() )) return;
            fillHist( hists, "mmm" + suffix );
        }
        else if ( lepflavprod == 121 )
        {
            if (!( wwwbaby.HLT_DoubleEl_noiso() || wwwbaby.HLT_DoubleEl_DZ() || wwwbaby.HLT_DoubleEl_DZ_2() )) return;
            fillHist( hists, "eee" + suffix );
        }
    }
}

//_________________________________________________________________________________________________
void fillHist( TasUtil::AutoHist& hists, TString prefix )
{
    float mll = ( wwwbaby.lep_p4()[lepidx["SignalLepton"][0]] + wwwbaby.lep_p4()[lepidx["SignalLepton"][1]] ).mass();
    hists.fill( wwwbaby.met_pt()                                   , Form( "%s_%s_met"         , sampleCategory().Data() , prefix.Data() ) , weight() , 1080 , 0       , 250.    );
    hists.fill( wwwbaby.nVert()                                    , Form( "%s_%s_nvtx"        , sampleCategory().Data() , prefix.Data() ) , weight() , 70   , 0       , 70.     );
    hists.fill( mll                                                , Form( "%s_%s_dimuon_mass" , sampleCategory().Data() , prefix.Data() ) , weight() , 1080 , 0       , 250.    );
    hists.fill( wwwbaby.nlep_VVV_cutbased_veto()                   , Form( "%s_%s_nvetolep"    , sampleCategory().Data() , prefix.Data() ) , weight() , 5    , 0       , 5       );
    hists.fill( wwwbaby.nlep_VVV_cutbased_fo()                     , Form( "%s_%s_nfolep"      , sampleCategory().Data() , prefix.Data() ) , weight() , 5    , 0       , 5       );
    hists.fill( wwwbaby.nisoTrack_mt2_cleaned_VVV_cutbased_veto()  , Form( "%s_%s_nisotrack"   , sampleCategory().Data() , prefix.Data() ) , weight() , 5    , 0       , 5       );
    fillLepHistograms( hists, "SignalLepton", ""      , prefix );
    fillLepHistograms( hists, "LooseLepton", ""      , prefix );
    fillJetHistograms( hists, "GoodSSJet"   , ""      , prefix );
    fillJetHistograms( hists, "LooseBJet"   , "b"     , prefix );
    fillJetHistograms( hists, "Good3LJet"   , "3l"    , prefix );
}

//_________________________________________________________________________________________________
void doSS( TasUtil::AutoHist& hists )
{
    if ( passSSMM() ) fillHistograms( hists, "SSMM_", 0 );
    if ( passSSEM() ) fillHistograms( hists, "SSem_", 1 );
    if ( passSSEE() ) fillHistograms( hists, "SSee_", 2 );
}

//_________________________________________________________________________________________________
void doSSSideband( TasUtil::AutoHist& hists )
{
    if ( passSSMM( 0 ) &&
         passSSpresel() &&
         fabs( MjjW() - 80. ) > 20. &&
         wwwbaby.met_pt() < 40. )
        fillHistograms( hists, "SSMM_LowMET_MjjSideBand_", 6 );
}

//_________________________________________________________________________________________________
void doSSAR( TasUtil::AutoHist& hists )
{
    if ( passSSARMM() ) fillHistograms( hists, "SSARmm_", 3 );
    if ( passSSAREM() ) fillHistograms( hists, "SSARem_", 4 );
    if ( passSSAREE() ) fillHistograms( hists, "SSARee_", 5 );
}

//_________________________________________________________________________________________________
void doSSMMFull( TasUtil::AutoHist& hists )
{
    // all pass
    int passSSMMidx     = -999;
    int passSScommonidx = -999;
    int passSSpreselidx = -999;
    const char* ds = sampleCategory().Data();
    passSSpresel( 0, passSSpreselidx );
    if ( passSSpreselidx == 1 ) return; hists.fill( 1 , Form( "%s_SSMM_cutflow", ds ), weight(), 14, 0, 14 ); fillHistograms( hists, "SSMM_Cut01", -999 );
    if ( passSSpreselidx == 2 ) return; hists.fill( 2 , Form( "%s_SSMM_cutflow", ds ), weight(), 14, 0, 14 ); fillHistograms( hists, "SSMM_Cut02", -999 );
    if ( passSSpreselidx == 3 ) return; hists.fill( 3 , Form( "%s_SSMM_cutflow", ds ), weight(), 14, 0, 14 ); fillHistograms( hists, "SSMM_Cut03", -999 );
    if ( passSSpreselidx == 4 ) return; hists.fill( 4 , Form( "%s_SSMM_cutflow", ds ), weight(), 14, 0, 14 ); fillHistograms( hists, "SSMM_Cut04", -999 );
    if ( passSSpreselidx == 5 ) return; hists.fill( 5 , Form( "%s_SSMM_cutflow", ds ), weight(), 14, 0, 14 ); fillHistograms( hists, "SSMM_Cut05", -999 );
    if ( passSSpreselidx == 6 ) return; hists.fill( 6 , Form( "%s_SSMM_cutflow", ds ), weight(), 14, 0, 14 ); fillHistograms( hists, "SSMM_Cut06", -999 );
    if ( passSSpreselidx == 7 ) return; hists.fill( 7 , Form( "%s_SSMM_cutflow", ds ), weight(), 14, 0, 14 ); fillHistograms( hists, "SSMM_Cut07", -999 );
    passSScommon( 0, passSScommonidx );
    if ( passSScommonidx == 1 ) return; hists.fill( 8 , Form( "%s_SSMM_cutflow", ds ), weight(), 14, 0, 14 ); fillHistograms( hists, "SSMM_Cut08", -999 );
    if ( passSScommonidx == 2 ) return; hists.fill( 9 , Form( "%s_SSMM_cutflow", ds ), weight(), 14, 0, 14 ); fillHistograms( hists, "SSMM_Cut09", -999 );
    if ( passSScommonidx == 3 ) return; hists.fill( 10, Form( "%s_SSMM_cutflow", ds ), weight(), 14, 0, 14 ); fillHistograms( hists, "SSMM_Cut10", -999 );
    if ( passSScommonidx == 4 ) return; hists.fill( 11, Form( "%s_SSMM_cutflow", ds ), weight(), 14, 0, 14 ); fillHistograms( hists, "SSMM_Cut11", -999 );
    passSSMM( 0, passSSMMidx );
    if ( passSSMMidx     == 1 ) return; hists.fill( 12, Form( "%s_SSMM_cutflow", ds ), weight(), 14, 0, 14 ); fillHistograms( hists, "SSMM_Cut12", -999 );
    if ( passSSMMidx     == 2 ) return; hists.fill( 13, Form( "%s_SSMM_cutflow", ds ), weight(), 14, 0, 14 ); fillHistograms( hists, "SSMM_Cut13", -999 );
}

//_________________________________________________________________________________________________
void fillHistograms( TasUtil::AutoHist& hists, TString prefix, int regionid )
{
    hists.fill( regionid, Form( "%s_Region_counter"   , sampleCategory().Data() ), weight(), 40, 0, 40 );
    hists.fill( regionid, Form( "%s_Region_rawcounter", sampleCategory().Data() ),        1, 40, 0, 40 );
    fillLepHistograms( hists, "SignalLepton", ""      , prefix );
    fillLepHistograms( hists, "TightLepton" , "tight" , prefix );
    fillLepHistograms( hists, "LooseLepton" , "loose" , prefix );
    fillLepHistograms( hists, "LbntLepton"  , "lbnt"  , prefix );
    fillJetHistograms( hists, "GoodSSJet"   , ""      , prefix );
    fillJetHistograms( hists, "LooseBJet"   , "b"     , prefix );
    fillJetHistograms( hists, "Good3LJet"   , "3l"    , prefix );
    fillJetHistograms( hists, "GoodSSWJet"  , "wtag"  , prefix );
    fillWWWHistograms( hists, prefix );
}

//_________________________________________________________________________________________________
void fillLepHistograms( TasUtil::AutoHist& hists, TString categ, TString name, TString prefix )
{
    hists.fill( lepidx[categ].size() , Form( "%s_%slep%s_size" , sampleCategory().Data(), prefix.Data(), name.Data() ), weight(), 5, 0, 5 );
    for ( unsigned int i = 0; i < lepidx[categ].size() && i < MAXOBJ; ++i )
    {
        int ilep = lepidx[categ][i];
        hists.fill( wwwbaby.lep_pdgId()[ilep]      , Form( "%s_%slep%s%d_pid", sampleCategory().Data(), prefix.Data(), name.Data(), i ), weight(),   40,  -20     ,  20      );
        hists.fill( wwwbaby.lep_p4()[ilep].pt()    , Form( "%s_%slep%s%d_pt" , sampleCategory().Data(), prefix.Data(), name.Data(), i ), weight(), 1080,    0     , 250.     );
        hists.fill( wwwbaby.lep_p4()[ilep].eta()   , Form( "%s_%slep%s%d_eta", sampleCategory().Data(), prefix.Data(), name.Data(), i ), weight(), 1080,   -3     ,   3      );
        hists.fill( wwwbaby.lep_p4()[ilep].phi()   , Form( "%s_%slep%s%d_phi", sampleCategory().Data(), prefix.Data(), name.Data(), i ), weight(), 1080,   -3.1416,   3.1416 );
        hists.fill( wwwbaby.lep_p4()[ilep].energy(), Form( "%s_%slep%s%d_E"  , sampleCategory().Data(), prefix.Data(), name.Data(), i ), weight(), 1080,    0     , 250.     );
        hists.fill( wwwbaby.lep_relIso03EA()[ilep] , Form( "%s_%slep%s%d_iso", sampleCategory().Data(), prefix.Data(), name.Data(), i ), weight(), 1080,    0     ,   0.1    );
        hists.fill( wwwbaby.lep_ip3d()[ilep]       , Form( "%s_%slep%s%d_ip3", sampleCategory().Data(), prefix.Data(), name.Data(), i ), weight(), 1080,    0     ,   0.05   );
    }
}

//_________________________________________________________________________________________________
void fillJetHistograms( TasUtil::AutoHist& hists, TString categ, TString name, TString prefix )
{
    hists.fill( jetidx[categ].size() , Form( "%s_%sjet%s_size" , sampleCategory().Data(), prefix.Data(), name.Data() ), weight(), 7, 0, 7 );
    for ( unsigned int i = 0; i < jetidx[categ].size() && i < MAXOBJ; ++i )
    {
        int ijet = jetidx[categ][i];
        hists.fill( wwwbaby.jets_p4()[ijet].pt()    , Form( "%s_%sjet%s%d_pt" , sampleCategory().Data(), prefix.Data(), name.Data(), i ), weight(), 180,  0     , 180      );
        hists.fill( wwwbaby.jets_p4()[ijet].eta()   , Form( "%s_%sjet%s%d_eta", sampleCategory().Data(), prefix.Data(), name.Data(), i ), weight(), 180, -3     ,   3      );
        hists.fill( wwwbaby.jets_p4()[ijet].phi()   , Form( "%s_%sjet%s%d_phi", sampleCategory().Data(), prefix.Data(), name.Data(), i ), weight(), 180, -3.1416,   3.1416 );
        hists.fill( wwwbaby.jets_p4()[ijet].energy(), Form( "%s_%sjet%s%d_E"  , sampleCategory().Data(), prefix.Data(), name.Data(), i ), weight(), 180,  0     , 250      );
    }
}

//_________________________________________________________________________________________________
void fillWWWHistograms( TasUtil::AutoHist& hists, TString prefix )
{
    hists.fill( wwwbaby.met_pt()                                  , Form( "%s_%smet"        , sampleCategory().Data() , prefix.Data() ) , weight() , 180 , 0. , 250.   );
    hists.fill( MjjW()                                            , Form( "%s_%sMjjW"       , sampleCategory().Data() , prefix.Data() ) , weight() , 180 , 0. , 160.   );
    hists.fill( MjjLead()                                         , Form( "%s_%sMjjLead"    , sampleCategory().Data() , prefix.Data() ) , weight() , 180 , 0. , 800.   );
    hists.fill( DEtajjLead()                                      , Form( "%s_%sDEtajjLead" , sampleCategory().Data() , prefix.Data() ) , weight() , 180 , 0. , 9.     );
    hists.fill( MllSS()                                           , Form( "%s_%sMllSS"      , sampleCategory().Data() , prefix.Data() ) , weight() , 180 , 0. , 180.   );
    hists.fill( getMTmax()                                        , Form( "%s_%sgetMTmax"   , sampleCategory().Data() , prefix.Data() ) , weight() , 180 , 0. , 180.   );
    hists.fill( M4()                                              , Form( "%s_%sm4"         , sampleCategory().Data() , prefix.Data() ) , weight() , 180 , 0. , 180.   );
    hists.fill( M4()                                              , Form( "%s_%sm4wide"     , sampleCategory().Data() , prefix.Data() ) , weight() , 150 , 0. , 1500.  );
    hists.fill( wwwbaby.nisoTrack_mt2_cleaned_VVV_cutbased_veto() , Form( "%s_%snisotrack"  , sampleCategory().Data() , prefix.Data() ) , weight() , 5   , 0  , 5       );
    hists.fill( wwwbaby.nVert()                                   , Form( "%s_%snvtx"       , sampleCategory().Data() , prefix.Data() ) , weight() , 70  , 0  , 70.     );
//    hists.fill( Pt3l()           , Form( "%s_%sPt3l"         , sampleCategory().Data() , prefix.Data() ) , weight() , 180 , 0. , 180.   );
//    hists.fill( DPhi3lMET()      , Form( "%s_%sDPhi3lMET"    , sampleCategory().Data() , prefix.Data() ) , weight() , 180 , 0. , 3.1416 );
//    hists.fill( get0SFOSMll()    , Form( "%s_%sget0SFOSMll"  , sampleCategory().Data() , prefix.Data() ) , weight() , 180 , 0. , 180.   );
//    hists.fill( get0SFOSMee()    , Form( "%s_%sget0SFOSMee"  , sampleCategory().Data() , prefix.Data() ) , weight() , 180 , 0. , 180.   );
//    hists.fill( get1SFOSMll()    , Form( "%s_%sget1SFOSMll"  , sampleCategory().Data() , prefix.Data() ) , weight() , 180 , 0. , 180.   );
//    hists.fill( get2SFOSMll0()   , Form( "%s_%sget2SFOSMll0" , sampleCategory().Data() , prefix.Data() ) , weight() , 180 , 0. , 180.   );
//    hists.fill( get2SFOSMll1()   , Form( "%s_%sget2SFOSMll1" , sampleCategory().Data() , prefix.Data() ) , weight() , 180 , 0. , 180.   );
}

// eof
