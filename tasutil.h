#ifndef tasutil_h
#define tasutil_h

#include "TLorentzVector.h"
#include "Math/LorentzVector.h"

namespace TasUtil
{
    namespace Calc
    {
        // Short hand for float LorentzVector
        typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > LorentzVector;

        // TLorentzVector
        TLorentzVector TLV(LorentzVector);
        LorentzVector  LV (TLorentzVector);

        // LorentzVector
        float DEta(LorentzVector, LorentzVector);
        float DPhi(LorentzVector, LorentzVector);
        float DR  (LorentzVector, LorentzVector);
        float DPt (LorentzVector, LorentzVector);
        float Mass(LorentzVector, LorentzVector);
        float Pt  (LorentzVector, LorentzVector);
        float MT  (LorentzVector, float, float);
        float MT  (LorentzVector, LorentzVector);

        // TLorentzVector
        float DEta(TLorentzVector, TLorentzVector);
        float DPhi(TLorentzVector, TLorentzVector);
        float DR  (TLorentzVector, TLorentzVector);
        float DPt (TLorentzVector, TLorentzVector);
        float Mass(TLorentzVector, TLorentzVector);
        float Pt  (TLorentzVector, TLorentzVector);
        float MT  (TLorentzVector, float, float);
        float MT  (TLorentzVector, TLorentzVector);

        // Creating MET LorentzVectors
        LorentzVector  makeMETLV (float, float);
        TLorentzVector makeMETTLV(float, float);
    }
}

#endif
