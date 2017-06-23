#ifndef tasutil_h
#define tasutil_h

namespace Calc
{
    typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > LorentzVector;
    DPhi(LorentzVector, LorentzVector);
    DEta(LorentzVector, LorentzVector);
    DR  (LorentzVector, LorentzVector);
    Mass(LorentzVector, LorentzVector);
}

#endif
