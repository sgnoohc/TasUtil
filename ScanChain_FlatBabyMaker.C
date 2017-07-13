//  .
// ..: P. Chang, philip@physics.ucsd.edu

#include "ScanChain_FlatBabyMaker.h"

//_________________________________________________________________________________________________
void ScanChain( TChain* chain, TString output_name, int nevents )
{
    
    // -~-~-~-~-~-~-~-~-~-~-~-~-~-~-
    // CORE Helper for 2016 Analysis
    // -~-~-~-~-~-~-~-~-~-~-~-~-~-~-
    // This sets up JEC/Elec MVA/Good Runs List and all the goodies.
    CORE2016 core;
    
    TString option = "";
    option += "applyJEC";
    
    if ( output_name.Contains( "Summer16" ) )
        option += "Summer16";
        
    core.initializeCORE( option );
    
    // -~-~-~-~-~-
    // Output baby
    // -~-~-~-~-~-
    
    TFile* ofile = new TFile( output_name, "recreate" );
    ofile->cd();
    TTreeX* ttree = new TTreeX( "t", "A Baby Ntuple" );
    
    core.createEventBranches( ttree );
    core.createMETBranches( ttree );
    
    // -~-~-~-~-~
    // Event Loop
    // -~-~-~-~-~
    Looper<CMS3> looper( chain, &cms3, nevents );
    
    while ( looper.nextEvent() )
    {
    
        ttree->clear();
        core.setEventBranches( ttree );
        core.setMETBranches( ttree );
        ttree->Fill();
        
    }
    
    // -~-~-~
    // Output
    // -~-~-~
    ofile->cd();
    ttree->Write();
    ofile->Close();
    
}

// eof
