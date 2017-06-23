# TasUtil

All you have to do to use the functions for almost any ROOT application is to source ```thistasutil.sh``` and just use like the following.

    #include "tasutil.cc"

    void ScanChain( ... )
    {
        LorentzVector a;
        LorentzVector b;
        ...
        ...
        float dphi = TasUtil::Calc::DPhi(a, b);
    }
