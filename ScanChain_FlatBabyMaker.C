//  .
// ..: P. Chang, philip@physics.ucsd.edu

#include "ScanChain_FlatBabyMaker.h"

//_________________________________________________________________________________________________
void ScanChain(TChain* chain, TString output_name, int nevents=-1)
{

    // -~-~-~-~-~-~-~-~-
    // Set up histograms
    // -~-~-~-~-~-~-~-~-

    // The end goal of this code is to create a histogram of dilepton invariant mass.
    // We will see whether we can observe a Z boson resonance peak.
    // (i.e. we're reproducing UA1/2 experiment result. See Nobel Prize 1984.)
    TH1F* hist_dilepton_mass = new TH1F("hist_dilepton_mass", "", 60, 60., 120.);

    // -~-~-~-~-~
    // Event Loop
    // -~-~-~-~-~

    TasUtil::Looper<CMS3> looper(chain, &cms3, nevents);
    while (looper.nextEvent())
    {

        // -~-~-~-~-~-~-~-~-~-
        // Select Good Leptons
        // -~-~-~-~-~-~-~-~-~-
        // Not every leptons "reconstructed" in an event are good leptons.
        // Detectors/algorithms are not perfect in identifying subatomic particles.
        // So inevitably, we get "fakes".
        // So we reject "bad" leptons if a lepton does not pass our "selection algorithm".
        // We call this "selection algorithm", "ID". (ID stands for "identification".)
        // So using our jargon we'd say, "this muon did not pass our muon ID criteria."

        // vector instance where we will store our good leptons.
        std::vector<MyLepton> good_leptons;

        // Loop over Muons in this event
        for (unsigned int imu = 0; imu < cms3.mus_p4().size(); ++imu)
        {
            // If the muon does not pass the "HAD_loose_v4" ID, we reject
            if (!muonID(imu, id_level_t::HAD_loose_v4))
                continue;

            // Instantiate a lepton object.
            MyLepton mylepton;

            // Get the LorentzVector of this muon.
            mylepton.p4 = cms3.mus_p4()[imu];

            // Get the charge of this muon.
            mylepton.charge = cms3.mus_charge()[imu];

            // Muons' pdgId is 13 (muon with charge -1) or -13 (anti-muon with charge +1).
            mylepton.pdgId = mylepton.charge * (-13);

            // Save the lepton to a vector.
            good_leptons.push_back(mylepton);
        }

        // Loop over electrons
        for (unsigned int iel = 0; iel < cms3.els_p4().size(); ++iel)
        {
            // If the electron does not pass the "HAD_loose_v4" ID, we reject
            if (!electronID(iel, id_level_t::HAD_loose_v4))
                continue;

            // Instantiate a lepton object.
            MyLepton mylepton;

            // Get the LorentzVector of this electron.
            mylepton.p4 = cms3.els_p4()[iel];

            // Get the charge of this electron.
            mylepton.charge = cms3.els_charge()[iel];

            // Electrons' pdgId is 11 (electron with charge -1) or -13 (anti-electron with charge +1).
            mylepton.pdgId = mylepton.charge * (-11);

            // Save the lepton to a vector.
            good_leptons.push_back(mylepton);
        }

        // -~-~-~-~-~-~-~-
        // Event Selection
        // -~-~-~-~-~-~-~-
        // When we analyze data, we often analyze a subset of data.
        // We focus on certain phase-spaces by making "event selections".
        // In this example, we focus on the phase-space where we have only two good leptons.
        // This is so that we peak up the Z->ll decay.
        // If we had produced single Z boson through pp collision, we expect only two good leptons.
        // More specifically, we want two and only two good, opposite charge, and same "flavor" leptons.
        // "flavor" here means, electron, muon, or a tau. (same "flavor" of leptons.)

        // We want only two leptons total.
        //
        // I use this kind of coding style so that what I am selecting is clear.
        // |||                                                     |
        // VVV                                                     |
        if (!( good_leptons.size() == 2 )) continue; //            |
        //     ~~~~~~~~~~~~~~~~~~~~~~~~ <-- My event selection. <--|
        //
        //        Otherwise, it looks like, if (good_leptons.size() != 2) continue; <-- and sometimes the negation confuses me. (Am I selecting two muons? or NOT two muons?)
        //

        // We want same-flavor and opposite-charge leptons.
        if (!( good_leptons[0].pdgId == -good_leptons[1].pdgId )) continue;

        // To recap, if we reach this point, we have two good leptons with same flavor, and opposite charge.
        // Let's compute the invariant mass of the two lepton system.
        float dilepton_mass = (good_leptons[0].p4 + good_leptons[1].p4).mass();

        // Let's fill the histogram we created at the beginning of the code.
        hist_dilepton_mass->Fill(dilepton_mass, cms3.evt_scale1fb());
        //                                      ^^^^^^^^^^^^^^^^^^^
        //                                      Each event comes with a "weight".
        //                                      For data events, each event carries a weight of 1. (one real collision event is one real collision event... duh.)
        //                                      For Monte Carlo (MC) simulated events, each event can carry a different kind of weight.
        //                                      So for MC, use "evt_scale1fb".
        //                                      However, as of right now don't worry too much about the absolute weights.
        //                                      We'll focus more on the relative sizes between data and MC.

        // Now we're done with this event.

    }

    // Once the loop has finished, save the histograms to output.root.
    TFile* ofile = new TFile("output/"+output_name, "recreate"); // recreate option overwrites the file if output.root already exists.
    ofile->cd(); // go to ofile directory to write it here. (There are unnecessarily complex peculiarities with ROOT on where things are written and which object owns which instances and etc...)
    hist_dilepton_mass->Write();
    ofile->Close();

}

// eof
