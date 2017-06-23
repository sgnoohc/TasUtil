# TasUtil

## Quick start

All you have to do to use the functions for almost any ROOT application is to source ```thistasutil.sh```,

    source TasUtil/thistasutil.sh


And just use like the following.

    #include "tasutil.cc"

    void ScanChain( ... )
    {
        LorentzVector a;
        LorentzVector b;
        ...
        ...
        float dphi = TasUtil::Calc::DPhi(a, b);
    }

## Example: Simple CMS3 looper

A super simple CMS3 looper looks like this

In a file called ```ScanChain.C``` copy paste the following

    #include "/home/users/phchang/cmstas/CORE_cms4/CMS3.cc"
    #include "tasutil.cc"

    void ScanChain(TChain* chain)
    {
        TasUtil::EL<CMS3> el(chain, &cms3);
        while (el.nextTree())
        {
            while (el.nextEvent())
            {
                // do your cms3 stuff
                for (auto& p4: cms3.mus_p4())
                    TasUtil::print(TString::Format("muon Pt = %f", p4.pt()));
            }
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


