#ifndef tasutil_h
#define tasutil_h

#include "TLorentzVector.h"

namespace TasUtil
{
    namespace Calc
    {
        // Short hand for float LorentzVector
        typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > LorentzVector;

        // TLorentzVector
        TLorentzVector TLV(LorentzVector);

        // LorentzVector
        float DPhi(LorentzVector, LorentzVector);
        float DEta(LorentzVector, LorentzVector);
        float DR  (LorentzVector, LorentzVector);
    }
}

#endif
