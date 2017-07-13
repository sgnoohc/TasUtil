//  .
// ..: P. Chang, philip@physics.ucsd.edu

#include "ScanChain_FlatBabyMaker.h"

//_________________________________________________________________________________________________
void ScanChain(TChain* chain, TString output_name, int nevents)
{

    // -~-~-~-~-
    // Benchmark
    // -~-~-~-~-
    // To time how long it takes to run this code
    TBenchmark *bmark = new TBenchmark();
    bmark->Start("benchmark");

    // -~-~-~-~-~-~-~-~-~-~-~-~-~-~-
    // CORE Helper for 2016 Analysis
    // -~-~-~-~-~-~-~-~-~-~-~-~-~-~-
    // This sets up JEC/Elec MVA/Good Runs List and all the goodies.
    COREHelper2016 core;

    TString option = "";
    option += "applyJEC";

    if ( output_name.Contains( "Summer16" ) )
        option += "Summer16";

    core.initializeCORE( option );

    // -~-~-~-~-~-
    // Output baby
    // -~-~-~-~-~-

    TFile* ofile = new TFile(output_name, "recreate");
    ofile->cd();
    TTreeX* ttree = new TTreeX("t", "A Baby Ntuple");

    ttree->createBranch<Int_t   >( "run" );
    ttree->createBranch<Int_t   >( "lumi" );
    ttree->createBranch<Int_t   >( "evt" );
    ttree->createBranch<Int_t   >( "isData" );
    ttree->createBranch<Int_t   >( "evt_passgoodrunlist" );
    ttree->createBranch<Float_t >( "evt_scale1fb" );
    ttree->createBranch<Float_t >( "evt_xsec" );
    ttree->createBranch<Float_t >( "evt_kfactor" );
    ttree->createBranch<Float_t >( "evt_filter" );
    ttree->createBranch<Int_t   >( "evt_nEvts" );
    ttree->createBranch<Float_t >( "puWeight" );
    ttree->createBranch<Int_t   >( "nVert" );
    ttree->createBranch<Int_t   >( "nTrueInt" );
    ttree->createBranch<Float_t >( "rho" );
    ttree->createBranch<Float_t >( "rho25" );


    // -~-~-~-~-~
    // Event Loop
    // -~-~-~-~-~
    Looper<CMS3> looper(chain, &cms3, nevents);
    while (looper.nextEvent())
    {

    }


    // -~-~-~
    // Output
    // -~-~-~
    ofile->cd();
    ttree->Write();
    ofile->Close();

    // -~-~-~-~-
    // Benchmark
    // -~-~-~-~-
    bmark->Stop("benchmark");
    cout << endl;
    cout << nevents << " Events Processed" << endl;
    cout << "------------------------------" << endl;
    cout << "CPU  Time:	" << Form( "%.01f s", bmark->GetCpuTime("benchmark")  ) << endl;
    cout << "Real Time:	" << Form( "%.01f s", bmark->GetRealTime("benchmark") ) << endl;
    cout << endl;

}

// eof
