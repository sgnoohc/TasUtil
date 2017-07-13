//  .
// ..: P. Chang, philip@physics.ucsd.edu

// ROOT
#include "TBenchmark.h"

// TasUtil tool
#include "tasutil.cc"

//// CORE
//#include "../CORE/CMS3.h"
//#include "../CORE/Base.h"
//#include "../CORE/OSSelections.h"
//#include "../CORE/SSSelections.h"
//#include "../CORE/VVVSelections.h"
//#include "../CORE/ElectronSelections.h"
//#include "../CORE/IsolationTools.h"
//#include "../CORE/JetSelections.h"
//#include "../CORE/MuonSelections.h"
//#include "../CORE/IsoTrackVeto.h"
//#include "../CORE/PhotonSelections.h"
//#include "../CORE/TriggerSelections.h"
//#include "../CORE/VertexSelections.h"
//#include "../CORE/MCSelections.h"
//#include "../CORE/MetSelections.h"
//#include "../CORE/SimPa.h"
//#include "../CORE/Tools/JetCorrector.h"
//#include "../CORE/Tools/MT2/MT2.h"
//#include "../CORE/Tools/hemJet.h"
//#include "../CORE/Tools/utils.h"
//#include "../CORE/Tools/goodrun.h"
//#include "../CORE/Tools/btagsf/BTagCalibrationStandalone.h"


using namespace std;
using namespace tas;

// Tiny data structure for easy handling of physics objects
struct MyLepton
{
    LorentzVector p4; // 4-vector of the particle
    int charge;       // Charge of the lepton (i.e. either +1 or -1)
    int pdgId;        // PDG (Particle Data Group, go to  pdg.lbl.gov) has a standardized codes for each particles.
                      // See p2,3, and 4 of http://pdg.lbl.gov/2007/reviews/montecarlorpp.pdf.
};

void ScanChain(TChain* chain, TString output_name, int nevents=-1); // the default nevents=-1 option means loop over all events.

