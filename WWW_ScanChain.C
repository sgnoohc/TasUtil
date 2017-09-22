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
    if ( !base_optstr.Contains( "wwwevents" ) )
        looper.setSkim( "output_wwwevents.root" );
    TasUtil::AutoHist hists;

    while ( looper.nextEvent() )
    {
        if ( wwwbaby.isData() )
        {
            duplicate_removal::DorkyEventIdentifier
                id( wwwbaby.run(), wwwbaby.evt(), wwwbaby.lumi() );
            if ( is_duplicate( id ) )
                continue;
        }

        setObjectIndices();

        if ( event_list.has( wwwbaby.evt(), wwwbaby.run(), wwwbaby.lumi() ) )
            printEvent();

        if ( doAnalysis( hists ) )
            if ( !base_optstr.Contains( "wwwevents" ) )
                looper.fillSkim();
    }
    if ( !base_optstr.Contains( "wwwevents" ) )
        looper.saveSkim();
    looper.getTTreePerfStats()->SaveAs( "perf.root" );
    hists.save( output_name );
}

//_________________________________________________________________________________________________
bool doAnalysis( TasUtil::AutoHist& hists )
{
    bool passed = false;
    for ( int isyst = 0; isyst < NSYST; ++isyst )
    {
        if ( passSSEE              () ) fillHistograms ( passed, hists, "SSEE"             , 0 , isyst );
        if ( passSSEM              () ) fillHistograms ( passed, hists, "SSEM"             , 1 , isyst );
        if ( passSSMM              () ) fillHistograms ( passed, hists, "SSMM"             , 2 , isyst );
        if ( pass3L0SFOS           () ) fillHistograms ( passed, hists, "3L0SFOS"          , 3 , isyst );
        if ( pass3L1SFOS           () ) fillHistograms ( passed, hists, "3L1SFOS"          , 4 , isyst );
        if ( pass3L2SFOS           () ) fillHistograms ( passed, hists, "3L2SFOS"          , 5 , isyst );
        if ( passBTagVRSSEE        () ) fillHistograms ( passed, hists, "BTagVRSSEE"       , 6 , isyst );
        if ( passBTagVRSSEM        () ) fillHistograms ( passed, hists, "BTagVRSSEM"       , 7 , isyst );
        if ( passBTagVRSSMM        () ) fillHistograms ( passed, hists, "BTagVRSSMM"       , 8 , isyst );
        if ( passBTagARSSEE        () ) fillHistograms ( passed, hists, "BTagARSSEE"       , 9 , isyst );
        if ( passBTagARSSEM        () ) fillHistograms ( passed, hists, "BTagARSSEM"       , 10, isyst );
        if ( passBTagARSSMM        () ) fillHistograms ( passed, hists, "BTagARSSMM"       , 11, isyst );
        if ( passSSAREE            () ) fillHistograms ( passed, hists, "SSAREE"           , 12, isyst );
        if ( passSSAREM            () ) fillHistograms ( passed, hists, "SSAREM"           , 13, isyst );
        if ( passSSARMM            () ) fillHistograms ( passed, hists, "SSARMM"           , 14, isyst );
        if ( passMjjSBVRSSEE       () ) fillHistograms ( passed, hists, "MjjSBVRSSEE"      , 15, isyst );
        if ( passMjjSBVRSSEM       () ) fillHistograms ( passed, hists, "MjjSBVRSSEM"      , 16, isyst );
        if ( passMjjSBVRSSMM       () ) fillHistograms ( passed, hists, "MjjSBVRSSMM"      , 17, isyst );
        if ( passMjjSBARSSEE       () ) fillHistograms ( passed, hists, "MjjSBARSSEE"      , 18, isyst );
        if ( passMjjSBARSSEM       () ) fillHistograms ( passed, hists, "MjjSBARSSEM"      , 19, isyst );
        if ( passMjjSBARSSMM       () ) fillHistograms ( passed, hists, "MjjSBARSSMM"      , 20, isyst );
        if ( passSSEEPred          () ) fillHistograms ( passed, hists, "SSEEPred"         , 21, isyst );
        if ( passSSEMPred          () ) fillHistograms ( passed, hists, "SSEMPred"         , 22, isyst );
        if ( passSSMMPred          () ) fillHistograms ( passed, hists, "SSMMPred"         , 23, isyst );
        if ( passSSAREEPred        () ) fillHistograms ( passed, hists, "SSAREEPred"       , 21, isyst );
        if ( passSSAREMPred        () ) fillHistograms ( passed, hists, "SSAREMPred"       , 22, isyst );
        if ( passSSARMMPred        () ) fillHistograms ( passed, hists, "SSARMMPred"       , 23, isyst );
        if ( passBTagVRSSEEPred    () ) fillHistograms ( passed, hists, "BTagVRSSEEPred"   , 24, isyst );
        if ( passBTagVRSSEMPred    () ) fillHistograms ( passed, hists, "BTagVRSSEMPred"   , 25, isyst );
        if ( passBTagVRSSMMPred    () ) fillHistograms ( passed, hists, "BTagVRSSMMPred"   , 26, isyst );
        if ( passBTagARSSEEPred    () ) fillHistograms ( passed, hists, "BTagARSSEEPred"   , 24, isyst );
        if ( passBTagARSSEMPred    () ) fillHistograms ( passed, hists, "BTagARSSEMPred"   , 25, isyst );
        if ( passBTagARSSMMPred    () ) fillHistograms ( passed, hists, "BTagARSSMMPred"   , 26, isyst );
        if ( passMjjSBVRSSEEPred   () ) fillHistograms ( passed, hists, "MjjSBVRSSEEPred"  , 27, isyst );
        if ( passMjjSBVRSSEMPred   () ) fillHistograms ( passed, hists, "MjjSBVRSSEMPred"  , 28, isyst );
        if ( passMjjSBVRSSMMPred   () ) fillHistograms ( passed, hists, "MjjSBVRSSMMPred"  , 29, isyst );
        if ( passMjjSBARSSEEPred   () ) fillHistograms ( passed, hists, "MjjSBARSSEEPred"  , 27, isyst );
        if ( passMjjSBARSSEMPred   () ) fillHistograms ( passed, hists, "MjjSBARSSEMPred"  , 28, isyst );
        if ( passMjjSBARSSMMPred   () ) fillHistograms ( passed, hists, "MjjSBARSSMMPred"  , 29, isyst );
        if ( passMjjSBPRVRSSEE     () ) fillHistograms ( passed, hists, "MjjSBPRVRSSEE"    , 30, isyst );
        if ( passMjjSBPRVRSSEM     () ) fillHistograms ( passed, hists, "MjjSBPRVRSSEM"    , 31, isyst );
        if ( passMjjSBPRVRSSMM     () ) fillHistograms ( passed, hists, "MjjSBPRVRSSMM"    , 32, isyst );
        if ( passMjjSBPRARSSEE     () ) fillHistograms ( passed, hists, "MjjSBPRARSSEE"    , 33, isyst );
        if ( passMjjSBPRARSSEM     () ) fillHistograms ( passed, hists, "MjjSBPRARSSEM"    , 34, isyst );
        if ( passMjjSBPRARSSMM     () ) fillHistograms ( passed, hists, "MjjSBPRARSSMM"    , 35, isyst );
        if ( passMjjSBPRVRSSEEPred () ) fillHistograms ( passed, hists, "MjjSBPRVRSSEEPred", 36, isyst );
        if ( passMjjSBPRVRSSEMPred () ) fillHistograms ( passed, hists, "MjjSBPRVRSSEMPred", 37, isyst );
        if ( passMjjSBPRVRSSMMPred () ) fillHistograms ( passed, hists, "MjjSBPRVRSSMMPred", 38, isyst );
        if ( passMjjSBPRARSSEEPred () ) fillHistograms ( passed, hists, "MjjSBPRARSSEEPred", 36, isyst );
        if ( passMjjSBPRARSSEMPred () ) fillHistograms ( passed, hists, "MjjSBPRARSSEMPred", 37, isyst );
        if ( passMjjSBPRARSSMMPred () ) fillHistograms ( passed, hists, "MjjSBPRARSSMMPred", 38, isyst );
    }
    return passed;
}

