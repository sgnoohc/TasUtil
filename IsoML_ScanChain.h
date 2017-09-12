//  .
// ..: P. Chang, philip@physics.ucsd.edu

// TasUtil tool
#define INCLUDE_CORE
#include "tasutil.cc"

#include "Math/VectorUtil.h"

using namespace std;

void ScanChain(TChain* chain, TString output_name, TString optstr, int nevents=-1); // the default nevents=-1 option means loop over all events.

std::vector<unsigned int> goodMuonIdx();
std::vector<unsigned int> goodElecIdx();

void fill( TasUtil::TTreeX* ttree, std::vector<unsigned int >& idx, int pdgid );


