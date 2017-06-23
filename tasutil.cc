#include "tasutil.h"

//==================================================================================================
TLorentzVector TasUtil::Calc::TLV(LorentzVector p)
{
    TLorentzVector tlv;
    tlv.SetPxPyPzE(p.x(), p.y(), p.z(), p.e());
    return tlv;
}

//==================================================================================================
float TasUtil::Calc::DEta(LorentzVector a, LorentzVector b) { return fabs(a.eta() - b.eta());       }
float TasUtil::Calc::DPhi(LorentzVector a, LorentzVector b) { return fabs(TLV(a).DeltaPhi(TLV(b))); }
float TasUtil::Calc::DR  (LorentzVector a, LorentzVector b) { return fabs(TLV(a).DeltaR  (TLV(b))); }
