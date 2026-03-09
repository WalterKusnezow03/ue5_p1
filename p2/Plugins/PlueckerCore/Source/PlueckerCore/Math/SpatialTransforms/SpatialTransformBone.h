#pragma once

#include "PlueckerCore/Math/SpatialTransforms/SpatialTransform.h"

class PLUECKERCORE_API SpatialTransformBone : public SpatialTransform {

public:
    //local transform of bone
    MMatrix Transform() override {
        //M = R * T <-- lese richtung
        MMatrix t = Translation();
        MMatrix r = Rotation();
        return r * t;
    }

protected:
    virtual FVector &axis() override {
        return translation;
    }
};