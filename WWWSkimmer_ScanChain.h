//  .
// ..: P. Chang, philip@physics.ucsd.edu

// TasUtil tool
#include "tasutil.cc"
#include "WWW_CORE/WWWTree.h"
#include "WWW_CORE/WWWTools.h"
#include "CORE/Tools/dorky/dorky.h"

using namespace std;

void ScanChain( TChain* chain, TString output_name, TString optstr, int nevents = -1 ); // the default nevents=-1 option means loop over all events.
bool passSkim();
bool passpresel( int& idx );
