//  .
// ..: P. Chang, philip@physics.ucsd.edu

#include "WWWSkimmer_ScanChain.h"

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
    looper.setSkim( output_name );
//    std::vector<TString> filtpttn;
//    filtpttn.push_back( "lep_" );
//    filtpttn.push_back( "jets_" );
//    filtpttn.push_back( "met_pt" );
//    filtpttn.push_back( "met_phi" );
//    filtpttn.push_back( "run" );
//    filtpttn.push_back( "evt" );
//    filtpttn.push_back( "lumi" );
//    filtpttn.push_back( "isData" );
//    filtpttn.push_back( "nisoTrack" );
//    filtpttn.push_back( "nVert" );
//    looper.setSkimBranchFilterPattern( filtpttn );
    looper.setSkimMaxSize( 400*1024*1024 );
//    TTree* skimtree = 0;
//    Int_t tightlepidx0 = 0;
//    Int_t tightlepidx1 = 0;
//    Int_t tightlepidx2 = 0;
//    Int_t looselepidx0 = 0;
//    Int_t looselepidx1 = 0;
//    Int_t looselepidx2 = 0;
//    Int_t lbntlepidx0 = 0;
//    Int_t lbntlepidx1 = 0;
//    Int_t lbntlepidx2 = 0;
//    Int_t goodssjetidx0 = 0;
//    Int_t goodssjetidx1 = 0;
//    Int_t goodsswjetidx0 = 0;
//    Int_t goodsswjetidx1 = 0;
//    Int_t good3ljetidx0 = 0;
//    Int_t good3ljetidx1 = 0;
    while ( looper.nextEvent() )
    {
//        if ( !skimtree )
//        {
//            skimtree = looper.getSkimTree();
//            if ( skimtree )
//            {
//                skimtree->Branch( "tightlepidx0", &tightlepidx0, "tightlepidx0/I" );
//                skimtree->Branch( "tightlepidx1", &tightlepidx1, "tightlepidx1/I" );
//                skimtree->Branch( "tightlepidx2", &tightlepidx2, "tightlepidx2/I" );
//                skimtree->Branch( "looselepidx0", &looselepidx0, "looselepidx0/I" );
//                skimtree->Branch( "looselepidx1", &looselepidx1, "looselepidx1/I" );
//                skimtree->Branch( "looselepidx2", &looselepidx2, "looselepidx2/I" );
//                skimtree->Branch( "lbntlepidx0", &lbntlepidx0, "lbntlepidx0/I" );
//                skimtree->Branch( "lbntlepidx1", &lbntlepidx1, "lbntlepidx1/I" );
//                skimtree->Branch( "lbntlepidx2", &lbntlepidx2, "lbntlepidx2/I" );
//                skimtree->Branch( "goodssjetidx0", &goodssjetidx0, "goodssjetidx0/I" );
//                skimtree->Branch( "goodssjetidx1", &goodssjetidx1, "goodssjetidx1/I" );
//                skimtree->Branch( "goodsswjetidx0", &goodsswjetidx0, "goodsswjetidx0/I" );
//                skimtree->Branch( "goodsswjetidx1", &goodsswjetidx1, "goodsswjetidx1/I" );
//                skimtree->Branch( "good3ljetidx0", &good3ljetidx0, "good3ljetidx0/I" );
//                skimtree->Branch( "good3ljetidx1", &good3ljetidx1, "good3ljetidx1/I" );
//            }
//        }
//        std::cout << wwwbaby.run() << std::endl;
//        std::cout << wwwbaby.lumi() << std::endl;
//        std::cout << wwwbaby.evt() << std::endl;
        if ( wwwbaby.isData() )
        {
            duplicate_removal::DorkyEventIdentifier id( wwwbaby.run(), wwwbaby.evt(), wwwbaby.lumi() );
            if ( is_duplicate( id ) )
                continue;
        }
        setObjectIndices();
        if ( passSkim() )
        {
//            tightlepidx0   = lepidx["TightLepton" ] [0];
//            tightlepidx1   = lepidx["TightLepton" ] [1];
//            tightlepidx2   = lepidx["TightLepton" ] .size() > 2 ? lepidx["TightLepton" ] [2 ] : -1;
//            looselepidx0   = lepidx["LooseLepton" ] [0];
//            looselepidx1   = lepidx["LooseLepton" ] [1];
//            looselepidx2   = lepidx["LooseLepton" ] .size() > 2 ? lepidx["LooseLepton" ] [2 ] : -1;
//            lbntlepidx0    = lepidx["LbntLepton"  ] .size() > 0 ? lepidx["LbntLepton"  ] [0 ] : -1;
//            lbntlepidx1    = lepidx["LbntLepton"  ] .size() > 1 ? lepidx["LbntLepton"  ] [1 ] : -1;
//            lbntlepidx2    = lepidx["LbntLepton"  ] .size() > 2 ? lepidx["LbntLepton"  ] [2 ] : -1;
//            goodssjetidx0  = jetidx["GoodSSJet"   ] .size() > 0 ? jetidx["GoodSSJet"   ] [0 ] : -1;
//            goodssjetidx1  = jetidx["GoodSSJet"   ] .size() > 1 ? jetidx["GoodSSJet"   ] [1 ] : -1;
//            goodsswjetidx0 = jetidx["GoodSSWJet"  ] .size() > 0 ? jetidx["GoodSSWJet"  ] [0 ] : -1;
//            goodsswjetidx1 = jetidx["GoodSSWJet"  ] .size() > 1 ? jetidx["GoodSSWJet"  ] [1 ] : -1;
//            good3ljetidx0  = jetidx["Good3LJet"   ] .size() > 0 ? jetidx["Good3LJet"   ] [0 ] : -1;
//            good3ljetidx1  = jetidx["Good3LJet"   ] .size() > 1 ? jetidx["Good3LJet"   ] [1 ] : -1;
//            if ( wwwbaby.run() == 1 && wwwbaby.lumi() == 319056 && wwwbaby.evt() == 60620636 )
//            {
//                std::cout << "here" << std::endl;
//            }
            looper.fillSkim();
        }
    }
    looper.saveSkim();
}