//=================================================================================================
//=================================================================================================
//=================================================================================================

//_________________________________________________________________________________________________
void fillHistograms( bool& passed, TasUtil::AutoHist& hists, TString prefix, int regionid, int isyst )
{

    passed = true;

    // Print event lists
    if ( wwwbaby.isData() )
        printevent( prefix );

    // Sample categories (e.g. Z, DY.. or fake, trueSS etc.)
    int sample_priority = -1;
    TString sample_category = sampleCategory( sample_priority );
    TString bkg_category = bkgCategory();
    TString empty = "";

    // If it's for a prediction the data category is replaced by "fakepred"
    if ( wwwbaby.isData() && prefix.Contains( "AR" ) && prefix.Contains( "Pred" ) )
    {
        sample_category = "fakepred";
        bkg_category = "fakepred";
    }

    // Fill histograms for the MC boundary plots
    fillHistogramsFull( hists, sample_category, empty, prefix, regionid, isyst );

    // Fill histograms for the bkg type boundary plots
    if ( sample_priority == 1 )
        fillHistogramsFull( hists, empty, bkg_category, prefix, regionid, isyst );

    // The following fill will only occur for counter categories
    fillHistogramsFull( hists, sample_category, bkg_category, prefix, regionid, isyst );
}

//_________________________________________________________________________________________________
void fillHistogramsFull(
        TasUtil::AutoHist& hists,
        TString sample_category,
        TString bkg_category,
        TString prefix,
        int regionid,
        int isyst )
{
    // Compute a boolean whether to use fakefactor or not.
    bool ff = prefix.Contains( "Pred" ) && prefix.Contains( "AR" );

    // Generally the format is something like "ttX__" or "_trueSS_"
    TString procprefix = sample_category + "_" + bkg_category;

    // The fake prediction will be applied to the appropriate region.
    if ( wwwbaby.isData() && prefix.Contains( "AR" ) && prefix.Contains( "Pred" ) )
    {
        if ( prefix.Contains( "SSAR" ) ) prefix.ReplaceAll( "AR", "" );
        if ( prefix.Contains( "ARSS" ) ) prefix.ReplaceAll( "AR", "VR" );
    }

    // The counter plots are split by lepton flavors
    std::vector<TString> binlabels = { "ee", "e#mu", "#mu#mu" };
    double wgt = weight( ff, isyst );

    hists.fill( regionid     , isyst, Form( "%s_SS_counter"              , procprefix.Data() ), wgt, 3, 0, 3, NSYST, 0, NSYST, binlabels );
    hists.fill( regionid     , isyst, Form( "%s_SS_rawcounter"           , procprefix.Data() ), 1  , 3, 0, 3, NSYST, 0, NSYST, binlabels );
    hists.fill( regionid - 3 , isyst, Form( "%s_3L_counter"              , procprefix.Data() ), wgt, 3, 0, 3, NSYST, 0, NSYST, binlabels );
    hists.fill( regionid - 3 , isyst, Form( "%s_3L_rawcounter"           , procprefix.Data() ), 1  , 3, 0, 3, NSYST, 0, NSYST, binlabels );
    hists.fill( regionid - 6 , isyst, Form( "%s_BTagVRSS_counter"        , procprefix.Data() ), wgt, 3, 0, 3, NSYST, 0, NSYST, binlabels );
    hists.fill( regionid - 6 , isyst, Form( "%s_BTagVRSS_rawcounter"     , procprefix.Data() ), 1  , 3, 0, 3, NSYST, 0, NSYST, binlabels );
    hists.fill( regionid - 9 , isyst, Form( "%s_BTagARSS_counter"        , procprefix.Data() ), wgt, 3, 0, 3, NSYST, 0, NSYST, binlabels );
    hists.fill( regionid - 9 , isyst, Form( "%s_BTagARSS_rawcounter"     , procprefix.Data() ), 1  , 3, 0, 3, NSYST, 0, NSYST, binlabels );
    hists.fill( regionid - 12, isyst, Form( "%s_ARSS_counter"            , procprefix.Data() ), wgt, 3, 0, 3, NSYST, 0, NSYST, binlabels );
    hists.fill( regionid - 12, isyst, Form( "%s_ARSS_rawcounter"         , procprefix.Data() ), 1  , 3, 0, 3, NSYST, 0, NSYST, binlabels );
    hists.fill( regionid - 15, isyst, Form( "%s_MjjSBVRSS_counter"       , procprefix.Data() ), wgt, 3, 0, 3, NSYST, 0, NSYST, binlabels );
    hists.fill( regionid - 15, isyst, Form( "%s_MjjSBVRSS_rawcounter"    , procprefix.Data() ), 1  , 3, 0, 3, NSYST, 0, NSYST, binlabels );
    hists.fill( regionid - 18, isyst, Form( "%s_MjjSBARSS_counter"       , procprefix.Data() ), wgt, 3, 0, 3, NSYST, 0, NSYST, binlabels );
    hists.fill( regionid - 18, isyst, Form( "%s_MjjSBARSS_rawcounter"    , procprefix.Data() ), 1  , 3, 0, 3, NSYST, 0, NSYST, binlabels );

    hists.fill( regionid - 21, isyst, Form( "%s_SSPred_counter"          , procprefix.Data() ), wgt, 3, 0, 3, NSYST, 0, NSYST, binlabels );
    hists.fill( regionid - 21, isyst, Form( "%s_SSPred_rawcounter"       , procprefix.Data() ), 1  , 3, 0, 3, NSYST, 0, NSYST, binlabels );
    hists.fill( regionid - 24, isyst, Form( "%s_BTagVRSSPred_counter"    , procprefix.Data() ), wgt, 3, 0, 3, NSYST, 0, NSYST, binlabels );
    hists.fill( regionid - 24, isyst, Form( "%s_BTagVRSSPred_rawcounter" , procprefix.Data() ), 1  , 3, 0, 3, NSYST, 0, NSYST, binlabels );
    hists.fill( regionid - 27, isyst, Form( "%s_MjjSBVRSSPred_counter"   , procprefix.Data() ), wgt, 3, 0, 3, NSYST, 0, NSYST, binlabels );
    hists.fill( regionid - 27, isyst, Form( "%s_MjjSBVRSSPred_rawcounter", procprefix.Data() ), 1  , 3, 0, 3, NSYST, 0, NSYST, binlabels );

    hists.fill( regionid - 30, isyst, Form( "%s_MjjSBPRVRSS_counter"       , procprefix.Data() ), wgt, 3, 0, 3, NSYST, 0, NSYST, binlabels );
    hists.fill( regionid - 30, isyst, Form( "%s_MjjSBPRVRSS_rawcounter"    , procprefix.Data() ), 1  , 3, 0, 3, NSYST, 0, NSYST, binlabels );
    hists.fill( regionid - 33, isyst, Form( "%s_MjjSBPRARSS_counter"       , procprefix.Data() ), wgt, 3, 0, 3, NSYST, 0, NSYST, binlabels );
    hists.fill( regionid - 33, isyst, Form( "%s_MjjSBPRARSS_rawcounter"    , procprefix.Data() ), 1  , 3, 0, 3, NSYST, 0, NSYST, binlabels );
    hists.fill( regionid - 36, isyst, Form( "%s_MjjSBPRVRSSPred_counter"   , procprefix.Data() ), wgt, 3, 0, 3, NSYST, 0, NSYST, binlabels );
    hists.fill( regionid - 36, isyst, Form( "%s_MjjSBPRVRSSPred_rawcounter", procprefix.Data() ), 1  , 3, 0, 3, NSYST, 0, NSYST, binlabels );


    // Check whether the beginning or the ending of the procprefix has "_"
    if ( !procprefix.BeginsWith( "_" ) && !procprefix.EndsWith( "_" ) )
        return;

    TString fullprefix = sample_category + "_" + bkg_category + "_" + prefix + "_";
    //fillLepHistograms( hists, "SignalLepton" , ""      , fullprefix );
    //fillLepHistograms( hists, "3LTightLepton", ""      , fullprefix );
    //fillLepHistograms( hists, "TightLepton"  , "tight" , fullprefix );
    fillLepHistograms( hists, "LooseLepton"  , "loose" , fullprefix );
    //fillLepHistograms( hists, "LbntLepton"   , "lbnt"  , fullprefix );
    //fillLepHistograms( hists, "Lbn3tLepton"  , "lbn3t" , fullprefix );
    fillJetHistograms( hists, "GoodSSJet"    , ""      , fullprefix );
    fillJetHistograms( hists, "LooseBJet"    , "b"     , fullprefix );
    fillJetHistograms( hists, "Good3LJet"    , "3l"    , fullprefix );
    fillJetHistograms( hists, "GoodSSWJet"   , "wtag"  , fullprefix );
    fillWWWHistograms( hists, fullprefix );

}

