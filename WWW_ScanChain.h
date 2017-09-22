//  .
// ..: P. Chang, philip@physics.ucsd.edu

// TasUtil tool
#include "tasutil.cc"
#include "WWW_CORE/WWWTree.h"
#include "WWW_CORE/WWWTools.h"

#include "CORE/Tools/dorky/dorky.h"

using namespace std;

#define MAXOBJ 3
#define NSYST 3

void ScanChain( TChain* chain, TString output_name, TString optstr, int nevents = -1 ); // the default nevents=-1 option means loop over all events.

bool doAnalysis( TasUtil::AutoHist& );

void fillHistograms( bool& passed, TasUtil::AutoHist&, TString, int regionid, int isyst );
void fillHistogramsFull( TasUtil::AutoHist&, TString, TString, TString, int regionid, int isyst );
void fillLepHistograms( TasUtil::AutoHist&, TString, TString, TString );
void fillJetHistograms( TasUtil::AutoHist&, TString, TString, TString );
void fillWWWHistograms( TasUtil::AutoHist&, TString );

void printevent( TString );

//void doTmpAnalysis( TasUtil::AutoHist& hists );
//void createBranches( TasUtil::TTreeX& tree );
//void fillTree( TasUtil::TTreeX& tree, int regionid );
//void fillHist( TasUtil::AutoHist&, TString );
//void doSS( TasUtil::AutoHist& );
//void doSSSideband( TasUtil::AutoHist& );
//void doSSAR( TasUtil::AutoHist& );
//void doSSMMFull( TasUtil::AutoHist& );
//void WZStudy( TasUtil::AutoHist&, TString, TString );
