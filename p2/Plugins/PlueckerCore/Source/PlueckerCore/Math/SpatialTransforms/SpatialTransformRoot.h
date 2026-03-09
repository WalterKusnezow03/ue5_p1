#pragma once

#include "PlueckerCore/Math/SpatialTransforms/SpatialTransform.h"

class PLUECKERCORE_API SpatialTransformRoot : public SpatialTransform {

public:
    //local transform of bone
    MMatrix Transform() override {
        //M = T * R <-- lese richtung
        MMatrix t = Translation();
        MMatrix r = Rotation();
        return t * r;
    }


    virtual FVector Torque(const FVector &force, const FVector &centerOfMass) override {
        return FVector(0, 0, 0);
    }

protected:
    FVector none = FVector(0, 0, 0);
    virtual FVector &axis() override {
        return none;
    }
};