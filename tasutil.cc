#include "tasutil.h"

//==================================================================================================
TLorentzVector Calc::TLV(LorentzVector p)
{
    TLorentzVector tlv;
    tlv.SetPxPyPzE(p.x(), p.y(), p.z(), p.e());
    return tlv;
}

//==================================================================================================
float Calc::DEta(LorentzVector a, LorentzVector b) { return fabs(a.eta() - b.eta());       }
float Calc::DPhi(LorentzVector a, LorentzVector b) { return fabs(TLV(a).DeltaPhi(TLV(b))); }
float Calc::DR  (LorentzVector a, LorentzVector b) { return fabs(TLV(a).DeltaR  (TLV(b))); }
