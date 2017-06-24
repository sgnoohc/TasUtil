# TasUtil

## Setup

The functions reside in one implementation file ```tasutil.cc``` and one header file ```tasutil.h```.
The user will have to simply include the source files using ```#include```.
To have them directly accessible without pointing to relative path source ```thistasutil.sh```.
It will basically add the path to the directory to ```$ROOT_INCLUDE_PATH```.

    source TasUtil/thistasutil.sh

The magic export command is the following,

    export ROOT_INCLUDE_PATH=$ROOT_INCLUDE_PATH:$DIR

This allows the following statement in your ROOT macros to work.

    #include "tasutil.cc"

## Example: Simple CMS3 looper

A super simple CMS3 looper looks like this

In a file called ```ScanChain.C``` copy paste the following

    #include "CORE/CMS3.cc"
    #include "tasutil.cc"

    void ScanChain(TChain* chain)
    {
        TasUtil::Looper<CMS3> looper(chain, &cms3);
        while (looper.nextEvent())
        {
            // do your cms3 stuff
            for (auto& p4: cms3.mus_p4())
                TasUtil::print(TString::Format("muon Pt = %f", p4.pt()));
        }
    }

And to actually loop over files create an entry script like the following and call it ```run.C```

    {
        gROOT->LoadMacro("ScanChain.C+");
        TChain *ch = new TChain("Events");
        ch->Add("/hadoop/cms/store/group/snt/run2_data_test/DoubleMuon_Run2017A-PromptReco-v2/V00-00-03_workaround/merged_ntuple_*.root");
        ch->Add("/hadoop/cms/store/group/snt/run2_data_test/DoubleEG_Run2017A-PromptReco-v2/V00-00-03_workaround/merged_ntuple_*.root");
        ScanChain(ch);
    }

## Example: Math functions in TasUtil

The usual variables can be calculated using functions in Math namespace.

    #include "tasutil.cc"

    void ScanChain( ... )
    {
        LorentzVector a;
        LorentzVector b;
        ...
        ...
        float dphi = TasUtil::Math::DPhi(a, b);
    }

