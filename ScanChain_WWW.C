//  .
// ..: P. Chang, philip@physics.ucsd.edu

#include "ScanChain_WWW.h"

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
        duplicate_removal::DorkyEventIdentifier id(wwwbaby.run(), wwwbaby.evt(), wwwbaby.lumi());

        if (is_duplicate(id))
        {
            TasUtil::warning(
                    Form( "duplicate events found run=%d, evt=%d, lumi=%d",
                        wwwbaby.run(), wwwbaby.evt(), wwwbaby.lumi() ));
            continue;
        }

        setObjectIndices();
        doSS( hists );
        doSSAR( hists );
    }

    hists.save( output_name );
}

//=================================================================================================
//=================================================================================================
//=================================================================================================

//_________________________________________________________________________________________________
void doSS( TasUtil::AutoHist& hists )
{
    if ( passSSMM() ) fillHistograms( hists, "SSmm_", 0 );
    if ( passSSEM() ) fillHistograms( hists, "SSem_", 1 );
    if ( passSSEE() ) fillHistograms( hists, "SSee_", 2 );
    if ( passSSMM( 0 ) && passSSpresel() && fabs( MjjW() - 80. ) > 20. && wwwbaby.met_pt() < 40. ) fillHistograms( hists, "SSmm_LowMET_MjjSideBand_", 6 );
    if ( passSSEM() ) fillHistograms( hists, "SSem_", 1 );
    if ( passSSEE() ) fillHistograms( hists, "SSee_", 2 );
}

//_________________________________________________________________________________________________
void doSSAR( TasUtil::AutoHist& hists )
{
    if ( passSSARMM() ) fillHistograms( hists, "SSARmm_", 3 );
    if ( passSSAREM() ) fillHistograms( hists, "SSARem_", 4 );
    if ( passSSAREE() ) fillHistograms( hists, "SSARee_", 5 );
}

//_________________________________________________________________________________________________
void fillHistograms( TasUtil::AutoHist& hists, TString prefix, int regionid )
{
    hists.fill( regionid, Form( "%s_Region_counter"   , sampleCategory().Data() ), weight(), 7, 0, 7 );
    hists.fill( regionid, Form( "%s_Region_rawcounter", sampleCategory().Data() ),        1, 7, 0, 7 );
    fillLepHistograms( hists, "SignalLepton", ""     , prefix );
    fillLepHistograms( hists, "TightLepton" , "tight", prefix );
    fillLepHistograms( hists, "LooseLepton" , "loose", prefix );
    fillLepHistograms( hists, "LbntLepton"  , "lbnt" , prefix );
    fillJetHistograms( hists, "GoodSSJet"   , ""     , prefix );
    fillJetHistograms( hists, "GoodSSWJet"  , "wtag" , prefix );
    fillWWWHistograms( hists, prefix );
}

//_________________________________________________________________________________________________
void fillLepHistograms( TasUtil::AutoHist& hists, TString categ, TString name, TString prefix )
{
    for ( unsigned int i = 0; i < lepidx[categ].size(); ++i )
    {
        int ilep = lepidx[categ][i];
        hists.fill( wwwbaby.lep_p4()[ilep].pt()    , Form( "%s_%slep%s%d_pt" , sampleCategory().Data(), prefix.Data(), name.Data(), i ), weight(), 180,  0     , 180      );
        hists.fill( wwwbaby.lep_p4()[ilep].eta()   , Form( "%s_%slep%s%d_eta", sampleCategory().Data(), prefix.Data(), name.Data(), i ), weight(), 180, -3     ,   3      );
        hists.fill( wwwbaby.lep_p4()[ilep].phi()   , Form( "%s_%slep%s%d_phi", sampleCategory().Data(), prefix.Data(), name.Data(), i ), weight(), 180, -3.1416,   3.1416 );
        hists.fill( wwwbaby.lep_p4()[ilep].energy(), Form( "%s_%slep%s%d_E"  , sampleCategory().Data(), prefix.Data(), name.Data(), i ), weight(), 180,  0     , 250      );
    }
}

//_________________________________________________________________________________________________
void fillJetHistograms( TasUtil::AutoHist& hists, TString categ, TString name, TString prefix )
{
    for ( unsigned int i = 0; i < jetidx[categ].size(); ++i )
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
    hists.fill( wwwbaby.met_pt() , Form( "%s_%smet"          , sampleCategory().Data() , prefix.Data() ) , weight() , 180 , 0. , 250.   );
    hists.fill( MjjW()           , Form( "%s_%sMjjW"         , sampleCategory().Data() , prefix.Data() ) , weight() , 180 , 0. , 160.   );
    hists.fill( MjjLead()        , Form( "%s_%sMjjLead"      , sampleCategory().Data() , prefix.Data() ) , weight() , 180 , 0. , 800.   );
    hists.fill( DEtajjLead()     , Form( "%s_%sDEtajjLead"   , sampleCategory().Data() , prefix.Data() ) , weight() , 180 , 0. , 9.     );
    hists.fill( MllSS()          , Form( "%s_%sMllSS"        , sampleCategory().Data() , prefix.Data() ) , weight() , 180 , 0. , 180.   );
    hists.fill( Pt3l()           , Form( "%s_%sPt3l"         , sampleCategory().Data() , prefix.Data() ) , weight() , 180 , 0. , 180.   );
    hists.fill( DPhi3lMET()      , Form( "%s_%sDPhi3lMET"    , sampleCategory().Data() , prefix.Data() ) , weight() , 180 , 0. , 3.1416 );
    hists.fill( getMTmax()       , Form( "%s_%sgetMTmax"     , sampleCategory().Data() , prefix.Data() ) , weight() , 180 , 0. , 180.   );
    hists.fill( get0SFOSMll()    , Form( "%s_%sget0SFOSMll"  , sampleCategory().Data() , prefix.Data() ) , weight() , 180 , 0. , 180.   );
    hists.fill( get0SFOSMee()    , Form( "%s_%sget0SFOSMee"  , sampleCategory().Data() , prefix.Data() ) , weight() , 180 , 0. , 180.   );
    hists.fill( get1SFOSMll()    , Form( "%s_%sget1SFOSMll"  , sampleCategory().Data() , prefix.Data() ) , weight() , 180 , 0. , 180.   );
    hists.fill( get2SFOSMll0()   , Form( "%s_%sget2SFOSMll0" , sampleCategory().Data() , prefix.Data() ) , weight() , 180 , 0. , 180.   );
    hists.fill( get2SFOSMll1()   , Form( "%s_%sget2SFOSMll1" , sampleCategory().Data() , prefix.Data() ) , weight() , 180 , 0. , 180.   );
}

// eof