//_________________________________________________________________________________________________
void fillLepHistograms( TasUtil::AutoHist& hists, TString categ, TString name, TString prefix )
{
    bool ff = prefix.Contains( "Pred" );
    hists.fill( lepidx[categ].size() , Form( "%slep%s_size" , prefix.Data(), name.Data() ), weight( ff ), 5, 0, 5 );
    for ( unsigned int i = 0; i < lepidx[categ].size() && i < MAXOBJ; ++i )
    {
        int ilep = lepidx[categ][i];
        hists.fill( wwwbaby.lep_pdgId()[ilep]      , Form( "%slep%s%d_pid"       , prefix.Data(), name.Data(), i ), weight( ff ),   40,  -20     ,  20      );
        hists.fill( wwwbaby.lep_p4()[ilep].pt()    , Form( "%slep%s%d_pt"        , prefix.Data(), name.Data(), i ), weight( ff ), 1080,    0     , 250.     );
        hists.fill( wwwbaby.lep_p4()[ilep].eta()   , Form( "%slep%s%d_eta"       , prefix.Data(), name.Data(), i ), weight( ff ), 1080,   -3     ,   3      );
        hists.fill( wwwbaby.lep_p4()[ilep].phi()   , Form( "%slep%s%d_phi"       , prefix.Data(), name.Data(), i ), weight( ff ), 1080,   -3.1416,   3.1416 );
        hists.fill( wwwbaby.lep_p4()[ilep].energy(), Form( "%slep%s%d_E"         , prefix.Data(), name.Data(), i ), weight( ff ), 1080,    0     , 250.     );
        hists.fill( wwwbaby.lep_relIso03EA()[ilep] , Form( "%slep%s%d_iso"       , prefix.Data(), name.Data(), i ), weight( ff ), 1080,    0     ,   0.1    );
        hists.fill( wwwbaby.lep_ip3d()[ilep]       , Form( "%slep%s%d_ip3"       , prefix.Data(), name.Data(), i ), weight( ff ), 1080,   -0.05  ,   0.05   );
        hists.fill( wwwbaby.lep_ip3derr()[ilep]    , Form( "%slep%s%d_ip3err"    , prefix.Data(), name.Data(), i ), weight( ff ), 1080,   -0.5   ,   0.5    );
        hists.fill( wwwbaby.lep_ip3d()[ilep]       , Form( "%slep%s%d_ip3_wide"  , prefix.Data(), name.Data(), i ), weight( ff ), 1080,   -0.5   ,   0.5    );
        hists.fill( wwwbaby.lep_ip3d()[ilep]       , Form( "%slep%s%d_ip3_widepp", prefix.Data(), name.Data(), i ), weight( ff ), 1080,   -2.5   ,   2.5    );
        hists.fill( wwwbaby.lep_ip3d()[ilep]       , Form( "%slep%s%d_ip3calc"   , prefix.Data(), name.Data(), i ), weight( ff ), 1080,   -0.05  ,   0.05   );
        hists.fill( wwwbaby.lep_dxy ()[ilep]       , Form( "%slep%s%d_dxy"       , prefix.Data(), name.Data(), i ), weight( ff ), 1080,   -0.5   ,   0.5    );
        hists.fill( wwwbaby.lep_dz  ()[ilep]       , Form( "%slep%s%d_dz"        , prefix.Data(), name.Data(), i ), weight( ff ), 1080,   -0.5   ,   0.5    );
        if ( wwwbaby.lep_ip3d()[ilep] > 0.5 )
            std::cout << wwwbaby.lep_ip3d()[ilep] << std::endl;
    }
}

