//  .
// ..: P. Chang, philip@physics.ucsd.edu

#include "ScanChain_FlatBabyMaker.h"

//_________________________________________________________________________________________________
void ScanChain( TChain* chain, TString output_name, TString base_optstr, int nevents )
{

    // -~-~-~-~-~-~
    // Event Looper
    // -~-~-~-~-~-~
    Looper<CMS3> looper( chain, &cms3, nevents );
    chain->GetEntry( 0 );
    cms3.Init( chain->GetTree() );

    // -~-~-~-~-~-~-~-~-~-~-~-~-~-~-
    // CORE Helper for 2016 Analysis
    // -~-~-~-~-~-~-~-~-~-~-~-~-~-~-
    // This sets up JEC/Elec MVA/Good Runs List and all the goodies.
    CORE2016 core;

    TString option = base_optstr;
    option += "applyJEC";

    core.initializeCORE( option );

    // -~-~-~-~-~-
    // Output baby
    // -~-~-~-~-~-

    TFile* ofile = new TFile( output_name, "recreate" );
    ofile->cd();
    TTreeX* ttree = new TTreeX( "t", "A Baby Ntuple" );

    core.createEventBranches( ttree );
//    core.createGenBranches( ttree );
    core.createJetBranches( ttree );
    core.createFatJetBranches( ttree );
    core.createMETBranches( ttree );
    core.createLeptonBranches( ttree,
            {
                {VVV_cutbased_tight    , "VVV_cutbased_tight"   },
                {VVV_cutbased_fo       , "VVV_cutbased_fo"      },
                {VVV_cutbased_fo_noiso , "VVV_cutbased_fo_noiso"},
                {VVV_cutbased_veto     , "VVV_cutbased_veto"    }
            }
            );
    core.createTrigBranches( ttree,
            {
                "HLT_Ele",
                "HLT_Mu",
                "HLT_TkMu",
                "HLT_IsoMu",
                "HLT_IsoTkMu",
            }
            );

    while ( looper.nextEvent() )
    {

        ttree->clear();

        core.setJetCorrector();

        core.setEventBranches( ttree );
//        core.setGenBranches( ttree );
        core.setJetBranches( ttree );
        core.setFatJetBranches( ttree );
        core.setMETBranches( ttree );
        core.setLeptonBranches( ttree );
        core.setTrigBranches( ttree );

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
