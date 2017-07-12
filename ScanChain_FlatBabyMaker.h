//  .
// ..: P. Chang, philip@physics.ucsd.edu

// TasUtil tool
#include "tasutil.cc"

// CORE tools
#include "CORE/CMS3.cc"
#include "CORE/Base.cc"
#include "CORE/TriggerSelections.cc"
#include "CORE/ElectronSelections.cc"
#include "CORE/MuonSelections.cc"
#include "CORE/IsolationTools.cc"
#include "CORE/Tools/goodrun.cc"
#include "CORE/Tools/JetCorrector.cc"

// Tiny data structure for easy handling of physics objects
struct MyLepton
{
    LorentzVector p4; // 4-vector of the particle
    int charge;       // Charge of the lepton (i.e. either +1 or -1)
    int pdgId;        // PDG (Particle Data Group, go to  pdg.lbl.gov) has a standardized codes for each particles.
                      // See p2,3, and 4 of http://pdg.lbl.gov/2007/reviews/montecarlorpp.pdf.
};

void ScanChain(TChain* chain, TString output_name, int nevents=-1); // the default nevents=-1 option means loop over all events.