//_________________________________________________________________________________________________
void fillJetHistograms( TasUtil::AutoHist& hists, TString categ, TString name, TString prefix )
{
    bool ff = prefix.Contains( "Pred" );
    hists.fill( jetidx[categ].size() , Form( "%sjet%s_size" , prefix.Data(), name.Data() ), weight( ff ), 5, 0, 5 );
    for ( unsigned int i = 0; i < jetidx[categ].size() && i < MAXOBJ; ++i )
    {
        int ijet = jetidx[categ][i];
        hists.fill( wwwbaby.jets_p4()[ijet].pt()    , Form( "%sjet%s%d_pt" , prefix.Data(), name.Data(), i ), weight( ff ), 180,  0     , 180      );
        hists.fill( wwwbaby.jets_p4()[ijet].eta()   , Form( "%sjet%s%d_eta", prefix.Data(), name.Data(), i ), weight( ff ), 180, -3     ,   3      );
        hists.fill( wwwbaby.jets_p4()[ijet].phi()   , Form( "%sjet%s%d_phi", prefix.Data(), name.Data(), i ), weight( ff ), 180, -3.1416,   3.1416 );
        hists.fill( wwwbaby.jets_p4()[ijet].energy(), Form( "%sjet%s%d_E"  , prefix.Data(), name.Data(), i ), weight( ff ), 180,  0     , 250      );
        hists.fill( wwwbaby.jets_csv()[ijet]        , Form( "%sjet%s%d_csv", prefix.Data(), name.Data(), i ), weight( ff ), 180, -1     ,   1      );
    }
}