//_________________________________________________________________________________________________
bool passSkim()
{
    if (
        // Same sign two lepton preselection
        (
         ( lepidx["TightLepton"].size() == 2 ) &&
         ( ( wwwbaby.lep_pdgId()[lepidx["TightLepton"][0]] * wwwbaby.lep_pdgId()[lepidx["TightLepton"][1]] ) > 0 ) &&
         ( wwwbaby.lep_p4()[lepidx["TightLepton"][0]].pt() >= 25.)  &&
         ( wwwbaby.lep_p4()[lepidx["TightLepton"][1]].pt() >= 20.) )
        ||
        // Three lepton preselection
        (
         ( lepidx["TightLepton"].size() == 3                      )  &&
         ( wwwbaby.lep_p4()[lepidx["TightLepton"][0]].pt() >= 25. )  &&
         ( wwwbaby.lep_p4()[lepidx["TightLepton"][1]].pt() >= 20. )  &&
         ( wwwbaby.lep_p4()[lepidx["TightLepton"][2]].pt() >= 20. ) )
        ||
        // One loose but not tight + one tight preselection
        (
         ( lepidx["TightLepton"].size() == 1 ) &&
         ( lepidx["LooseLepton"].size() == 2 ) &&
         ( ( wwwbaby.lep_pdgId()[lepidx["LooseLepton"][0]] * wwwbaby.lep_pdgId()[lepidx["LooseLepton"][1]] ) > 0 ) &&
         ( wwwbaby.lep_p4()[lepidx["LooseLepton"][0]].pt() >= 25.)  &&
         ( wwwbaby.lep_p4()[lepidx["LooseLepton"][1]].pt() >= 20.) )
        ||
        // One loose but not tight + two tight preselection
        (
         ( lepidx["TightLepton"].size() == 2 ) &&
         ( lepidx["LooseLepton"].size() == 3 ) &&
         ( wwwbaby.lep_p4()[lepidx["LooseLepton"][0]].pt() >= 25.)  &&
         ( wwwbaby.lep_p4()[lepidx["LooseLepton"][1]].pt() >= 20.)  &&
         ( wwwbaby.lep_p4()[lepidx["LooseLepton"][2]].pt() >= 20.) )
       )
        return true;
    else
        return false;

}

//eof
