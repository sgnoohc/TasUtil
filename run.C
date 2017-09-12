void run( TString scanchainname, const char* input_path, TString treename, TString output_path, TString nevents = "-1", TString compilerflag = "" )
{

    // Load all *.so files in current directory
    TSystemDirectory dir( "./", "./" );
    TList *files = dir.GetListOfFiles();

    if ( files )
    {
        TSystemFile *file;
        TString fname;
        TIter next( files );

        while ( ( file = ( TSystemFile* )next() ) )
        {
            fname = file->GetName();

            if ( !file->IsDirectory() && fname.EndsWith( "CORE.so" ) )
                gSystem->Load( fname );
        }
    }
    gROOT->ProcessLine( ".L " + scanchainname + ".C+" + compilerflag );
    gROOT->ProcessLine( Form( "TString input_path = \"%s\";", input_path ) );
    gROOT->ProcessLine( "TString output_path = \"" + output_path + "\";" );
    gROOT->ProcessLine( "TString ttreename = \"" + treename + "\";" );
    gROOT->ProcessLine( "TChain *chain = new TChain(ttreename);" );
    gROOT->ProcessLine( "TObjArray* files = input_path.Tokenize(\",\");" );
    gROOT->ProcessLine( "for (unsigned int ifile = 0; ifile < files->GetEntries(); ++ifile) { TString filepath = ((TObjString*) files->At(ifile))->GetString(); std::cout << \"Adding to TChain: file = \" << filepath << std::endl; chain->Add(filepath); }" );
    gROOT->ProcessLine( "TString CMSxVersion = TString(gSystem->BaseName(gSystem->DirName(chain->GetListOfFiles()->At(0)->GetTitle())));");//.ReplaceAll(\"/\",\"_\").ReplaceAll(\"-\",\"_\");" );
    gROOT->ProcessLine( "TString sample_name = TString(gSystem->BaseName(gSystem->DirName(gSystem->DirName(chain->GetListOfFiles()->At(0)->GetTitle()))));");//.ReplaceAll(\"/\",\"_\").ReplaceAll(\"-\",\"_\");" );
    gROOT->ProcessLine( "TString file_name   = TString(gSystem->BaseName(gSystem->BaseName(chain->GetListOfFiles()->At(0)->GetTitle())));");//.ReplaceAll(\"/\",\"_\").ReplaceAll(\"-\",\"_\");" );
//    gROOT->ProcessLine( "TString sample_name_based_opt_string = sample_name + \"_\" + CMSxVersion + \"_\" + file_name;");//.ReplaceAll(\"/\",\"_\").ReplaceAll(\"-\",\"_\");" );
    gROOT->ProcessLine( "TString sample_name_based_opt_string = input_path" );
    gROOT->ProcessLine( "std::cout << \"base_optstr = \" << sample_name_based_opt_string << std::endl;" );
    gROOT->ProcessLine( "std::cout << std::endl;" );
    gROOT->ProcessLine( "std::cout << std::endl;" );
    gROOT->ProcessLine( "std::cout << \"   .\" << std::endl;" );
    gROOT->ProcessLine( "std::cout << \"  ..: Start " + scanchainname + " ...\" << std::endl;" );
    gROOT->ProcessLine( "std::cout << std::endl;" );
    gROOT->ProcessLine( "std::cout << std::endl;" );
    gROOT->ProcessLine( "ScanChain(chain,output_path,sample_name_based_opt_string," + nevents + ");" );
    gROOT->ProcessLine( "std::cout << std::endl;" );
    gROOT->ProcessLine( "std::cout << std::endl;" );
    gROOT->ProcessLine( "std::cout << \"   .\" << std::endl;" );
    gROOT->ProcessLine( "std::cout << \"  ..: Finished " + scanchainname + " ...\" << std::endl;" );
    gROOT->ProcessLine( "std::cout << std::endl;" );
    gROOT->ProcessLine( "std::cout << std::endl;" );
    
}
