# TasUtil

## Test command
    sh run.sh ScanChain_FlatBabyMaker /hadoop/cms/store/group/snt/run2_moriond17/WJetsToLNu_TuneCUETP8M1_13TeV-amcatnloFXFX-pythia8_RunIISummer16MiniAODv2-PUMoriond17_80X_mcRun2_asymptotic_2016_TrancheIV_v6-v1/V08-00-16/merged_ntuple_1.root Events testSummer16.root 100

## What can it do?

  - Looper
  - AutoHist
  - Math

## What can it do?

Learning by examples is often the fastest way to learn something.
So I will first list the example usage case and explain the features and setup later.

### Example: Simple CMS3 looper

A super simple CMS3 looper looks like this. (Assuming you have [CORE](https://github.com/cmstas/CORE/) checked out in the current working directory.)

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

Then run,

    root -l -b -q run.C

This will loop over these root files and print transverse momentum of muons in the events for all events ```std::cout```.

### Example: Math functions in TasUtil

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

## Features

- class Looper

   The Looper class allows user to loop over events in a TChain easily.
   The pre-requisite is that the TTree being looped over will be accessed by a separate class.
   This "Tree Class" must contain ```Init```, ```GetEntry```, and ```progress``` methods.
   One can create this kind of class using this [code](https://github.com/cmstas/Software/blob/master/makeCMS3ClassFiles/makeCMS3ClassFiles.C).

- namespace Math

   This namespace will hold various physics quantities that we compute over and over again.

- class Particle

   The Particle class is a bundle of various ```map<TString, 'value'>``` where, ```'value'``` can be a float, int, TLorentzVector, or TString.
   Idea is to have an object that holds various 'properties'.
   It's called "Particle", largely because a 'particle' is really nothing more than an object with various 'properties'.
   TODO: Will add a feature to write this "particle" to a TTree easily.

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

