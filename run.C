void run( TString scanchainname, TString input_path, TString treename, TString output_path )
{
    gSystem->Load( "code/CMS3_CORE.so" );
    gROOT->ProcessLine( ".L " + "code/" + scanchainname + ".C+" );
    gROOT->ProcessLine( "TString input_path = \"" + input_path + "\";" );
    gROOT->ProcessLine( "TString output_path = \"" + output_path + "\";" );
    gROOT->ProcessLine( "TString ttreename = \"" + treename"\";" );
    gROOT->ProcessLine( "TChain *chain = new TChain(ttreename);" );
    gROOT->ProcessLine( "TObjArray* files = input_path.Tokenize(\",\");" );
    gROOT->ProcessLine( "for (unsigned int ifile = 0; ifile < files->GetEntries(); ++ifile) { TString filepath = ((TObjString*) files->At(ifile))->GetString(); std::cout << \"Adding to TChain: file = \" << filepath << std::endl; chain->Add(filepath); }" );
    gROOT->ProcessLine( scanchainname+"(chain,output_path);" );
}
