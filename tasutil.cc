#include "tasutil.h"

//==================================================================================================
TLorentzVector TasUtil::Calc::TLV(LorentzVector p)
{
    TLorentzVector tlv;
    tlv.SetPxPyPzE(p.x(), p.y(), p.z(), p.e());
    return tlv;
}

//==================================================================================================
LorentzVector TasUtil::Calc::LV(TLorentzVector p)
{
    LorentzVector lv;
    lv.SetPxPyPzE(p.X(), p.Y(), p.X(), p.E());
    return lv;
}

//==================================================================================================
float TasUtil::Calc::DEta(LorentzVector a, LorentzVector b) { return fabs(a.eta() - b.eta());       }

//==================================================================================================
float TasUtil::Calc::DPhi(LorentzVector a, LorentzVector b) { return fabs(TLV(a).DeltaPhi(TLV(b))); }

//==================================================================================================
float TasUtil::Calc::DR  (LorentzVector a, LorentzVector b) { return fabs(TLV(a).DeltaR  (TLV(b))); }

//==================================================================================================
float TasUtil::Calc::MT  (LorentzVector a, float met, float met_phi)
{
    /// Compute MT
    TLorentzVector metp4;
    metp4.SetPtEtaPhiE(met, 0, met_phi, met);
    float mt = sqrt(2 * a.Pt() * metp4.Pt() * ( 1 - cos(metp4.DeltaPhi(p4) ) ));
    return mt;
}