//_________________________________________________________________________________________________
void fillWWWHistograms( TasUtil::AutoHist& hists, TString prefix )
{
    bool ff = prefix.Contains( "Pred" );
    hists.fill( wwwbaby.met_pt()                                  , Form( "%smet"        , prefix.Data() ) , weight( ff ) , 180 , 0. , 250.   );
    hists.fill( MjjW()                                            , Form( "%sMjjW"       , prefix.Data() ) , weight( ff ) , 180 , 0. , 160.   );
    hists.fill( MjjLead()                                         , Form( "%sMjjLead"    , prefix.Data() ) , weight( ff ) , 180 , 0. , 800.   );
    hists.fill( DEtajjLead()                                      , Form( "%sDEtajjLead" , prefix.Data() ) , weight( ff ) , 180 , 0. , 9.     );
    hists.fill( DPhill()                                          , Form( "%sDPhill"     , prefix.Data() ) , weight( ff ) , 180 , 0. , 3.1416 );
    hists.fill( DEtall()                                          , Form( "%sDEtall"     , prefix.Data() ) , weight( ff ) , 180 , 0. , 9.     );
    hists.fill( Mll()                                             , Form( "%sMll"        , prefix.Data() ) , weight( ff ) , 180 , 0. , 180.   );
    hists.fill( Mll()                                             , Form( "%sMll250"     , prefix.Data() ) , weight( ff ) , 180 , 0. , 250.   );
    hists.fill( Mll()                                             , Form( "%sMll500"     , prefix.Data() ) , weight( ff ) , 180 , 0. , 500.   );
    hists.fill( MTmax()                                           , Form( "%sMTmax"      , prefix.Data() ) , weight( ff ) , 180 , 0. , 180.   );
    hists.fill( M4()                                              , Form( "%sm4"         , prefix.Data() ) , weight( ff ) , 180 , 0. , 180.   );
    hists.fill( M4()                                              , Form( "%sm4wide"     , prefix.Data() ) , weight( ff ) , 150 , 0. , 1500.  );
    hists.fill( wwwbaby.nisoTrack_mt2_cleaned_VVV_cutbased_veto() , Form( "%snisotrack"  , prefix.Data() ) , weight( ff ) , 5   , 0  , 5      );
    hists.fill( wwwbaby.nlep_VVV_cutbased_veto()                  , Form( "%snvetolep"   , prefix.Data() ) , weight( ff ) , 5   , 0  , 5      );
    hists.fill( wwwbaby.nVert()                                   , Form( "%snvtx"       , prefix.Data() ) , weight( ff ) , 70  , 0  , 70.    );
    if ( lepidx["3LTightLepton"].size() == 3 )
    {
        hists.fill( Pt3l()           , Form( "%sPt3l"         , prefix.Data() ) , weight( ff ) , 180 , 0. , 180.   );
        hists.fill( DPhi3lMET()      , Form( "%sDPhi3lMET"    , prefix.Data() ) , weight( ff ) , 180 , 0. , 3.1416 );
        if ( pass3L0SFOS() )
        {
            hists.fill( get0SFOSMll()    , Form( "%sget0SFOSMll"  , prefix.Data() ) , weight( ff ) , 180 , 0. , 180.   );
            hists.fill( get0SFOSMee()    , Form( "%sget0SFOSMee"  , prefix.Data() ) , weight( ff ) , 180 , 0. , 180.   );
        }
        if ( pass3L1SFOS() )
        {
            hists.fill( get1SFOSMll()    , Form( "%sget1SFOSMll"  , prefix.Data() ) , weight( ff ) , 180 , 0. , 180.   );
        }
        if ( pass3L2SFOS() )
        {
            hists.fill( get2SFOSMll0()   , Form( "%sget2SFOSMll0" , prefix.Data() ) , weight( ff ) , 180 , 0. , 180.   );
            hists.fill( get2SFOSMll1()   , Form( "%sget2SFOSMll1" , prefix.Data() ) , weight( ff ) , 180 , 0. , 180.   );
        }
    }
}

