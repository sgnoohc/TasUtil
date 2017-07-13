//  .
// ..: P. Chang, philip@physics.ucsd.edu

#include "ScanChain_FlatBabyMaker.h"

//_________________________________________________________________________________________________
void ScanChain(TChain* chain, TString output_name, int nevents)
{
    // -~-~-~-~-~-~-~-~-~-~-~
    // Configuration switches
    // -~-~-~-~-~-~-~-~-~-~-~
    bool applyJECfromFile = true;
    bool isFastSim = false;

    // -~-~-~-~-
    // Benchmark
    // -~-~-~-~-
    // To time how long it takes to run this code
    TBenchmark *bmark = new TBenchmark();
    bmark->Start("benchmark");

    // -~-~-~-~-~-
    // Output baby
    // -~-~-~-~-~-

    TFile* ofile = new TFile(output_name, "recreate");
    ofile->cd();
    TasUtil::TTreeX* ttree = new TasUtil::TTreeX("t", "A Baby Ntuple");

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

    // -~-~-~-~-~-~-~-~
    // Electron ID tool
    // -~-~-~-~-~-~-~-~
    TasUtil::print( "Creating MVA input for electrons.", __FUNCTION__ );
    createAndInitMVA("CORE", true, false, 80); // for electrons

    // -~-~-~-~-~-~-~
    // Good Runs List
    // -~-~-~-~-~-~-~
    TString json_file = "Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON_snt.txt"; // 26p4 fb
    TasUtil::print( "Setting grl:" + json_file, __FUNCTION__ );
    set_goodrun_file( json_file.Data() );

    // -~-~-~-~-~-~
    // b-tagging SF
    // -~-~-~-~-~-~
    // TODO

    // -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~
    // retrieve JEC from files, if using
    // -~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~

    // stores current corrections for a given event
    FactorizedJetCorrector   * jet_corrector_pfL1FastJetL2L3_current = NULL;
    JetCorrectionUncertainty * jecUnc_current                        = NULL;

    // default corrections
    std::vector<std::string> jetcorr_filenames_pfL1FastJetL2L3;
    FactorizedJetCorrector   * jet_corrector_pfL1FastJetL2L3 = NULL;
    JetCorrectionUncertainty * jecUnc                        = NULL;

    // corrections for later runs in 2016F
    std::vector<std::string> jetcorr_filenames_pfL1FastJetL2L3_postrun278802;
    FactorizedJetCorrector   * jet_corrector_pfL1FastJetL2L3_postrun278802 = NULL;
    JetCorrectionUncertainty * jecUnc_postrun278802                        = NULL;

    if ( applyJECfromFile )
    {
        jetcorr_filenames_pfL1FastJetL2L3.clear();

        //JECs for 76X
        if ( output_name.Contains( "16Dec2015" ) || output_name.Contains( "76X_mcRun2" ) )
        {
            if ( output_name.Contains( "Run2015C" ) || output_name.Contains( "Run2015D" ) )
            {
                jetcorr_filenames_pfL1FastJetL2L3.clear();
                jetcorr_filenames_pfL1FastJetL2L3.push_back( "jetCorrections/source_76X/DATA/Fall15_25nsV2_DATA_L1FastJet_AK4PFchs.txt" );
                jetcorr_filenames_pfL1FastJetL2L3.push_back( "jetCorrections/source_76X/DATA/Fall15_25nsV2_DATA_L2Relative_AK4PFchs.txt" );
                jetcorr_filenames_pfL1FastJetL2L3.push_back( "jetCorrections/source_76X/DATA/Fall15_25nsV2_DATA_L3Absolute_AK4PFchs.txt" );
                jetcorr_filenames_pfL1FastJetL2L3.push_back( "jetCorrections/source_76X/DATA/Fall15_25nsV2_DATA_L2L3Residual_AK4PFchs.txt" );
                jecUnc = new JetCorrectionUncertainty( "jetCorrections/source_76X/DATA/Fall15_25nsV2_DATA_Uncertainty_AK4PFchs.txt" );
            }
            else
            {
                // files for 76X MC
                jetcorr_filenames_pfL1FastJetL2L3.clear();
                jetcorr_filenames_pfL1FastJetL2L3.push_back( "jetCorrections/source_76X/MC/Fall15_25nsV2_MC_L1FastJet_AK4PFchs.txt" );
                jetcorr_filenames_pfL1FastJetL2L3.push_back( "jetCorrections/source_76X/MC/Fall15_25nsV2_MC_L2Relative_AK4PFchs.txt" );
                jetcorr_filenames_pfL1FastJetL2L3.push_back( "jetCorrections/source_76X/MC/Fall15_25nsV2_MC_L3Absolute_AK4PFchs.txt" );
                jecUnc = new JetCorrectionUncertainty( "jetCorrections/source_76X/MC/Fall15_25nsV2_MC_Uncertainty_AK4PFchs.txt" );
            }
        }
        else if ( output_name.Contains( "80MiniAODv" ) || output_name.Contains( "RelVal" ) )
        {
            // files for 80X MC, ICHEP production
            jetcorr_filenames_pfL1FastJetL2L3.clear();
            jetcorr_filenames_pfL1FastJetL2L3.push_back( "jetCorrections/source_80X/MC/Spring16_25nsV1_MC_L1FastJet_AK4PFchs.txt" );
            jetcorr_filenames_pfL1FastJetL2L3.push_back( "jetCorrections/source_80X/MC/Spring16_25nsV1_MC_L2Relative_AK4PFchs.txt" );
            jetcorr_filenames_pfL1FastJetL2L3.push_back( "jetCorrections/source_80X/MC/Spring16_25nsV1_MC_L3Absolute_AK4PFchs.txt" );
            jecUnc = new JetCorrectionUncertainty( "jetCorrections/source_80X/MC/Spring16_25nsV1_MC_Uncertainty_AK4PFchs.txt" );
        }
        else if ( output_name.Contains( "Summer16" ) || output_name.Contains( "TEST" ) )
        {
            // files for 80X MC, Summer16 (Moriond17) production
            jetcorr_filenames_pfL1FastJetL2L3.clear();
            jetcorr_filenames_pfL1FastJetL2L3.push_back( "jetCorrections/source_80X/MC/Summer16_23Sep2016V3_MC_L1FastJet_AK4PFchs.txt" );
            jetcorr_filenames_pfL1FastJetL2L3.push_back( "jetCorrections/source_80X/MC/Summer16_23Sep2016V3_MC_L2Relative_AK4PFchs.txt" );
            jetcorr_filenames_pfL1FastJetL2L3.push_back( "jetCorrections/source_80X/MC/Summer16_23Sep2016V3_MC_L3Absolute_AK4PFchs.txt" );
            jecUnc = new
                JetCorrectionUncertainty( "jetCorrections/source_80X/MC/Summer16_23Sep2016V3_MC_Uncertainty_AK4PFchs.txt" );
        }
        else if ( output_name.Contains( "Run2016" ) || output_name.Contains( "CMSSW_8_0_11_V08-00-06" ) )
        {

            // // This scheme will eventually be used for Moriond. For now just use JECs from ICHEP
            if ( output_name.Contains( "Run2016B" ) || output_name.Contains( "Run2016C" ) || output_name.Contains( "Run2016D" ) )
            {
                // files for 80X Data
                jetcorr_filenames_pfL1FastJetL2L3.clear();
                jetcorr_filenames_pfL1FastJetL2L3.push_back( "jetCorrections/source_80X/DATA/Summer16_23Sep2016BCDV3_DATA_L1FastJet_AK4PFchs.txt" );
                jetcorr_filenames_pfL1FastJetL2L3.push_back( "jetCorrections/source_80X/DATA/Summer16_23Sep2016BCDV3_DATA_L2Relative_AK4PFchs.txt" );
                jetcorr_filenames_pfL1FastJetL2L3.push_back( "jetCorrections/source_80X/DATA/Summer16_23Sep2016BCDV3_DATA_L3Absolute_AK4PFchs.txt" );
                jetcorr_filenames_pfL1FastJetL2L3.push_back( "jetCorrections/source_80X/DATA/Summer16_23Sep2016BCDV3_DATA_L2L3Residual_AK4PFchs.txt" );
                jecUnc = new
                    JetCorrectionUncertainty( "jetCorrections/source_80X/DATA/Summer16_23Sep2016BCDV3_DATA_Uncertainty_AK4PFchs.txt" );
            }

            if ( output_name.Contains( "Run2016E" ) || output_name.Contains( "Run2016F" ) )
            {
                // files for 80X Data
                jetcorr_filenames_pfL1FastJetL2L3.clear();
                jetcorr_filenames_pfL1FastJetL2L3.push_back( "jetCorrections/source_80X/DATA/Summer16_23Sep2016EFV3_DATA_L1FastJet_AK4PFchs.txt" );
                jetcorr_filenames_pfL1FastJetL2L3.push_back( "jetCorrections/source_80X/DATA/Summer16_23Sep2016EFV3_DATA_L2Relative_AK4PFchs.txt" );
                jetcorr_filenames_pfL1FastJetL2L3.push_back( "jetCorrections/source_80X/DATA/Summer16_23Sep2016EFV3_DATA_L3Absolute_AK4PFchs.txt" );
                jetcorr_filenames_pfL1FastJetL2L3.push_back( "jetCorrections/source_80X/DATA/Summer16_23Sep2016EFV3_DATA_L2L3Residual_AK4PFchs.txt" );
                jecUnc = new
                    JetCorrectionUncertainty( "jetCorrections/source_80X/DATA/Summer16_23Sep2016EFV3_DATA_Uncertainty_AK4PFchs.txt" );

                jetcorr_filenames_pfL1FastJetL2L3_postrun278802.clear();
                jetcorr_filenames_pfL1FastJetL2L3_postrun278802.push_back( "jetCorrections/source_80X/DATA/Summer16_23Sep2016GV3_DATA_L1FastJet_AK4PFchs.txt" );
                jetcorr_filenames_pfL1FastJetL2L3_postrun278802.push_back( "jetCorrections/source_80X/DATA/Summer16_23Sep2016GV3_DATA_L2Relative_AK4PFchs.txt" );
                jetcorr_filenames_pfL1FastJetL2L3_postrun278802.push_back( "jetCorrections/source_80X/DATA/Summer16_23Sep2016GV3_DATA_L3Absolute_AK4PFchs.txt" );
                jetcorr_filenames_pfL1FastJetL2L3_postrun278802.push_back( "jetCorrections/source_80X/DATA/Summer16_23Sep2016GV3_DATA_L2L3Residual_AK4PFchs.txt" );
                jecUnc_postrun278802 = new
                    JetCorrectionUncertainty( "jetCorrections/source_80X/DATA/Summer16_23Sep2016GV3_DATA_Uncertainty_AK4PFchs.txt" );
                jet_corrector_pfL1FastJetL2L3_postrun278802  = makeJetCorrector( jetcorr_filenames_pfL1FastJetL2L3_postrun278802 );
            }

            if ( output_name.Contains( "Run2016G" ) )
            {
                // files for 80X Data
                jetcorr_filenames_pfL1FastJetL2L3.clear();
                jetcorr_filenames_pfL1FastJetL2L3.push_back( "jetCorrections/source_80X/DATA/Summer16_23Sep2016GV3_DATA_L1FastJet_AK4PFchs.txt" );
                jetcorr_filenames_pfL1FastJetL2L3.push_back( "jetCorrections/source_80X/DATA/Summer16_23Sep2016GV3_DATA_L2Relative_AK4PFchs.txt" );
                jetcorr_filenames_pfL1FastJetL2L3.push_back( "jetCorrections/source_80X/DATA/Summer16_23Sep2016GV3_DATA_L3Absolute_AK4PFchs.txt" );
                jetcorr_filenames_pfL1FastJetL2L3.push_back( "jetCorrections/source_80X/DATA/Summer16_23Sep2016GV3_DATA_L2L3Residual_AK4PFchs.txt" );
                jecUnc = new
                    JetCorrectionUncertainty( "jetCorrections/source_80X/DATA/Summer16_23Sep2016GV3_DATA_Uncertainty_AK4PFchs.txt" );

                jetcorr_filenames_pfL1FastJetL2L3_postrun278802.clear();
                jetcorr_filenames_pfL1FastJetL2L3_postrun278802.push_back( "jetCorrections/source_80X/DATA/Summer16_23Sep2016GV3_DATA_L1FastJet_AK4PFchs.txt" );
                jetcorr_filenames_pfL1FastJetL2L3_postrun278802.push_back( "jetCorrections/source_80X/DATA/Summer16_23Sep2016GV3_DATA_L2Relative_AK4PFchs.txt" );
                jetcorr_filenames_pfL1FastJetL2L3_postrun278802.push_back( "jetCorrections/source_80X/DATA/Summer16_23Sep2016GV3_DATA_L3Absolute_AK4PFchs.txt" );
                jetcorr_filenames_pfL1FastJetL2L3_postrun278802.push_back( "jetCorrections/source_80X/DATA/Summer16_23Sep2016GV3_DATA_L2L3Residual_AK4PFchs.txt" );
                jecUnc_postrun278802 = new
                    JetCorrectionUncertainty( "jetCorrections/source_80X/DATA/Summer16_23Sep2016GV3_DATA_Uncertainty_AK4PFchs.txt" );
                jet_corrector_pfL1FastJetL2L3_postrun278802  = makeJetCorrector( jetcorr_filenames_pfL1FastJetL2L3_postrun278802 );
            }

            if ( output_name.Contains( "Run2016H" ) )
            {
                jetcorr_filenames_pfL1FastJetL2L3.clear();
                jetcorr_filenames_pfL1FastJetL2L3.push_back( "jetCorrections/source_80X/DATA/Summer16_23Sep2016HV3_DATA_L1FastJet_AK4PFchs.txt" );
                jetcorr_filenames_pfL1FastJetL2L3.push_back( "jetCorrections/source_80X/DATA/Summer16_23Sep2016HV3_DATA_L2Relative_AK4PFchs.txt" );
                jetcorr_filenames_pfL1FastJetL2L3.push_back( "jetCorrections/source_80X/DATA/Summer16_23Sep2016HV3_DATA_L3Absolute_AK4PFchs.txt" );
                jetcorr_filenames_pfL1FastJetL2L3.push_back( "jetCorrections/source_80X/DATA/Summer16_23Sep2016HV3_DATA_L2L3Residual_AK4PFchs.txt" );
                jecUnc = new
                    JetCorrectionUncertainty( "jetCorrections/source_80X/DATA/Summer16_23Sep2016HV3_DATA_Uncertainty_AK4PFchs.txt" );

                jetcorr_filenames_pfL1FastJetL2L3_postrun278802.clear();
                jetcorr_filenames_pfL1FastJetL2L3_postrun278802.push_back( "jetCorrections/source_80X/DATA/Summer16_23Sep2016HV3_DATA_L1FastJet_AK4PFchs.txt" );
                jetcorr_filenames_pfL1FastJetL2L3_postrun278802.push_back( "jetCorrections/source_80X/DATA/Summer16_23Sep2016HV3_DATA_L2Relative_AK4PFchs.txt" );
                jetcorr_filenames_pfL1FastJetL2L3_postrun278802.push_back( "jetCorrections/source_80X/DATA/Summer16_23Sep2016HV3_DATA_L3Absolute_AK4PFchs.txt" );
                jetcorr_filenames_pfL1FastJetL2L3_postrun278802.push_back( "jetCorrections/source_80X/DATA/Summer16_23Sep2016HV3_DATA_L2L3Residual_AK4PFchs.txt" );
                jecUnc_postrun278802 = new
                    JetCorrectionUncertainty( "jetCorrections/source_80X/DATA/Summer16_23Sep2016HV3_DATA_Uncertainty_AK4PFchs.txt" );
                jet_corrector_pfL1FastJetL2L3_postrun278802  = makeJetCorrector( jetcorr_filenames_pfL1FastJetL2L3_postrun278802 );
            }
        }

        if ( isFastSim )
        {
            // files for 25ns fastsim samples
            jetcorr_filenames_pfL1FastJetL2L3.clear();
            jetcorr_filenames_pfL1FastJetL2L3.push_back( "jetCorrections/source_80X/FASTSIM/Spring16_FastSimV1_L1FastJet_AK4PFchs.txt" );
            jetcorr_filenames_pfL1FastJetL2L3.push_back( "jetCorrections/source_80X/FASTSIM/Spring16_FastSimV1_L2Relative_AK4PFchs.txt" );
            jetcorr_filenames_pfL1FastJetL2L3.push_back( "jetCorrections/source_80X/FASTSIM/Spring16_FastSimV1_L3Absolute_AK4PFchs.txt" );
            jecUnc = new
                JetCorrectionUncertainty( "jetCorrections/source_80X/FASTSIM/Spring16_FastSimV1_Uncertainty_AK4PFchs.txt" );
        }

        if ( jetcorr_filenames_pfL1FastJetL2L3.size() == 0 )
            TasUtil::error("JECs are not set properly. Check the JECs.", __FUNCTION__);

        TasUtil::print("JECs used:");

        for ( size_t jecind = 0; jecind < jetcorr_filenames_pfL1FastJetL2L3.size(); jecind++ )
            TasUtil::print( TString( jetcorr_filenames_pfL1FastJetL2L3.at( jecind ) ) );

        jet_corrector_pfL1FastJetL2L3  = makeJetCorrector( jetcorr_filenames_pfL1FastJetL2L3 );
    }

    // -~-~-~-~-~
    // Event Loop
    // -~-~-~-~-~
    TasUtil::Looper<CMS3> looper(chain, &cms3, nevents);
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

//BabyTree_->Branch("gen_ht", &gen_ht );
//
//BabyTree_->Branch("nBJetTight", &nBJetTight );
//BabyTree_->Branch("nBJetMedium", &nBJetMedium );
//BabyTree_->Branch("nBJetLoose", &nBJetLoose );
//
//BabyTree_->Branch("nBJetTight_up", &nBJetTight_up );
//BabyTree_->Branch("nBJetMedium_up", &nBJetMedium_up );
//BabyTree_->Branch("nBJetLoose_up", &nBJetLoose_up );
//
//BabyTree_->Branch("nBJetTight_dn", &nBJetTight_dn );
//BabyTree_->Branch("nBJetMedium_dn", &nBJetMedium_dn );
//BabyTree_->Branch("nBJetLoose_dn", &nBJetLoose_dn );
//
//BabyTree_->Branch("nJet200MuFrac50DphiMet", &nJet200MuFrac50DphiMet );
//
//BabyTree_->Branch("nMuons10", &nMuons10 );
//BabyTree_->Branch("nBadMuons20", &nBadMuons20 );
//BabyTree_->Branch("nElectrons10", &nElectrons10 );
//BabyTree_->Branch("nGammas20", &nGammas20 );
//BabyTree_->Branch("nTaus20", &nTaus20 );
//
//BabyTree_->Branch("met_pt"      , &met_pt       );
//BabyTree_->Branch("met_phi"     , &met_phi      );
//BabyTree_->Branch("met_calo_pt" , &met_calo_pt  );
//BabyTree_->Branch("met_calo_phi", &met_calo_phi );
//BabyTree_->Branch("met_miniaod_pt" , &met_miniaod_pt  );
//BabyTree_->Branch("met_miniaod_phi", &met_miniaod_phi );
//BabyTree_->Branch("met_muegclean_pt" , &met_muegclean_pt  );
//BabyTree_->Branch("met_muegclean_phi", &met_muegclean_phi );
//BabyTree_->Branch("met_rawPt"   , &met_rawPt    );
//BabyTree_->Branch("met_rawPhi"  , &met_rawPhi   );
//BabyTree_->Branch("met_genPt"   , &met_genPt    );
//BabyTree_->Branch("met_genPhi"  , &met_genPhi   );
//BabyTree_->Branch("sumet_raw"   , &sumet_raw    );
//
////MET Filters
//BabyTree_->Branch("Flag_ecalLaserCorrFilter"   , &Flag_ecalLaserCorrFilter   );
//BabyTree_->Branch("Flag_hcalLaserEventFilter"  , &Flag_hcalLaserEventFilter  );
//BabyTree_->Branch("Flag_trackingFailureFilter" , &Flag_trackingFailureFilter );
//BabyTree_->Branch("Flag_CSCTightHaloFilter"    , &Flag_CSCTightHaloFilter    );
//
//// recommended from MET twiki
//BabyTree_->Branch("Flag_HBHENoiseFilter"                    , &Flag_HBHENoiseFilter                    );
//BabyTree_->Branch("Flag_HBHEIsoNoiseFilter"                 , &Flag_HBHEIsoNoiseFilter                 );
//BabyTree_->Branch("Flag_CSCTightHalo2015Filter"             , &Flag_CSCTightHalo2015Filter             );
//BabyTree_->Branch("Flag_EcalDeadCellTriggerPrimitiveFilter" , &Flag_EcalDeadCellTriggerPrimitiveFilter );
//BabyTree_->Branch("Flag_goodVertices"                       , &Flag_goodVertices                       );
//BabyTree_->Branch("Flag_eeBadScFilter"                      , &Flag_eeBadScFilter                      );
//BabyTree_->Branch("Flag_globalTightHalo2016"                , &Flag_globalTightHalo2016                );
//
//BabyTree_->Branch("Flag_badMuonFilter"                , &Flag_badMuonFilter               );
//BabyTree_->Branch("Flag_badChargedCandidateFilter"    , &Flag_badChargedCandidateFilter   );
//BabyTree_->Branch("Flag_badMuonFilterv2"              , &Flag_badMuonFilterv2             );
//BabyTree_->Branch("Flag_badChargedCandidateFilterv2"  , &Flag_badChargedCandidateFilterv2 );
//BabyTree_->Branch("Flag_badMuons"                     , &Flag_badMuons                    );
//BabyTree_->Branch("Flag_duplicateMuons"               , &Flag_duplicateMuons              );
//BabyTree_->Branch("Flag_noBadMuons"                   , &Flag_noBadMuons                  );
//
////TRIGGER
//// for ATLAS cross checks
//BabyTree_->Branch("HLT_singleEl"       , &HLT_singleEl       );
//BabyTree_->Branch("HLT_singleMu"       , &HLT_singleMu       );
//BabyTree_->Branch("HLT_singleMu_noiso" , &HLT_singleMu_noiso );
//
//// Double electron
//BabyTree_->Branch("HLT_DoubleEl_noiso" , &HLT_DoubleEl_noiso );
//BabyTree_->Branch("HLT_DoubleEl"       , &HLT_DoubleEl       ); // prescaled - turned off
//BabyTree_->Branch("HLT_DoubleEl_DZ"    , &HLT_DoubleEl_DZ    ); // prescaled
//BabyTree_->Branch("HLT_DoubleEl_DZ_2"  , &HLT_DoubleEl_DZ_2  ); // new
//
//// electron-muon
//BabyTree_->Branch("HLT_MuEG"         , &HLT_MuEG         );
//BabyTree_->Branch("HLT_MuEG_2"       , &HLT_MuEG_2       );
//BabyTree_->Branch("HLT_MuEG_noiso"   , &HLT_MuEG_noiso   );
//BabyTree_->Branch("HLT_MuEG_noiso_2" , &HLT_MuEG_noiso_2 );
//
//BabyTree_->Branch("HLT_Mu8_EG17"     , &HLT_Mu8_EG17     );
//BabyTree_->Branch("HLT_Mu8_EG23"     , &HLT_Mu8_EG23     );
//BabyTree_->Branch("HLT_Mu8_EG23_DZ"  , &HLT_Mu8_EG23_DZ  );
//
//BabyTree_->Branch("HLT_Mu12_EG23_DZ" , &HLT_Mu12_EG23_DZ );
//
//BabyTree_->Branch("HLT_Mu17_EG12"    , &HLT_Mu17_EG12    );
//
//BabyTree_->Branch("HLT_Mu23_EG8"     , &HLT_Mu23_EG8     );
//BabyTree_->Branch("HLT_Mu23_EG8_DZ"  , &HLT_Mu23_EG8_DZ  );
//BabyTree_->Branch("HLT_Mu23_EG12"    , &HLT_Mu23_EG12    );
//BabyTree_->Branch("HLT_Mu23_EG12_DZ" , &HLT_Mu23_EG12_DZ );
//
//// Double electron
//BabyTree_->Branch("HLT_DoubleMu_noiso"    , &HLT_DoubleMu_noiso    );
//BabyTree_->Branch("HLT_DoubleMu_noiso_27_8"     , &HLT_DoubleMu_noiso_27_8     );
//BabyTree_->Branch("HLT_DoubleMu_noiso_30_11"    , &HLT_DoubleMu_noiso_30_11    );
//BabyTree_->Branch("HLT_DoubleMu_noiso_40_11"    , &HLT_DoubleMu_noiso_40_11    );
//BabyTree_->Branch("HLT_DoubleMu"          , &HLT_DoubleMu          );
//BabyTree_->Branch("HLT_DoubleMu_tk"       , &HLT_DoubleMu_tk       );
//BabyTree_->Branch("HLT_DoubleMu_dbltk"    , &HLT_DoubleMu_dbltk    );
//BabyTree_->Branch("HLT_DoubleMu_nonDZ"    , &HLT_DoubleMu_nonDZ    );
//BabyTree_->Branch("HLT_DoubleMu_tk_nonDZ" , &HLT_DoubleMu_tk_nonDZ ); // new unprescaled : use these
//
//// Single photon
//BabyTree_->Branch("HLT_Photon22_R9Id90_HE10_IsoM"  , &HLT_Photon22_R9Id90_HE10_IsoM  );
//BabyTree_->Branch("HLT_Photon30_R9Id90_HE10_IsoM"  , &HLT_Photon30_R9Id90_HE10_IsoM  );
//BabyTree_->Branch("HLT_Photon36_R9Id90_HE10_IsoM"  , &HLT_Photon36_R9Id90_HE10_IsoM  );
//BabyTree_->Branch("HLT_Photon50_R9Id90_HE10_IsoM"  , &HLT_Photon50_R9Id90_HE10_IsoM  );
//BabyTree_->Branch("HLT_Photon75_R9Id90_HE10_IsoM"  , &HLT_Photon75_R9Id90_HE10_IsoM  );
//BabyTree_->Branch("HLT_Photon90_R9Id90_HE10_IsoM"  , &HLT_Photon90_R9Id90_HE10_IsoM  );
//BabyTree_->Branch("HLT_Photon120_R9Id90_HE10_IsoM" , &HLT_Photon120_R9Id90_HE10_IsoM );
//BabyTree_->Branch("HLT_Photon165_R9Id90_HE10_IsoM" , &HLT_Photon165_R9Id90_HE10_IsoM );
//BabyTree_->Branch("HLT_Photon165_HE10"             , &HLT_Photon165_HE10             );
//
//BabyTree_->Branch("HLT_CaloJet500_NoJetID" , &HLT_CaloJet500_NoJetID );
//BabyTree_->Branch("HLT_ECALHT800_NoJetID"  , &HLT_ECALHT800_NoJetID  );
//
//BabyTree_->Branch("HLT_Photon22_R9Id90_HE10_IsoM_matchedtophoton"  , &HLT_Photon22_R9Id90_HE10_IsoM_matchedtophoton  );
//BabyTree_->Branch("HLT_Photon30_R9Id90_HE10_IsoM_matchedtophoton"  , &HLT_Photon30_R9Id90_HE10_IsoM_matchedtophoton  );
//BabyTree_->Branch("HLT_Photon36_R9Id90_HE10_IsoM_matchedtophoton"  , &HLT_Photon36_R9Id90_HE10_IsoM_matchedtophoton  );
//BabyTree_->Branch("HLT_Photon50_R9Id90_HE10_IsoM_matchedtophoton"  , &HLT_Photon50_R9Id90_HE10_IsoM_matchedtophoton  );
//BabyTree_->Branch("HLT_Photon75_R9Id90_HE10_IsoM_matchedtophoton"  , &HLT_Photon75_R9Id90_HE10_IsoM_matchedtophoton  );
//BabyTree_->Branch("HLT_Photon90_R9Id90_HE10_IsoM_matchedtophoton"  , &HLT_Photon90_R9Id90_HE10_IsoM_matchedtophoton  );
//BabyTree_->Branch("HLT_Photon120_R9Id90_HE10_IsoM_matchedtophoton" , &HLT_Photon120_R9Id90_HE10_IsoM_matchedtophoton );
//BabyTree_->Branch("HLT_Photon165_R9Id90_HE10_IsoM_matchedtophoton" , &HLT_Photon165_R9Id90_HE10_IsoM_matchedtophoton );
//BabyTree_->Branch("HLT_Photon165_HE10_matchedtophoton"             , &HLT_Photon165_HE10_matchedtophoton             );
//
//BabyTree_->Branch("dilmass", &dilmass );
//BabyTree_->Branch("dilpt"  , &dilpt );
//BabyTree_->Branch("dRll"   , &dRll );
//
//BabyTree_->Branch("matched_neutralemf", &matched_neutralemf );
//BabyTree_->Branch("matched_emf"       , &matched_emf );
//BabyTree_->Branch("elveto", &elveto );
//
//BabyTree_->Branch("nlep"             , &nlep, "nlep/I" );
//BabyTree_->Branch("nveto_leptons"    , &nveto_leptons );
//
//BabyTree_->Branch("nVetoEl_relIso03EAless01" , &nVetoEl_relIso03EAless01 );
//BabyTree_->Branch("nVetoEl_relIso03EAless02" , &nVetoEl_relIso03EAless02 );
//BabyTree_->Branch("nVetoEl_relIso03EAless03" , &nVetoEl_relIso03EAless03 );
//BabyTree_->Branch("nVetoEl_relIso03EAless04" , &nVetoEl_relIso03EAless04 );
//BabyTree_->Branch("nVetoMu_relIso03EAless01" , &nVetoMu_relIso03EAless01 );
//BabyTree_->Branch("nVetoMu_relIso03EAless02" , &nVetoMu_relIso03EAless02 );
//BabyTree_->Branch("nVetoMu_relIso03EAless03" , &nVetoMu_relIso03EAless03 );
//BabyTree_->Branch("nVetoMu_relIso03EAless04" , &nVetoMu_relIso03EAless04 );
//
//
//BabyTree_->Branch("lep_p4"           , &lep_p4         );
//BabyTree_->Branch("lep_pt"           , "std::vector< Float_t >"       , &lep_pt         );
//BabyTree_->Branch("lep_eta"          , "std::vector< Float_t >"       , &lep_eta        );
//BabyTree_->Branch("lep_phi"          , "std::vector< Float_t >"       , &lep_phi        );
//BabyTree_->Branch("lep_mass"         , "std::vector< Float_t >"       , &lep_mass       );
//BabyTree_->Branch("lep_charge"       , "std::vector< Int_t >"         , &lep_charge     );
//
////New vars for testing WWW
//BabyTree_->Branch("lep_3ch_agree"            , "std::vector< Bool_t  > " , &lep_3ch_agree             );
//BabyTree_->Branch("lep_isFromW"              , "std::vector< Bool_t   > " , &lep_isFromW               );
//BabyTree_->Branch("lep_isFromZ"              , "std::vector< Bool_t   > " , &lep_isFromZ               );
//BabyTree_->Branch("lep_isFromB"              , "std::vector< Bool_t   > " , &lep_isFromB               );
//BabyTree_->Branch("lep_isFromC"              , "std::vector< Bool_t   > " , &lep_isFromC               );
//BabyTree_->Branch("lep_isFromL"              , "std::vector< Bool_t   > " , &lep_isFromL               );
//BabyTree_->Branch("lep_isFromLF"             , "std::vector< Bool_t   > " , &lep_isFromLF              );
//BabyTree_->Branch("lep_closest_jet_p4"       , &lep_closest_jet_p4 );
//BabyTree_->Branch("lep_ptRatio"              , "std::vector< Double_t >" , &lep_ptRatio               );
//BabyTree_->Branch("lep_motherIdSS"           , "std::vector< Int_t    >" , &lep_motherIdSS            );
//BabyTree_->Branch("lep_genPart_index"        , "std::vector< Int_t    >" , &lep_genPart_index         );
//BabyTree_->Branch("lep_coneCorrPt"           , "std::vector< Double_t >" , &lep_coneCorrPt            );
//BabyTree_->Branch("lep_ptRel"                , "std::vector< Double_t >" , &lep_ptRel                 );
//BabyTree_->Branch("lep_relIso03"             , "std::vector< Double_t >" , &lep_relIso03              );
//BabyTree_->Branch("lep_relIso03DB"           , "std::vector< Double_t >" , &lep_relIso03DB            );
//BabyTree_->Branch("lep_relIso03EA"           , "std::vector< Double_t >" , &lep_relIso03EA            );
//BabyTree_->Branch("lep_relIso03EAv2"         , "std::vector< Double_t >" , &lep_relIso03EAv2          );
//BabyTree_->Branch("lep_relIso04DB"           , "std::vector< Double_t >" , &lep_relIso04DB            );
//BabyTree_->Branch("lep_relIso04EA"           , "std::vector< Double_t >" , &lep_relIso04EA            );
//BabyTree_->Branch("lep_relIso04EAv2"         , "std::vector< Double_t >" , &lep_relIso04EAv2          );
//BabyTree_->Branch("lep_miniRelIsoCMS3_EA"    , "std::vector< Double_t >" , &lep_miniRelIsoCMS3_EA     );
//BabyTree_->Branch("lep_miniRelIsoCMS3_EAv2"  , "std::vector< Double_t >" , &lep_miniRelIsoCMS3_EAv2   );
//BabyTree_->Branch("lep_miniRelIsoCMS3_DB"    , "std::vector< Double_t >" , &lep_miniRelIsoCMS3_DB     );
//
//BabyTree_->Branch("lep_pass_VVV_cutbased_veto"              , "std::vector< Bool_t  > " , &lep_pass_VVV_cutbased_veto             );
//BabyTree_->Branch("lep_pass_VVV_cutbased_veto_noiso"        , "std::vector< Bool_t  > " , &lep_pass_VVV_cutbased_veto_noiso       );
//BabyTree_->Branch("lep_pass_VVV_cutbased_veto_noiso_noip"   , "std::vector< Bool_t  > " , &lep_pass_VVV_cutbased_veto_noiso_noip  );
//BabyTree_->Branch("lep_pass_VVV_cutbased_fo"                , "std::vector< Bool_t  > " , &lep_pass_VVV_cutbased_fo               );
//BabyTree_->Branch("lep_pass_VVV_cutbased_fo_noiso"          , "std::vector< Bool_t  > " , &lep_pass_VVV_cutbased_fo_noiso         );
//BabyTree_->Branch("lep_pass_VVV_cutbased_tight_noiso"       , "std::vector< Bool_t  > " , &lep_pass_VVV_cutbased_tight_noiso      );
//BabyTree_->Branch("lep_pass_VVV_cutbased_tight"             , "std::vector< Bool_t  > " , &lep_pass_VVV_cutbased_tight            );
//BabyTree_->Branch("lep_pass_VVV_MVAbased_tight_noiso"       , "std::vector< Bool_t  > " , &lep_pass_VVV_MVAbased_tight_noiso      );
//BabyTree_->Branch("lep_pass_VVV_MVAbased_tight"             , "std::vector< Bool_t  > " , &lep_pass_VVV_MVAbased_tight            );
//BabyTree_->Branch("lep_pass_VVV_baseline"                   , "std::vector< Bool_t  > " , &lep_pass_VVV_baseline                  );
//
//BabyTree_->Branch("nlep_VVV_cutbased_veto"             , &nlep_VVV_cutbased_veto             , "nlep_VVV_cutbased_veto/I" );
//BabyTree_->Branch("nlep_VVV_cutbased_veto_noiso"       , &nlep_VVV_cutbased_veto_noiso       , "nlep_VVV_cutbased_veto_noiso/I" );
//BabyTree_->Branch("nlep_VVV_cutbased_veto_noiso_noip"  , &nlep_VVV_cutbased_veto_noiso_noip  , "nlep_VVV_cutbased_veto_noiso_noip/I" );
//BabyTree_->Branch("nlep_VVV_cutbased_fo"               , &nlep_VVV_cutbased_fo               , "nlep_VVV_cutbased_fo/I" );
//BabyTree_->Branch("nlep_VVV_cutbased_fo_noiso"         , &nlep_VVV_cutbased_fo_noiso         , "nlep_VVV_cutbased_fo_noiso/I" );
//BabyTree_->Branch("nlep_VVV_cutbased_tight_noiso"      , &nlep_VVV_cutbased_tight_noiso      , "nlep_VVV_cutbased_tight_noiso/I" );
//BabyTree_->Branch("nlep_VVV_cutbased_tight"            , &nlep_VVV_cutbased_tight            , "nlep_VVV_cutbased_tight/I" );
//BabyTree_->Branch("nlep_VVV_MVAbased_tight_noiso"      , &nlep_VVV_MVAbased_tight_noiso      , "nlep_VVV_MVAbased_tight_noiso/I" );
//BabyTree_->Branch("nlep_VVV_MVAbased_tight"            , &nlep_VVV_MVAbased_tight            , "nlep_VVV_MVAbased_tight/I" );
//BabyTree_->Branch("nlep_VVV_baseline"                  , &nlep_VVV_baseline                  , "nlep_VVV_baseline/I" );
//
//BabyTree_->Branch("lep_pdgId"        , "std::vector< Int_t >"         , &lep_pdgId      );
//BabyTree_->Branch("lep_mc_Id"        , "std::vector< Int_t >"         , &lep_mc_Id      );
//BabyTree_->Branch("lep_dxy"          , "std::vector< Float_t >"       , &lep_dxy        );
//BabyTree_->Branch("lep_ip3d"         , "std::vector< Float_t >"       , &lep_ip3d       );
//BabyTree_->Branch("lep_ip3derr"      , "std::vector< Float_t >"       , &lep_ip3derr    );
//BabyTree_->Branch("lep_etaSC"        , "std::vector< Float_t >"       , &lep_etaSC      );
//BabyTree_->Branch("lep_dz"           , "std::vector< Float_t >"       , &lep_dz         );
//BabyTree_->Branch("lep_tightId"      , "std::vector< Int_t >"         , &lep_tightId    );
//BabyTree_->Branch("lep_mcMatchId"    , "std::vector< Int_t >"         , &lep_mcMatchId  );
//BabyTree_->Branch("lep_lostHits"     , "std::vector< Int_t >"         , &lep_lostHits   );
//BabyTree_->Branch("lep_convVeto"     , "std::vector< Int_t >"         , &lep_convVeto   );
//BabyTree_->Branch("lep_tightCharge"  , "std::vector< Int_t >"         , &lep_tightCharge);
//BabyTree_->Branch("lep_MVA"          , "std::vector< Float_t >"       , &lep_MVA        );
//BabyTree_->Branch("lep_validfraction", &lep_validfraction );
//BabyTree_->Branch("lep_pterr"        , &lep_pterr         );
//BabyTree_->Branch("lep_sta_pterrOpt" , &lep_sta_pterrOpt  );
//BabyTree_->Branch("lep_glb_pterrOpt" , &lep_glb_pterrOpt  );
//// BabyTree_->Branch("lep_bft_pterrOpt" , &lep_bft_pterrOpt  );
//BabyTree_->Branch("lep_x2ondof"      , &lep_x2ondof       );
//BabyTree_->Branch("lep_sta_x2ondof"  , &lep_sta_x2ondof   );
//BabyTree_->Branch("lep_glb_x2ondof"  , &lep_glb_x2ondof   );
//// BabyTree_->Branch("lep_bft_x2ondof"  , &lep_bft_x2ondof   );
//
//BabyTree_->Branch("nisoTrack_5gev" , &nisoTrack_5gev );
//BabyTree_->Branch("nisoTrack_mt2"  , &nisoTrack_mt2  );
//
//BabyTree_->Branch("nisoTrack_mt2_cleaned_VVV_cutbased_veto"  ,             &nisoTrack_mt2_cleaned_VVV_cutbased_veto             );
//BabyTree_->Branch("nisoTrack_mt2_cleaned_VVV_cutbased_veto_noiso"  ,       &nisoTrack_mt2_cleaned_VVV_cutbased_veto_noiso       );
//BabyTree_->Branch("nisoTrack_mt2_cleaned_VVV_cutbased_veto_noiso_noip"  ,  &nisoTrack_mt2_cleaned_VVV_cutbased_veto_noiso_noip  );
//BabyTree_->Branch("nisoTrack_mt2_cleaned_VVV_cutbased_fo"  ,               &nisoTrack_mt2_cleaned_VVV_cutbased_fo               );
//BabyTree_->Branch("nisoTrack_mt2_cleaned_VVV_cutbased_fo_noiso"  ,         &nisoTrack_mt2_cleaned_VVV_cutbased_fo_noiso         );
//BabyTree_->Branch("nisoTrack_mt2_cleaned_VVV_cutbased_tight_noiso"  ,      &nisoTrack_mt2_cleaned_VVV_cutbased_tight_noiso      );
//BabyTree_->Branch("nisoTrack_mt2_cleaned_VVV_cutbased_tight"  ,            &nisoTrack_mt2_cleaned_VVV_cutbased_tight            );
//BabyTree_->Branch("nisoTrack_mt2_cleaned_VVV_MVAbased_tight_noiso"  ,      &nisoTrack_mt2_cleaned_VVV_MVAbased_tight_noiso      );
//BabyTree_->Branch("nisoTrack_mt2_cleaned_VVV_MVAbased_tight"  ,            &nisoTrack_mt2_cleaned_VVV_MVAbased_tight            );
//BabyTree_->Branch("nisoTrack_mt2_cleaned_VVV_baseline"  ,                  &nisoTrack_mt2_cleaned_VVV_baseline                  );
//
//BabyTree_->Branch("nisoTrack_stop"  , &nisoTrack_stop  );
//BabyTree_->Branch("nisoTrack_PFLep5_woverlaps"  , &nisoTrack_PFLep5_woverlaps  );
//BabyTree_->Branch("nisoTrack_PFHad10_woverlaps" , &nisoTrack_PFHad10_woverlaps );
//
//BabyTree_->Branch("ngamma"             , &ngamma        , "ngamma/I" );
//BabyTree_->Branch("gamma_p4"           , &gamma_p4    );
//BabyTree_->Branch("gamma_pt"           , "std::vector <Float_t>" , &gamma_pt           );
//BabyTree_->Branch("gamma_eta"          , "std::vector <Float_t>" , &gamma_eta          );
//BabyTree_->Branch("gamma_phi"          , "std::vector <Float_t>" , &gamma_phi          );
//BabyTree_->Branch("gamma_mass"         , "std::vector <Float_t>" , &gamma_mass         );
//BabyTree_->Branch("gamma_mcMatchId"    , "std::vector <Int_t  >" , &gamma_mcMatchId    );
//BabyTree_->Branch("gamma_genPt"        , "std::vector <Float_t>" , &gamma_genPt        );
//BabyTree_->Branch("gamma_genIso"       , "std::vector <Float_t>" , &gamma_genIso       );
//BabyTree_->Branch("gamma_chHadIso"     , "std::vector <Float_t>" , &gamma_chHadIso     );
//BabyTree_->Branch("gamma_neuHadIso"    , "std::vector <Float_t>" , &gamma_neuHadIso    );
//BabyTree_->Branch("gamma_phIso"        , "std::vector <Float_t>" , &gamma_phIso        );
//BabyTree_->Branch("gamma_sigmaIetaIeta", "std::vector <Float_t>" , &gamma_sigmaIetaIeta);
//BabyTree_->Branch("gamma_r9"           , "std::vector <Float_t>" , &gamma_r9           );
//BabyTree_->Branch("gamma_hOverE"       , "std::vector <Float_t>" , &gamma_hOverE       );
//BabyTree_->Branch("gamma_hOverE_online", "std::vector <Float_t>" , &gamma_hOverE_online);
//BabyTree_->Branch("gamma_idCutBased"   , "std::vector <Int_t  >" , &gamma_idCutBased   );
//BabyTree_->Branch("gamma_ecpfclusiso"                            , &gamma_ecpfclusiso  );
//BabyTree_->Branch("gamma_hcpfclusiso"                            , &gamma_hcpfclusiso  );
//BabyTree_->Branch("gamma_hollowtkiso03"                          , &gamma_hollowtkiso03);
//BabyTree_->Branch("gamma_genIsPromptFinalState"                  , &gamma_genIsPromptFinalState);
//BabyTree_->Branch("gamma_drMinParton"                            , &gamma_drMinParton);
//
//BabyTree_->Branch("ngenPart"         , &ngenPart        , "ngenPart/I" );
//BabyTree_->Branch("genPart_p4"       , &genPart_p4         );
//BabyTree_->Branch("genPart_pt"       , "std::vector <Float_t>" , &genPart_pt        );
//BabyTree_->Branch("genPart_eta"      , "std::vector <Float_t>" , &genPart_eta       );
//BabyTree_->Branch("genPart_phi"      , "std::vector <Float_t>" , &genPart_phi       );
//BabyTree_->Branch("genPart_mass"     , "std::vector <Float_t>" , &genPart_mass      );
//BabyTree_->Branch("genPart_pdgId"    , "std::vector <Int_t  >" , &genPart_pdgId     );
//BabyTree_->Branch("genPart_status"   , "std::vector <Int_t  >" , &genPart_status    );
//BabyTree_->Branch("genPart_charge"   , "std::vector <Float_t>" , &genPart_charge    );
//BabyTree_->Branch("genPart_motherId" , "std::vector <Int_t  >" , &genPart_motherId  );
//BabyTree_->Branch("genPart_grandmaId", "std::vector <Int_t  >" , &genPart_grandmaId );
//BabyTree_->Branch("genPart_isp6status3"  , "std::vector <Bool_t  >" , &genPart_isp6status3  );
//
//BabyTree_->Branch("ngenLep", &ngenLep, "ngenLep/I" );
//BabyTree_->Branch("genLep_pt"      , "std::vector <Float_t>" , &genLep_pt      );
//BabyTree_->Branch("genLep_eta"     , "std::vector <Float_t>" , &genLep_eta     );
//BabyTree_->Branch("genLep_phi"     , "std::vector <Float_t>" , &genLep_phi     );
//BabyTree_->Branch("genLep_mass"    , "std::vector <Float_t>" , &genLep_mass    );
//BabyTree_->Branch("genLep_pdgId"   , "std::vector <Int_t  >" , &genLep_pdgId   );
//BabyTree_->Branch("genLep_status"  , "std::vector <Int_t  >" , &genLep_status  );
//BabyTree_->Branch("genLep_charge"  , "std::vector <Float_t>" , &genLep_charge  );
//BabyTree_->Branch("genLep_sourceId", "std::vector <Int_t  >" , &genLep_sourceId);
//BabyTree_->Branch("genLep_isp6status3"  , "std::vector <Bool_t  >" , &genLep_isp6status3  );
//
//BabyTree_->Branch("ngenTau", &ngenTau, "ngenTau/I" );
//BabyTree_->Branch("genTau_pt"      , "std::vector <Float_t>" , &genTau_pt      );
//BabyTree_->Branch("genTau_eta"     , "std::vector <Float_t>" , &genTau_eta     );
//BabyTree_->Branch("genTau_phi"     , "std::vector <Float_t>" , &genTau_phi     );
//BabyTree_->Branch("genTau_mass"    , "std::vector <Float_t>" , &genTau_mass    );
//BabyTree_->Branch("genTau_pdgId"   , "std::vector <Int_t  >" , &genTau_pdgId   );
//BabyTree_->Branch("genTau_status"  , "std::vector <Int_t  >" , &genTau_status  );
//BabyTree_->Branch("genTau_charge"  , "std::vector <Float_t>" , &genTau_charge  );
//BabyTree_->Branch("genTau_sourceId", "std::vector <Int_t  >" , &genTau_sourceId);
//
//BabyTree_->Branch("ngenLepFromTau", &ngenLepFromTau, "ngenLepFromTau/I" );
//BabyTree_->Branch("genLepFromTau_pt"      , "std::vector <Float_t>" , &genLepFromTau_pt      );
//BabyTree_->Branch("genLepFromTau_eta"     , "std::vector <Float_t>" , &genLepFromTau_eta     );
//BabyTree_->Branch("genLepFromTau_phi"     , "std::vector <Float_t>" , &genLepFromTau_phi     );
//BabyTree_->Branch("genLepFromTau_mass"    , "std::vector <Float_t>" , &genLepFromTau_mass    );
//BabyTree_->Branch("genLepFromTau_pdgId"   , "std::vector <Int_t  >" , &genLepFromTau_pdgId   );
//BabyTree_->Branch("genLepFromTau_status"  , "std::vector <Int_t  >" , &genLepFromTau_status  );
//BabyTree_->Branch("genLepFromTau_charge"  , "std::vector <Float_t>" , &genLepFromTau_charge  );
//BabyTree_->Branch("genLepFromTau_sourceId", "std::vector <Int_t  >" , &genLepFromTau_sourceId);
//
////----- JETS - pt > JET_PT_MIN, eta < 2.4
//BabyTree_->Branch("njets"           , &njets             );
//BabyTree_->Branch("jets_p4"         , &jets_p4           );
//BabyTree_->Branch("removed_jets_p4" , &removed_jets_p4   );
//BabyTree_->Branch("removed_jets_csv", &removed_jets_csv  );
//BabyTree_->Branch("jets_medb_p4"    , &jets_medb_p4      );
//
//BabyTree_->Branch("njets_up"        , &njets_up        );
//BabyTree_->Branch("jets_up_p4"      , &jets_up_p4      );
//BabyTree_->Branch("jets_medb_up_p4" , &jets_medb_up_p4 );
//BabyTree_->Branch("jets_csv"        , &jets_csv        );
//BabyTree_->Branch("jets_up_csv"     , &jets_up_csv     );
//
//BabyTree_->Branch("njets_dn"        , &njets_dn        );
//BabyTree_->Branch("jets_dn_p4"      , &jets_dn_p4      );
//BabyTree_->Branch("jets_medb_dn_p4" , &jets_medb_dn_p4 );
//BabyTree_->Branch("jets_dn_csv"     , &jets_dn_csv     );
//
//BabyTree_->Branch("jets_muf"          , &jets_muf          );
//BabyTree_->Branch("jets_mcFlavour"    , &jets_mcFlavour    );
//BabyTree_->Branch("jets_mcHadronFlav" , &jets_mcHadronFlav );
//
////----- HIGH PT PF CANDS
//BabyTree_->Branch("nhighPtPFcands"           , &nhighPtPFcands        );
//BabyTree_->Branch("highPtPFcands_p4"         , &highPtPFcands_p4      );
//BabyTree_->Branch("highPtPFcands_dz"         , &highPtPFcands_dz      );
//BabyTree_->Branch("highPtPFcands_pdgId"      , &highPtPFcands_pdgId   );
//
//BabyTree_->Branch("ht"    , &ht    );
//BabyTree_->Branch("ht_up" , &ht_up );
//BabyTree_->Branch("ht_dn" , &ht_dn );
//
//BabyTree_->Branch("metsig_unofficial"    , &metsig_unofficial    );
//BabyTree_->Branch("metsig_unofficial_up" , &metsig_unofficial_up );
//BabyTree_->Branch("metsig_unofficial_dn" , &metsig_unofficial_dn );
//
//BabyTree_->Branch("mt_lep1" , &mt_lep1 );
//BabyTree_->Branch("mt2"     , &mt2     );
//BabyTree_->Branch("mt2_up"  , &mt2_up  );
//BabyTree_->Branch("mt2_dn"  , &mt2_dn  );
//BabyTree_->Branch("mt2j"    , &mt2j    );
//BabyTree_->Branch("mt2b"    , &mt2b    );
//BabyTree_->Branch("mt2b_up" , &mt2b_up );
//BabyTree_->Branch("mt2b_dn" , &mt2b_dn );
//BabyTree_->Branch("mt2_genmet"     , &mt2_genmet     );
//BabyTree_->Branch("mt2b_genmet"    , &mt2b_genmet    );
//
//BabyTree_->Branch("mjj_mindphi"   , &mjj_mindphi   );
//BabyTree_->Branch("mjj"           , &mjj           );
//BabyTree_->Branch("mbb_csv"       , &mbb_csv       );
//BabyTree_->Branch("mbb_bpt"       , &mbb_bpt       );
//BabyTree_->Branch("dphi_jj"       , &dphi_jj       );
//BabyTree_->Branch("dphi_ll"       , &dphi_ll       );
//BabyTree_->Branch("sum_mlb"       , &sum_mlb       );
//BabyTree_->Branch("deta_jj"       , &deta_jj       );
//BabyTree_->Branch("dR_jj"         , &dR_jj         );
//BabyTree_->Branch("dphi_metj1"    , &dphi_metj1    );
//BabyTree_->Branch("dphi_metj2"    , &dphi_metj2    );
//BabyTree_->Branch("dphi_genmetj1" , &dphi_genmetj1 );
//BabyTree_->Branch("dphi_genmetj2" , &dphi_genmetj2 );
//
//BabyTree_->Branch("mjj_mindphi_up" , &mjj_mindphi_up );
//BabyTree_->Branch("mjj_up"         , &mjj_up         );
//BabyTree_->Branch("mbb_csv_up"     , &mbb_csv_up     );
//BabyTree_->Branch("mbb_bpt_up"     , &mbb_bpt_up     );
//BabyTree_->Branch("dphi_jj_up"     , &dphi_jj_up     );
//BabyTree_->Branch("dphi_ll_up"     , &dphi_ll_up     );
//BabyTree_->Branch("sum_mlb_up"     , &sum_mlb_up     );
//BabyTree_->Branch("deta_jj_up"     , &deta_jj_up     );
//BabyTree_->Branch("dR_jj_up"       , &dR_jj_up       );
//BabyTree_->Branch("dphi_metj1_up"  , &dphi_metj1_up  );
//BabyTree_->Branch("dphi_metj2_up"  , &dphi_metj2_up  );
//
//BabyTree_->Branch("mjj_mindphi_dn" , &mjj_mindphi_dn );
//BabyTree_->Branch("mjj_dn"         , &mjj_dn         );
//BabyTree_->Branch("mbb_csv_dn"     , &mbb_csv_dn     );
//BabyTree_->Branch("mbb_bpt_dn"     , &mbb_bpt_dn     );
//BabyTree_->Branch("dphi_jj_dn"     , &dphi_jj_dn     );
//BabyTree_->Branch("dphi_ll_dn"     , &dphi_ll_dn     );
//BabyTree_->Branch("sum_mlb_dn"     , &sum_mlb_dn     );
//BabyTree_->Branch("deta_jj_dn"     , &deta_jj_dn     );
//BabyTree_->Branch("dR_jj_dn"       , &dR_jj_dn       );
//BabyTree_->Branch("dphi_metj1_dn"  , &dphi_metj1_dn  );
//BabyTree_->Branch("dphi_metj2_dn"  , &dphi_metj2_dn  );
//
//BabyTree_->Branch("weight_btagsf", &weight_btagsf );
//BabyTree_->Branch("weight_btagsf_heavy_UP", &weight_btagsf_heavy_UP );
//BabyTree_->Branch("weight_btagsf_light_UP", &weight_btagsf_light_UP );
//BabyTree_->Branch("weight_btagsf_heavy_DN", &weight_btagsf_heavy_DN );
//BabyTree_->Branch("weight_btagsf_light_DN", &weight_btagsf_light_DN );
//
//BabyTree_->Branch("chpfcands_0013_pt"     , &chpfcands_0013_pt   );
//BabyTree_->Branch("chpfcands_1316_pt"     , &chpfcands_1316_pt   );
//BabyTree_->Branch("chpfcands_1624_pt"     , &chpfcands_1624_pt   );
//BabyTree_->Branch("chpfcands_2430_pt"     , &chpfcands_2430_pt   );
//BabyTree_->Branch("chpfcands_30in_pt"     , &chpfcands_30in_pt   );
//BabyTree_->Branch("phpfcands_0013_pt"     , &phpfcands_0013_pt   );
//BabyTree_->Branch("phpfcands_1316_pt"     , &phpfcands_1316_pt   );
//BabyTree_->Branch("phpfcands_1624_pt"     , &phpfcands_1624_pt   );
//BabyTree_->Branch("phpfcands_2430_pt"     , &phpfcands_2430_pt   );
//BabyTree_->Branch("phpfcands_30in_pt"     , &phpfcands_30in_pt   );
//BabyTree_->Branch("nupfcands_0013_pt"     , &nupfcands_0013_pt   );
//BabyTree_->Branch("nupfcands_1316_pt"     , &nupfcands_1316_pt   );
//BabyTree_->Branch("nupfcands_1624_pt"     , &nupfcands_1624_pt   );
//BabyTree_->Branch("nupfcands_2430_pt"     , &nupfcands_2430_pt   );
//BabyTree_->Branch("nupfcands_30in_pt"     , &nupfcands_30in_pt   );
//
//BabyTree_->Branch("chpfcands_0013_sumet"     , &chpfcands_0013_sumet   );
//BabyTree_->Branch("chpfcands_1316_sumet"     , &chpfcands_1316_sumet   );
//BabyTree_->Branch("chpfcands_1624_sumet"     , &chpfcands_1624_sumet   );
//BabyTree_->Branch("chpfcands_2430_sumet"     , &chpfcands_2430_sumet   );
//BabyTree_->Branch("chpfcands_30in_sumet"     , &chpfcands_30in_sumet   );
//BabyTree_->Branch("phpfcands_0013_sumet"     , &phpfcands_0013_sumet   );
//BabyTree_->Branch("phpfcands_1316_sumet"     , &phpfcands_1316_sumet   );
//BabyTree_->Branch("phpfcands_1624_sumet"     , &phpfcands_1624_sumet   );
//BabyTree_->Branch("phpfcands_2430_sumet"     , &phpfcands_2430_sumet   );
//BabyTree_->Branch("phpfcands_30in_sumet"     , &phpfcands_30in_sumet   );
//BabyTree_->Branch("nupfcands_0013_sumet"     , &nupfcands_0013_sumet   );
//BabyTree_->Branch("nupfcands_1316_sumet"     , &nupfcands_1316_sumet   );
//BabyTree_->Branch("nupfcands_1624_sumet"     , &nupfcands_1624_sumet   );
//BabyTree_->Branch("nupfcands_2430_sumet"     , &nupfcands_2430_sumet   );
//BabyTree_->Branch("nupfcands_30in_sumet"     , &nupfcands_30in_sumet   );
//
//BabyTree_->Branch("chpfcands_0013_phi"     , &chpfcands_0013_phi   );
//BabyTree_->Branch("chpfcands_1316_phi"     , &chpfcands_1316_phi   );
//BabyTree_->Branch("chpfcands_1624_phi"     , &chpfcands_1624_phi   );
//BabyTree_->Branch("chpfcands_2430_phi"     , &chpfcands_2430_phi   );
//BabyTree_->Branch("chpfcands_30in_phi"     , &chpfcands_30in_phi   );
//BabyTree_->Branch("phpfcands_0013_phi"     , &phpfcands_0013_phi   );
//BabyTree_->Branch("phpfcands_1316_phi"     , &phpfcands_1316_phi   );
//BabyTree_->Branch("phpfcands_1624_phi"     , &phpfcands_1624_phi   );
//BabyTree_->Branch("phpfcands_2430_phi"     , &phpfcands_2430_phi   );
//BabyTree_->Branch("phpfcands_30in_phi"     , &phpfcands_30in_phi   );
//BabyTree_->Branch("nupfcands_0013_phi"     , &nupfcands_0013_phi   );
//BabyTree_->Branch("nupfcands_1316_phi"     , &nupfcands_1316_phi   );
//BabyTree_->Branch("nupfcands_1624_phi"     , &nupfcands_1624_phi   );
//BabyTree_->Branch("nupfcands_2430_phi"     , &nupfcands_2430_phi   );
//BabyTree_->Branch("nupfcands_30in_phi"     , &nupfcands_30in_phi   );
//
//BabyTree_->Branch("met_T1CHS_pt"               , &met_T1CHS_pt               );
//BabyTree_->Branch("met_T1CHS_phi"              , &met_T1CHS_phi              );
//BabyTree_->Branch("met_T1CHS_fromCORE_pt"      , &met_T1CHS_fromCORE_pt      );
//BabyTree_->Branch("met_T1CHS_fromCORE_phi"     , &met_T1CHS_fromCORE_phi     );
//
//BabyTree_->Branch("met_T1CHS_miniAOD_CORE_pt"      , &met_T1CHS_miniAOD_CORE_pt      );
//BabyTree_->Branch("met_T1CHS_miniAOD_CORE_phi"     , &met_T1CHS_miniAOD_CORE_phi     );
//BabyTree_->Branch("met_T1CHS_miniAOD_CORE_up_pt"      , &met_T1CHS_miniAOD_CORE_up_pt      );
//BabyTree_->Branch("met_T1CHS_miniAOD_CORE_up_phi"     , &met_T1CHS_miniAOD_CORE_up_phi     );
//BabyTree_->Branch("met_T1CHS_miniAOD_CORE_dn_pt"      , &met_T1CHS_miniAOD_CORE_dn_pt      );
//BabyTree_->Branch("met_T1CHS_miniAOD_CORE_dn_phi"     , &met_T1CHS_miniAOD_CORE_dn_phi     );
//
//BabyTree_->Branch("decayedphoton_lep1_p4", &decayedphoton_lep1_p4 );
//BabyTree_->Branch("decayedphoton_lep2_p4", &decayedphoton_lep2_p4 );
//BabyTree_->Branch("decayedphoton_bosn_p4", &decayedphoton_bosn_p4 );
//BabyTree_->Branch("decayedphoton_mt2"    , &decayedphoton_mt2     );
//
//BabyTree_->Branch("hyp_type", &hyp_type);
//BabyTree_->Branch("evt_type", &evt_type);
//
//BabyTree_->Branch("mass_gluino", &mass_gluino);
//BabyTree_->Branch("mass_LSP"   , &mass_LSP   );
//BabyTree_->Branch("mass_chi"   , &mass_chi   );
//
//BabyTree_->Branch("isrboost"   , &isrboost   );
//BabyTree_->Branch("isr_njets"  , &isr_njets  );
//BabyTree_->Branch("isr_weight" , &isr_weight );
//BabyTree_->Branch("isr_unc"    , &isr_unc    );
//
//BabyTree_->Branch("weightsf_lepid"    , &weightsf_lepid     );
//BabyTree_->Branch("weightsf_lepiso"   , &weightsf_lepiso    );
//BabyTree_->Branch("weightsf_lepip"    , &weightsf_lepip     );
//BabyTree_->Branch("weightsf_lepreco"  , &weightsf_lepreco   );
//BabyTree_->Branch("weightsf_lepconv"  , &weightsf_lepconv   );
//BabyTree_->Branch("weightsf_lepid_FS" , &weightsf_lepid_FS  );
//BabyTree_->Branch("weightsf_lepiso_FS", &weightsf_lepiso_FS );
//BabyTree_->Branch("weightsf_lepip_FS" , &weightsf_lepip_FS  );
