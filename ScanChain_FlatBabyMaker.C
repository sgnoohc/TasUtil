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
    core.createLeptonBranches( ttree, { WW_veto_v2 } );

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

//        if ( verbose )
//            cout << "before electrons" << endl;
//
//        //ELECTRONS
//        nlep = 0;
//
//        nlep_VVV_cutbased_veto            = 0;
//        nlep_VVV_cutbased_veto_noiso      = 0;
//        nlep_VVV_cutbased_veto_noiso_noip = 0;
//        nlep_VVV_cutbased_fo              = 0;
//        nlep_VVV_cutbased_fo_noiso        = 0;
//        nlep_VVV_cutbased_tight_noiso     = 0;
//        nlep_VVV_cutbased_tight           = 0;
//        nlep_VVV_MVAbased_tight_noiso     = 0;
//        nlep_VVV_MVAbased_tight           = 0;
//        nlep_VVV_baseline                 = 0;
//
//        nElectrons10 = 0;
//        nVetoEl_relIso03EAless01 = 0;
//        nVetoEl_relIso03EAless02 = 0;
//        nVetoEl_relIso03EAless03 = 0;
//        nVetoEl_relIso03EAless04 = 0;
//
//        for ( unsigned int iEl = 0; iEl < cms3.els_p4().size(); iEl++ )
//        {
//            //cout<<"checking electrons"<<endl;
//            if ( !( passElectronSelection_VVV( iEl, VVV_cutbased_veto_noiso_noip )
//                    || passElectronSelection_VVV( iEl, VVV_MVAbased_tight_noiso ) ) )
//            {
//                /*cout<<"Electron did not pass analysis selection, checking veto selection"<<endl;
//                cout<<"etaSC: "<<fabs(els_etaSC().at(iEl))<<" ";
//                cout<<"conv_vtx: "<<els_conv_vtx_flag().at(iEl)<<" ";
//                cout<<"exp_inlayers: "<<els_exp_innerlayers().at(iEl)<<" ";
//                cout<<"dxy: "<<fabs(els_dxyPV().at(iEl))<<" ";
//                cout<<"TSnoIso: "<<isTriggerSafenoIso_v1(iEl)<<" ";
//                cout<<"dz: "<<fabs(els_dzPV().at(iEl))<<" ";
//                cout<<"sip: "<<fabs(els_ip3d().at(iEl))/els_ip3derr().at(iEl)<<" ";
//                cout<<"MVA: "<<getMVAoutput(iEl)<<endl;*/
//                if ( passElectronSelection_VVV( iEl, VVV_cutbased_veto_noiso ) )
//                {
//                    //cout<<"Electron passed veto selection, eta is "<< cms3.els_p4().at(iEl).eta() <<endl;
//                    if ( abs( cms3.els_p4().at( iEl ).eta() ) <= 2.4 )
//                    {
//                        //cout<<"Electron eta <= 2.4, adding to veto leptons selection, relIsoValue is "<<eleRelIso03EA(iEl, /*eaversion=*/1)<<endl;
//                        nveto_leptons++;
//
//                        if ( eleRelIso03EA( iEl, /*eaversion=*/1 ) > 0.40 )
//                            nVetoEl_relIso03EAless04++;
//                        else if ( eleRelIso03EA( iEl, /*eaversion=*/1 ) > 0.30 )
//                            nVetoEl_relIso03EAless03++;
//                        else if ( eleRelIso03EA( iEl, /*eaversion=*/1 ) > 0.20 )
//                            nVetoEl_relIso03EAless02++;
//                        else if ( eleRelIso03EA( iEl, /*eaversion=*/1 ) > 0.10 )
//                            nVetoEl_relIso03EAless01++;
//
//                        nVetoEl_relIso03EAless03 += nVetoEl_relIso03EAless04;
//                        nVetoEl_relIso03EAless02 += nVetoEl_relIso03EAless03;
//                        nVetoEl_relIso03EAless01 += nVetoEl_relIso03EAless02;
//                    }
//
//                    //cout<<"num veto els: 01=="<<nVetoEl_relIso03EAless01<<" 02=="<< nVetoEl_relIso03EAless02<<" 03=="<< nVetoEl_relIso03EAless03<<" 04=="<< nVetoEl_relIso03EAless04<<endl;
//                }
//
//                continue;
//            }
//
//            nElectrons10++;
//
//            if ( cms3.els_p4().at( iEl ).pt() > 10.0 )
//            {
//                lep_pt_ordering       .push_back( std::pair<int, float>( nlep, cms3.els_p4().at( iEl ).pt() ) );
//                vec_lep_p4s          .push_back( cms3.els_p4().at( iEl ) );
//                vec_lep_pt           .push_back( cms3.els_p4().at( iEl ).pt() );
//                vec_lep_eta          .push_back( cms3.els_p4().at( iEl ).eta() );   //save eta, even though we use SCeta for ID
//                vec_lep_phi          .push_back( cms3.els_p4().at( iEl ).phi() );
//                vec_lep_mass         .push_back( cms3.els_mass().at( iEl ) );
//                vec_lep_charge       .push_back( cms3.els_charge().at( iEl ) );
//
//                //WWW Selection Vars
//                vec_lep_3ch_agree            .push_back( threeChargeAgree( iEl ) );
//                const LorentzVector& temp_jet_p4 = closestJet( cms3.els_p4().at( iEl ), 0.4, 3.0, /*whichCorr = */2 );
//                float closeJetPt            = temp_jet_p4.pt();
//                vec_lep_closest_jet_p4       .push_back( temp_jet_p4 );
//                vec_lep_ptRatio              .push_back( ( closeJetPt > 0. ? cms3.els_p4().at( iEl ).pt() / closeJetPt : 1. ) );
//                vec_lep_coneCorrPt           .push_back( coneCorrPt( /* pdgid= */ 11, iEl ) );
//                vec_lep_ptRel                .push_back( ptRel( cms3.els_p4().at( iEl ), temp_jet_p4, true ) );
//                vec_lep_relIso03             .push_back( eleRelIso03_noCorr( iEl ) );
//                vec_lep_relIso03DB           .push_back( eleRelIso03DB( iEl ) );
//                vec_lep_relIso03EA           .push_back( eleRelIso03EA( iEl, /*eaversion=*/1 ) );
//                vec_lep_relIso03EAv2         .push_back( eleRelIso03EA( iEl, /*eaversion=*/2 ) );
//                vec_lep_relIso04DB           .push_back( elRelIsoCustomCone( iEl, 0.4, false, 0.0, /*useDBCorr=*/true ) );
//                vec_lep_relIso04EA           .push_back( elRelIsoCustomCone( iEl, 0.4, false,
//                        0.0, /*useDBCorr=*/false, /*useEACorr=*/true, /*mindr=*/ -1, /*eaversion=*/1 ) );
//                vec_lep_relIso04EAv2         .push_back( elRelIsoCustomCone( iEl, 0.4, false,
//                        0.0, /*useDBCorr=*/false, /*useEACorr=*/true, /*mindr=*/ -1, /*eaversion=*/2 ) );
//                vec_lep_miniRelIsoCMS3_EA    .push_back( elMiniRelIsoCMS3_EA( iEl, /*eaversion=*/1 ) );
//                vec_lep_miniRelIsoCMS3_EAv2  .push_back( elMiniRelIsoCMS3_EA( iEl, /*eaversion=*/2 ) );
//                vec_lep_miniRelIsoCMS3_DB    .push_back( elMiniRelIsoCMS3_DB( iEl ) );
//
//                vec_lep_pass_VVV_cutbased_veto            .push_back( passElectronSelection_VVV( iEl, VVV_cutbased_veto ) );
//                vec_lep_pass_VVV_cutbased_veto_noiso      .push_back( passElectronSelection_VVV( iEl, VVV_cutbased_veto_noiso ) );
//                vec_lep_pass_VVV_cutbased_veto_noiso_noip .push_back( passElectronSelection_VVV( iEl, VVV_cutbased_veto_noiso_noip ) );
//                vec_lep_pass_VVV_cutbased_fo              .push_back( passElectronSelection_VVV( iEl, VVV_cutbased_fo ) );
//                vec_lep_pass_VVV_cutbased_fo_noiso        .push_back( passElectronSelection_VVV( iEl, VVV_cutbased_fo_noiso ) );
//                vec_lep_pass_VVV_cutbased_tight_noiso     .push_back( passElectronSelection_VVV( iEl, VVV_cutbased_tight_noiso ) );
//                vec_lep_pass_VVV_cutbased_tight           .push_back( passElectronSelection_VVV( iEl, VVV_cutbased_tight ) );
//                vec_lep_pass_VVV_MVAbased_tight_noiso     .push_back( passElectronSelection_VVV( iEl, VVV_MVAbased_tight_noiso ) );
//                vec_lep_pass_VVV_MVAbased_tight           .push_back( passElectronSelection_VVV( iEl, VVV_MVAbased_tight ) );
//                vec_lep_pass_VVV_baseline                 .push_back( passElectronSelection_VVV( iEl, VVV_baseline ) );
//
//                if ( vec_lep_pass_VVV_cutbased_veto.back() )
//                    nlep_VVV_cutbased_veto++;
//
//                if ( vec_lep_pass_VVV_cutbased_veto_noiso.back() )
//                    nlep_VVV_cutbased_veto_noiso++;
//
//                if ( vec_lep_pass_VVV_cutbased_veto_noiso_noip.back() )
//                    nlep_VVV_cutbased_veto_noiso_noip++;
//
//                if ( vec_lep_pass_VVV_cutbased_fo.back() )
//                    nlep_VVV_cutbased_fo++;
//
//                if ( vec_lep_pass_VVV_cutbased_fo_noiso.back() )
//                    nlep_VVV_cutbased_fo_noiso++;
//
//                if ( vec_lep_pass_VVV_cutbased_tight_noiso.back() )
//                    nlep_VVV_cutbased_tight_noiso++;
//
//                if ( vec_lep_pass_VVV_cutbased_tight.back() )
//                    nlep_VVV_cutbased_tight++;
//
//                if ( vec_lep_pass_VVV_MVAbased_tight_noiso.back() )
//                    nlep_VVV_MVAbased_tight_noiso++;
//
//                if ( vec_lep_pass_VVV_MVAbased_tight.back() )
//                    nlep_VVV_MVAbased_tight++;
//
//                if ( vec_lep_pass_VVV_baseline.back() )
//                    nlep_VVV_baseline++;
//
//                if ( !isData )
//                {
//                    vec_lep_isFromW              .push_back( isFromW( /* pdgid= */11, iEl ) );
//                    vec_lep_isFromZ              .push_back( isFromZ( /* pdgid= */11, iEl ) );
//                    vec_lep_isFromB              .push_back( isFromB( /* pdgid= */11, iEl ) );
//                    vec_lep_isFromC              .push_back( isFromC( /* pdgid= */11, iEl ) );
//                    vec_lep_isFromL              .push_back( isFromLight( /* pdgid= */11, iEl ) );
//                    vec_lep_isFromLF             .push_back( isFromLightFake( /* pdgid= */11, iEl ) );
//                    pair<int, int> motherId_genIdx = lepMotherID_v2( Lep( cms3.els_charge().at( iEl ) * ( -11 ),
//                                                     iEl ) ); //don't forget the sign
//                    vec_lep_motherIdSS           .push_back( motherId_genIdx.first );
//                    vec_lep_genPart_index        .push_back( motherId_genIdx.second );
//                    vec_lep_mc_Id                .push_back( cms3.els_mc_id().at( iEl ) );
//                }
//                else
//                {
//                    vec_lep_isFromW              .push_back( -1 );
//                    vec_lep_isFromZ              .push_back( -1 );
//                    vec_lep_isFromB              .push_back( -1 );
//                    vec_lep_isFromC              .push_back( -1 );
//                    vec_lep_isFromL              .push_back( -1 );
//                    vec_lep_isFromLF             .push_back( -1 );
//                    vec_lep_motherIdSS           .push_back( -1 );
//                    vec_lep_genPart_index        .push_back( -1 );
//                    vec_lep_mc_Id                .push_back( -1 );
//                }
//
//                vec_lep_pdgId        .push_back( cms3.els_charge().at( iEl ) * ( -11 ) );
//                vec_lep_dxy          .push_back( cms3.els_dxyPV().at( iEl ) );
//                vec_lep_ip3d         .push_back( cms3.els_ip3d().at( iEl ) );
//                vec_lep_ip3derr      .push_back( cms3.els_ip3derr().at( iEl ) );
//                vec_lep_dz           .push_back( cms3.els_dzPV().at( iEl ) );
//                vec_lep_tightId      .push_back( eleTightID( iEl, ZMET ) );
//                vec_lep_etaSC        .push_back( els_etaSC().at( iEl ) );
//                vec_lep_MVA          .push_back( getMVAoutput( iEl ) );
//                vec_lep_validfraction.push_back( -1 );
//                vec_lep_ptErr        .push_back( cms3.els_ptErr() .at( iEl ) );
//                vec_lep_sta_pterrOpt .push_back( -1 );
//                vec_lep_glb_pterrOpt .push_back( -1 );
//                // vec_lep_bft_pterrOpt .push_back ( cms3.els_bfit_qoverpError().at(iEl) / cms3.els_bfit_qoverp() .at(iEl) );
//                vec_lep_x2ondof      .push_back( cms3.els_chi2()            .at( iEl ) / cms3.els_ndof()        .at( iEl ) );
//                vec_lep_sta_x2ondof  .push_back( -1 );
//                vec_lep_glb_x2ondof  .push_back( -1 );
//                // vec_lep_bft_x2ondof  .push_back ( cms3.els_bfit_chi2()       .at(iEl) / cms3.els_bfit_ndof()   .at(iEl) );
//
//                if ( !isData && ( cms3.els_mc3dr().at( iEl ) < 0.2 && cms3.els_mc3idx().at( iEl ) != -9999
//                                  && abs( cms3.els_mc3_id().at( iEl ) ) == 11 ) ) // matched to a prunedGenParticle electron?
//                {
//                    int momid =  abs( genPart_motherId[cms3.els_mc3idx().at( iEl )] );
//                    vec_lep_mcMatchId.push_back( momid != 11 ? momid : genPart_grandmaId[cms3.els_mc3idx().at(
//                                                     iEl )] ); // if mother is different store mother, otherwise store grandmother
//                }
//                else
//                    vec_lep_mcMatchId.push_back( 0 );
//
//                vec_lep_lostHits.push_back( cms3.els_exp_innerlayers().at( iEl ) ); //cms2.els_lost_pixelhits().at(iEl);
//                vec_lep_convVeto.push_back( !cms3.els_conv_vtx_flag().at( iEl ) );
//                vec_lep_tightCharge.push_back( tightChargeEle( iEl ) );
//
//                nlep++;
//            }
//
//            if ( cms3.els_p4().at( iEl ).pt() > 20.0 )
//                p4sLeptonsForJetCleaning.push_back( cms3.els_p4().at( iEl ) );
//        }

    }

    // -~-~-~
    // Output
    // -~-~-~
    ofile->cd();
    ttree->Write();
    ofile->Close();

}

// eof
