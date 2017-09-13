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
    if ( output_name.Contains( "duplicate_removed" ) )
        looper.setSkimMaxSize( 400*1024*1024 );
    while ( looper.nextEvent() )
    {
        if ( wwwbaby.isData() )
        {
            duplicate_removal::DorkyEventIdentifier id( wwwbaby.run(), wwwbaby.evt(), wwwbaby.lumi() );
            if ( is_duplicate( id ) )
                continue;
        }
        setObjectIndices();
        if ( passSkim() )
        {
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
