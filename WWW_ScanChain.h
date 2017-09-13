//  .
// ..: P. Chang, philip@physics.ucsd.edu

// TasUtil tool
#include "tasutil.cc"
#include "WWW_CORE/WWWTree.h"
#include "WWW_CORE/WWWTools.h"

#include "CORE/Tools/dorky/dorky.h"

using namespace std;

#define MAXOBJ 3

void ScanChain( TChain* chain, TString output_name, TString optstr, int nevents = -1 ); // the default nevents=-1 option means loop over all events.

void doAnalysis( TasUtil::AutoHist&, TasUtil::EventList& );
//bool passpresel( int& idx );
void doTmpAnalysis( TasUtil::AutoHist& hists );
//void doSSAnalysis( TasUtil::AutoHist& hists, int cutidx );

void fillHistograms( TasUtil::AutoHist&, TString, int regionid );
void fillHistogramsFull( TasUtil::AutoHist&, TString, TString, TString, int regionid );
void fillLepHistograms( TasUtil::AutoHist&, TString, TString, TString );
void fillJetHistograms( TasUtil::AutoHist&, TString, TString, TString );
void fillWWWHistograms( TasUtil::AutoHist&, TString );

//void createBranches( TasUtil::TTreeX& tree );
//void fillTree( TasUtil::TTreeX& tree, int regionid );
//void fillHist( TasUtil::AutoHist&, TString );
//void doSS( TasUtil::AutoHist& );
//void doSSSideband( TasUtil::AutoHist& );
//void doSSAR( TasUtil::AutoHist& );
//void doSSMMFull( TasUtil::AutoHist& );
//void WZStudy( TasUtil::AutoHist&, TString, TString );
