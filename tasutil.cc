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
float TasUtil::Calc::DEta(LorentzVector a, LorentzVector b) { return DEta(TLV(a), TLV(b)); }
float TasUtil::Calc::DPhi(LorentzVector a, LorentzVector b) { return DPhi(TLV(a), TLV(b)); }
float TasUtil::Calc::DR  (LorentzVector a, LorentzVector b) { return DR  (TLV(a), TLV(b)); }
float TasUtil::Calc::DPt (LorentzVector a, LorentzVector b) { return DPt (TLV(a), TLV(b)); }
float TasUtil::Calc::Mass(LorentzVector a, LorentzVector b) { return Mass(TLV(a), TLV(b)); }
float TasUtil::Calc::Pt  (LorentzVector a, LorentzVector b) { return Pt  (TLV(a), TLV(b)); }
float TasUtil::Calc::MT  (LorentzVector a, float met, float met_phi) { return MT(TLV(a), makeMETTLV(met, met_phi)); }
float TasUtil::Calc::MT  (LorentzVector a, LorentzVector b) { return MT(TLV(a), TLV(b)); }

//==================================================================================================
float TasUtil::Calc::DEta(TLorentzVector obj1, TLorentzVector obj2) { return fabs(obj1.Eta() - obj2.Eta()); }
float TasUtil::Calc::DPhi(TLorentzVector obj1, TLorentzVector obj2) { return fabs(obj1.DeltaPhi(obj2));     }
float TasUtil::Calc::DR  (TLorentzVector obj1, TLorentzVector obj2) { return fabs(obj1.DeltaR(obj2));       }
float TasUtil::Calc::DPt (TLorentzVector obj1, TLorentzVector obj2) { return fabs(obj1.Pt()-obj2.Pt());     }
float TasUtil::Calc::Mass(TLorentzVector obj1, TLorentzVector obj2) { return (obj1 + obj2).M();             }
float TasUtil::Calc::Pt  (TLorentzVector obj1, TLorentzVector obj2) { return (obj1 + obj2).Pt();            }
float TasUtil::Calc::MT  (TLorentzVector p4  , float met, float met_phi) { return MT(p4, makeMETTLV(met, met_phi)); }
float TasUtil::Calc::MT  (TLorentzVector p4  , TLorentzVector metp4)
{
    /// Compute MT
    float mt = sqrt(2 * p4.Pt() * metp4.Pt() * (1 - cos(metp4.DeltaPhi(p4))));
    return mt;
}

//==================================================================================================
LorentzVector  TasUtil::Calc::makeMETLV (float met, float met_phi) { return LV(makeMETTLV(met, met_phi)); }
TLorentzVector TasUtil::Calc::makeMETTLV(float met, float met_phi);
{
    TLorentzVector tlv;
    tlv.SetPtEtaPhiE(met, 0, met_phi, met);
    return tlv;
}