//_________________________________________________________________________________________________
void printevent( TString region )
{
    std::cout << std::endl;
    std::cout << "passed event list ";
    std::cout << region.Data() << " ";
    std::cout << wwwbaby.evt() << " ";
    std::cout << wwwbaby.run() << " ";
    std::cout << wwwbaby.lumi() << " ";
    std::cout << std::endl;
}

////_________________________________________________________________________________________________
//void doTmpAnalysis( TasUtil::AutoHist& hists )
//{
//    lepidx["SignalLepton"] = lepidx["LooseLepton"];
//    if (!( lepidx["TightLepton"].size()                     ==   1   )) return;
//    if (!( lepidx["LooseLepton"].size()                     ==   2   )) return;
//    if (!( wwwbaby.lep_p4()[lepidx["SignalLepton"][0]].pt()  >   30. )) return;
//    if (!( wwwbaby.lep_p4()[lepidx["SignalLepton"][1]].pt()  >   30. )) return;
//    if (!( isSS()                                                    )) return;
//    if ( isSSMM() && Mll() > 40. )
//    {
//        fillHistograms( hists, "SSMM_CutSSMMLep", 0 );
//        if ( jetidx["GoodSSJet"].size() == 0 )
//            fillHistograms( hists, "SSMM_CutNjet", 0 );
//    }
//    if ( isSSEM() && Mll() > 40. && abs(wwwbaby.lep_pdgId()[lepidx["LbntLepton"][0]]) == 13 )
//        fillHistograms( hists, "SSEM_CutSSEMLep", 1 );
//    if ( isSSEE() && Mll() > 40. )
//        fillHistograms( hists, "SSEE_CutSSEELep", 2 );
//}

// eof
